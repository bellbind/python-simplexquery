//#include <iostream>
#include <cstring>
#include <sstream>
#include <xqilla/xqilla-simple.hpp>
#include <xqilla/context/URIResolver.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/util/XMLURL.hpp>

extern "C" {

extern void * (* get_malloc())(size_t);
extern void (* get_free())(void *);

}

void * operator new(size_t size) {
    return get_malloc()(size);
}
void operator delete(void * p) {
    get_free()(p);
}

namespace {
    XERCES_CPP_NAMESPACE_USE
    // use external memory management
    class CustomMemoryManager : public MemoryManager {
    public:
        virtual MemoryManager* getExceptionMemoryManager() {
            return this;
        }
#if XERCES_VERSION_MAJOR >= 3
        virtual void * allocate(XMLSize_t size) {
            return get_malloc()(size);
        }
#else
        virtual void * allocate(size_t size) {
            return get_malloc()(size);
        }
#endif
        virtual void deallocate(void* p) {
            get_free()(p);
        }
    };
    CustomMemoryManager memory_manager;
    
    char * alloc_strbuf(std::string stdval) 
    {
        char * buf = reinterpret_cast<char *>(
            get_malloc()(stdval.length() + 1));
        if (buf == NULL) return NULL;
        memcpy(buf, stdval.c_str(), stdval.length() + 1);
        return buf;
    }
    
    // URIResolver for invoking callback Python callable
    class CallbackURIResolver : public URIResolver 
    {
    private:
        char * (* callback)(void *, const char *);
        void * callback_arg;
    
    public:
        CallbackURIResolver(
            char * (* _callback)(void *, const char *), 
            void * _callback_arg) :
            callback(_callback), callback_arg(_callback_arg) {}
        virtual ~CallbackURIResolver() {}
        
        virtual bool 
        resolveDocument(Sequence & result, const XMLCh * uri, 
                        DynamicContext * context, 
                        const QueryPathNode * projection) {
            //std::cout << UTF8(uri) << std::endl;
            //std::cout << UTF8(context->getBaseURI()) << std::endl;
            const XMLCh * id = uri;
            XMLURL url(context->getMemoryManager());
            if (url.setURL(context->getBaseURI(), uri, url)) {
                id = context->getMemoryManager()->getPooledString(
                    url.getURLText());
            }
            std::string stduri(UTF8(id));
            
            char * cxml = callback(callback_arg, stduri.c_str());
            if (!cxml) return false;
            std::string xml(cxml);
            get_free()(cxml);
            
            MemBufInputSource is(
                reinterpret_cast<const XMLByte *>(xml.c_str()),
                xml.length(), id);
            Node::Ptr doc = context->parseDocument(is);
            result.addItem(doc);
            return true;
        }
        
        virtual bool 
        resolveCollection(Sequence & result, const XMLCh * uri, 
                          DynamicContext * context, 
                          const QueryPathNode * projection) {
            return false;
        }
        
        virtual bool 
        resolveDefaultCollection(Sequence & result, 
                                 DynamicContext * context, 
                                 const QueryPathNode * projection) {
            return false;
        }
        
        virtual bool putDocument(const Node::Ptr & document, 
                                 const XMLCh * uri, 
                                 DynamicContext * context) {
            return false;
        }
    };
    
    // function as a class
    class Executor {
        MemoryManager * memory_manager;
        const char * xquery;
        const char * context_xml;
        char * (* resolver)(void *, const char *);
        void * resolver_arg;
        XQilla xqilla;
        AutoDelete<XQQuery> query;
        AutoDelete<DynamicContext> context;
        CallbackURIResolver uri_resolver;
    public:
        Executor(MemoryManager * _memory_manager,
                 const char * _xquery, const char * _context_xml,
                 char * (* _resolver)(void *, const char *), 
                 void * _resolver_arg) :
            memory_manager(_memory_manager), 
            xquery(_xquery), context_xml(_context_xml), 
            resolver(_resolver), resolver_arg(_resolver_arg),
            xqilla(memory_manager), query(NULL), context(NULL),
            uri_resolver(resolver, resolver_arg) {
        }
        
        char * execute() {
            setup();
            Result result = query->execute(context);
            
            Item::Ptr item;
            while (item = result->next(context)) {
                std::string stdval(UTF8(item->asString(context)));
                return alloc_strbuf(stdval);
            }
            return NULL;
        }
        
        int execute_all(void (* callback)(void *, const char *), 
                        void * callback_arg) {
            setup();
            Result result = query->execute(context);
            
            Item::Ptr item;
            while (item = result->next(context)) {
                std::string stdval(UTF8(item->asString(context)));
                callback(callback_arg, stdval.c_str());
            }
            return 1;
        }
    
    private:
        void setup() {
            query.set(xqilla.parse(X(xquery)));
            context.set(query->createDynamicContext());
            if (context_xml) {
                std::string xml(context_xml);
                MemBufInputSource is(
                    reinterpret_cast<const XMLByte *>(xml.c_str()), 
                    xml.length(), ".");
                Node::Ptr doc = context->parseDocument(is);
                Sequence seq(doc);
                if (!seq.isEmpty() && seq.first()->isNode()) {
                    context->setContextItem(seq.first());
                    context->setContextPosition(1);
                    context->setContextSize(1);
                }
            }
            if (resolver) {
                context->registerURIResolver(&uri_resolver, false);
            }
        }
    };
}

extern "C" { 

extern char *
execute(const char * xquery, const char * context_xml,
        char * (* resolver)(void *, const char *), void * resolver_arg)
{
    Executor executor(
        &memory_manager, xquery, context_xml, resolver, resolver_arg);
    try {
        return executor.execute();
    } catch (XQException & ex) {
        //std::cout << UTF8(ex.getError()) << std::endl;
    } catch (...) {
        //std::cout << "ex" << std::endl;
    }
    return NULL;
}

extern int
execute_all(const char * xquery, const char * context_xml, 
            char * (* resolver)(void *, const char *), 
            void * resolver_arg,
            void (* callback)(void *, const char *), void * callback_arg,
            char ** error_out)
{
    Executor executor(
        &memory_manager, xquery, context_xml, resolver, resolver_arg);
    try {
        try {
            return executor.execute_all(callback, callback_arg);
        } catch (XQException & ex) {
            if (error_out) {
                std::ostringstream message;
                message << "<" << UTF8(ex.getType()) << ">" <<
                    "[" << UTF8(ex.getXQueryFile()) << 
                    ":" << ex.getXQueryLine() <<
                    ":" << ex.getXQueryColumn() << "]" <<
#ifdef DEBUG
                    "(" << UTF8(ex.getCppFunction()) << 
                    "|" << ex.getCppFile() <<
                    ":" << ex.getCppLine() << ")" <<
#endif
                    " " << UTF8(ex.getError()) ;
                std::string stdval = message.str();
                *error_out = alloc_strbuf(stdval);
            }
        }
    } catch (...) {
        //std::cout << "ex" << std::endl;
        if (error_out) {
            std::string stdval("unknown C++ exception");
            *error_out = alloc_strbuf(stdval);
        }
    }
    return 0;
}

extern const char *
get_xerces_version()
{
    return XERCES_FULLVERSIONDOT;
}

}

