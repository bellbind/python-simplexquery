//#include <iostream>
#include <cstdlib>
#include <cstring>
#include <xqilla/xqilla-simple.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>

extern "C" { 
  extern char *
  execute(const char * xquery, const char * context_xml)
  {
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
      
      Result result = query->execute(context);
      
      Item::Ptr item;
      while (item = result->next(context)) {
	// it must keep the u8val instance until copy finished
	//UTF8Str u8val(item->asString(context)); 
	//std::string stdval(u8val.UTF8Form());
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
  }

  extern int
  execute_all(const char * xquery, const char * context_xml, 
	      void (*callback)(void*, const char*), void* callback_arg)
  {
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
      
      Result result = query->execute(context);
      
      Item::Ptr item;
      while (item = result->next(context)) {
	// it must keep the u8val instance until copy finished
	//UTF8Str u8val(item->asString(context)); 
	//callback(callback_arg, u8val.UTF8Form());
	std::string stdval(UTF8(item->asString(context)));
	callback(callback_arg, stdval.c_str());
      }
      return 1;
    } catch (XQException ex) {
      ;
    }
    return 0;
  }
}
