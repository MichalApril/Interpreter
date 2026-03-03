#ifndef XMLINTERP4ACTIONS_HH
#define XMLINTERP4ACTIONS_HH

#include <string>
#include <xercesc/util/XMLString.hpp>

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax/Locator.hpp>

#include <xercesc/sax2/Attributes.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
  


#include "Configuration.hh"



class XMLInterp4Config : public xercesc::DefaultHandler {
  public:

  XMLInterp4Config(Configuration &rConfig);


    virtual void startDocument() override;

    virtual void endDocument() override;

  
    virtual void startElement(
                    const XMLCh *const               pURI, 
                    const XMLCh *const               pLocalName, 
                    const XMLCh *const               pQNname,
                    const   xercesc::Attributes&     rAttrs
                  ) override;


    virtual  void endElement(
                    const XMLCh *const pURI, 
                    const XMLCh *const pLocalName, 
                    const XMLCh *const pQNname
                   ) override;
   
    void fatalError(const xercesc::SAXParseException&);
   
    void error(const xercesc::SAXParseException &);
 
    void warning(const xercesc::SAXParseException &);


    void WhenStartElement( const std::string&             rElemName,
		           const xercesc::Attributes&     rAttrs
                         );
    void ProcessLibAttrs(const xercesc::Attributes&   rAttrs);
    void ProcessCubeAttrs(const xercesc::Attributes&   rAttrs); 
    void ProcessCommandAttrs(const xercesc::Attributes &rAttrs);
    static std::string GetAttributeValue(const xercesc::Attributes &rAttrs, const char* name);

                    
  private:
  Configuration & _rConfig;
};

#endif
