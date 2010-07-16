//#include <iostream>
#include <cstring>
#include <xqilla/xqilla-simple.hpp>
#include <xqilla/context/URIResolver.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemoryManager.hpp>
#include <xercesc/util/XMLURL.hpp>

extern "C" {

extern void * (* get_malloc())(size_t);
extern void (* get_free())(void *);

}

namespace {
    XERCES_CPP_NAMESPACE_USE
    // use external memory management
    class CustomMemoryManager : public MemoryManager {
    public:
        virtual MemoryManager* getExceptionMemoryManager() {
            return this;
        }
        virtual void * allocate(XMLSize_t size) {
            return get_malloc()(size);
        }
        virtual void * allocate(size_t size) {
            return get_malloc()(size);
        }
        virtual void deallocate(void* p) {
            get_free()(p);
        }
    };
    CustomMemoryManager memory_manager;
    
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
            const XMLCh * id = uri;
            XMLURL url(context->getMemoryManager());
            if (url.setURL(context->getBaseURI(), uri, url)) {
                id = context->getMemoryManager()->getPooledString(
                    url.getURLText());
            }
            std::string stduri(UTF8(url.getURLText()));
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
            xqilla(memory_manager), query(xqilla.parse(X(xquery))), 
            context(query->createDynamicContext()),
            uri_resolver(resolver, resolver_arg) {
            setup();
        }
        
        char * execute() {
            Result result = query->execute(context);
            
            Item::Ptr item;
            while (item = result->next(context)) {
                std::string stdval(UTF8(item->asString(context)));
                char * buf = reinterpret_cast<char *>(
                    get_malloc()(stdval.length() + 1));
                if (buf == NULL) return NULL;
                memcpy(buf, stdval.c_str(), stdval.length() + 1);
                return buf;
            }
            return NULL;
        }
        
        int execute_all(void (* callback)(void *, const char *), 
                        void * callback_arg) {
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
    try {
        Executor executor(&memory_manager, xquery, context_xml, resolver, resolver_arg);
        return executor.execute();
    } catch (XQException ex) {
        ;
    } catch (...) {}
    return NULL;
}

extern int
execute_all(const char * xquery, const char * context_xml, 
            char * (* resolver)(void *, const char *), 
            void * resolver_arg,
            void (* callback)(void *, const char *), void * callback_arg)
{
    try {
        Executor executor(&memory_manager, xquery, context_xml, resolver, resolver_arg);
        return executor.execute_all(callback, callback_arg);
    } catch (XQException ex) {
        ;
    } catch (...) {}
    return 0;
}

}

