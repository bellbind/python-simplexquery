//#include <iostream>
#include <cstdlib>
#include <cstring>
#include <xqilla/xqilla-simple.hpp>
#include <xqilla/context/URIResolver.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/util/XMLURL.hpp>

class CallbackURIResolver : public URIResolver 
{
private:
  char * (* _callback)(void *, const char *);
  void * _callback_arg;
  
public:
  CallbackURIResolver(char * (* callback)(void *, const char *), 
		      void * callback_arg) :
    _callback(callback), _callback_arg(callback_arg) {}
  virtual ~CallbackURIResolver() {}
  
  
  virtual bool 
  resolveDocument(Sequence & result, const XMLCh * uri, 
		  DynamicContext * context, 
		  const QueryPathNode * projection) {
    const XMLCh * systemId = uri;
    XERCES_CPP_NAMESPACE::XMLURL url(context->getMemoryManager());
    if(url.setURL(context->getBaseURI(), uri, url)) {
      systemId = context->getMemoryManager()->getPooledString(url.getURLText());
    }
    std::string stduri(UTF8(url.getURLText()));
    char * cxml = _callback(_callback_arg, stduri.c_str());
    if (!cxml) return false;
    std::string xml(cxml);
    free(cxml);
    
    XERCES_CPP_NAMESPACE::MemBufInputSource is((XMLByte*) xml.c_str(), 
					       xml.length(), systemId);
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
  
  virtual bool putDocument(const Node::Ptr & document, const XMLCh * uri, 
			   DynamicContext * context) {
    return false;
  }
};

class Executor {
  const char * xquery;
  const char * context_xml;
  char * (* url_resolver)(void *, const char *);
  void * url_resolver_arg;
  XQilla xqilla;
  AutoDelete<XQQuery> query;
  AutoDelete<DynamicContext> context;
  
public:
  Executor(const char * _xquery, const char * _context_xml,
	  char * (* _url_resolver)(void *, const char *), 
	  void * _url_resolver_arg) :
    xquery(_xquery), context_xml(_context_xml), url_resolver(_url_resolver),
    url_resolver_arg(_url_resolver_arg),
    xqilla(), query(xqilla.parse(X(xquery))), 
    context(query->createDynamicContext()) {}
  
  char * execute() {
    setup();
    
    Result result = query->execute(context);
    
    Item::Ptr item;
    while (item = result->next(context)) {
      std::string stdval(UTF8(item->asString(context)));
      //std::cout << stdval << std::endl;
      char * buf = (char *) malloc(stdval.length() + 1);
      strcpy(buf, stdval.c_str());
      return buf;
    }
    return NULL;
  }

  int execute_all(void (* callback)(void *, const char *), void * callback_arg) {
    setup();
    Result result = query->execute(context);
    
    Item::Ptr item;
    while (item = result->next(context)) {
      // it must keep the u8val instance until copy finished
      std::string stdval(UTF8(item->asString(context)));
      callback(callback_arg, stdval.c_str());
    }
    return 1;
  }
  
private:
  void setup() {
    if (context_xml) {
      std::string xml(context_xml);
      XERCES_CPP_NAMESPACE::MemBufInputSource is((XMLByte*) xml.c_str(), 
						 xml.length(), "input");
      Node::Ptr doc = context->parseDocument(is);
      Sequence seq(doc);
      if (!seq.isEmpty() && seq.first()->isNode()) {
	context->setContextItem(seq.first());
	context->setContextPosition(1);
	context->setContextSize(1);
      }
    }
    if (url_resolver) {
      URIResolver * resolver = new CallbackURIResolver(url_resolver, url_resolver_arg);
      context->registerURIResolver(resolver, true);
    }
  }
};

extern "C" { 
  extern char *
  execute(const char * xquery, const char * context_xml,
	  char * (* url_resolver)(void *, const char *), 
	  void * url_resolver_arg)
  {
    try {
      Executor executor(xquery, context_xml, url_resolver, url_resolver_arg);
      return executor.execute();
    } catch (XQException ex) {
      ;
    }
    return NULL;
    /*
    //std::cout << xquery << std::endl;
    //std::cout << context_xml << std::endl;
    try {
      XQilla xqilla;
      AutoDelete<XQQuery> query(xqilla.parse(X(xquery)));
      AutoDelete<DynamicContext> context(query->createDynamicContext());
      
      if (context_xml) {
	std::string xml(context_xml);
	XERCES_CPP_NAMESPACE::MemBufInputSource is((XMLByte*) xml.c_str(), 
						   xml.length(), "input");
	Node::Ptr doc = context->parseDocument(is);
	Sequence seq(doc);
	if (!seq.isEmpty() && seq.first()->isNode()) {
	  context->setContextItem(seq.first());
	  context->setContextPosition(1);
	  context->setContextSize(1);
	}
      }
      if (url_resolver) {
	URIResolver * resolver = new CallbackURIResolver(url_resolver, url_resolver_arg);
	context->registerURIResolver(resolver, true);
      }
      
      Result result = query->execute(context);
      
      Item::Ptr item;
      while (item = result->next(context)) {
	// it must keep the u8val instance until copy finished
	std::string stdval(UTF8(item->asString(context)));
	//std::cout << stdval << std::endl;
	char * buf = (char *) malloc(stdval.length() + 1);
	strcpy(buf, stdval.c_str());
	return buf;
      }
    } catch (XQException ex) {
      ;
    }
    return NULL;
    */
  }

  extern int
  execute_all(const char * xquery, const char * context_xml, 
	      void (* callback)(void *, const char *), void * callback_arg,
	      char * (* url_resolver)(void *, const char *), 
	      void * url_resolver_arg)
  {
    try {
      Executor executor(xquery, context_xml, url_resolver, url_resolver_arg);
      return executor.execute_all(callback, callback_arg);
    } catch (XQException ex) {
      ;
    }
    return 0;
    /*
    //std::cout << xquery << std::endl;
    //std::cout << context_xml << std::endl;
    try {
      XQilla xqilla;
      AutoDelete<XQQuery> query(xqilla.parse(X(xquery)));
      AutoDelete<DynamicContext> context(query->createDynamicContext());
      
      if (context_xml) {
	std::string xml(context_xml);
	XERCES_CPP_NAMESPACE::MemBufInputSource is((XMLByte*) xml.c_str(), 
						   xml.length(), "input");
	Node::Ptr doc = context->parseDocument(is);
	Sequence seq(doc);
	if (!seq.isEmpty() && seq.first()->isNode()) {
	  context->setContextItem(seq.first());
	  context->setContextPosition(1);
	  context->setContextSize(1);
	}
      }
      if (url_resolver) {
	URIResolver * resolver = new CallbackURIResolver(url_resolver, url_resolver_arg);
	context->registerURIResolver(resolver, true);
      }
      
      Result result = query->execute(context);
      
      Item::Ptr item;
      while (item = result->next(context)) {
	// it must keep the u8val instance until copy finished
	std::string stdval(UTF8(item->asString(context)));
	callback(callback_arg, stdval.c_str());
      }
      return 1;
    } catch (XQException ex) {
      ;
    }
    return 0;
    */
  }
}
