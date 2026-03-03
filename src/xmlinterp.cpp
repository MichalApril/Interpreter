#include <xercesc/util/PlatformUtils.hpp>
#include "xmlinterp.hh"
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include "Cuboid.hh"



using namespace std;



XMLInterp4Config::XMLInterp4Config(Configuration &rConfig) : _rConfig(rConfig)
{
}


void XMLInterp4Config::startDocument()
{
  cout << "*** Rozpoczecie przetwarzania dokumentu XML." << endl;
}


void XMLInterp4Config::endDocument()
{
  cout << "=== Koniec przetwarzania dokumentu XML." << endl;
}

void XMLInterp4Config::ProcessLibAttrs(const xercesc::Attributes  &rAttrs)
{
 if (rAttrs.getLength() != 1) {
      cerr << "Zla ilosc atrybutow dla \"Lib\"" << endl;
      exit(1);
 }

 char* sParamName = xercesc::XMLString::transcode(rAttrs.getQName(0));

 if (strcmp(sParamName,"Name")) {
      cerr << "Zla nazwa atrybutu dla Lib" << endl;
      exit(1);
 }         

 XMLSize_t  Size = 0;
 char* sLibName = xercesc::XMLString::transcode(rAttrs.getValue(Size));

 cout << "  Nazwa biblioteki: " << sLibName << endl;

if (!_rConfig.GetLibManager().AddPlugin(sLibName)) {
      cerr << "!!! Blad: Nie udalo sie zaladowac wtyczki: " << sLibName << endl;  
    }

 xercesc::XMLString::release(&sParamName);
 xercesc::XMLString::release(&sLibName);
}

void XMLInterp4Config::ProcessCommandAttrs(const xercesc::Attributes &rAttrs)
{
    std::string cmdName = GetAttributeValue(rAttrs, "Name");
    if (cmdName.empty()) {
        std::cerr << "Blad: Komenda bez nazwy!" << std::endl;
        return;
    }

    std::string params = GetAttributeValue(rAttrs, "Params");
    
    std::shared_ptr<LibInterface> plugin = _rConfig.GetLibManager().FindPlugin(cmdName);
    if (!plugin) {
        std::cerr << "Blad: Nieznana komenda: " << cmdName << std::endl;
        return;
    }

    AbstractInterp4Command *pCmd = plugin->CreateCmd();
    if (!pCmd) return;

    std::stringstream ss(params);
    if (pCmd->ReadParams(ss)) {
        Sender* pSender = _rConfig.GetSender();
        if (pSender) {
            Scene& rScene = _rConfig.GetScene();
            
            std::thread t([pCmd, &rScene, pSender]() {
                pCmd->ExecCmd(rScene, pCmd->GetName().c_str(), *pSender);
                delete pCmd;
            });
            
            _rConfig.AddThread(std::move(t));
        } else {
            delete pCmd; 
        }
    } else {
        std::cerr << "Blad parsowania parametrow dla: " << cmdName << std::endl;
        delete pCmd;
    }
}
std::string XMLInterp4Config::GetAttributeValue(const xercesc::Attributes &rAttrs, const char* name)
{
    XMLCh* xmlName = xercesc::XMLString::transcode(name);
    const XMLCh* xmlValue = rAttrs.getValue(xmlName);
    xercesc::XMLString::release(&xmlName);

    if (!xmlValue) return "";

    char* cValue = xercesc::XMLString::transcode(xmlValue);
    std::string sValue = cValue;
    xercesc::XMLString::release(&cValue);
    return sValue;
}

static void ParseVector(const std::string& text, Vector3D& vec)
{
    if (text.empty()) return;
    std::istringstream IStrm(text);
    double x, y, z;

    IStrm >> x >> y >> z;
    if (!IStrm.fail()) {
        vec[0] = x;
        vec[1] = y;
        vec[2] = z;
    }
}

void XMLInterp4Config::ProcessCubeAttrs(const xercesc::Attributes  &rAttrs)
{
 if (rAttrs.getLength() < 1) {
      cerr << "Zla ilosc atrybutow dla \"Cuboid\"" << endl;
      exit(1);
 }

 Cuboid *pObj = new Cuboid();

 std::string sName = GetAttributeValue(rAttrs, "Name");
 if (sName.empty()) {
     cerr << "Brak nazwy dla obiektu Cuboid!" << endl;
     delete pObj;
     return;
 }
 pObj->SetName(sName.c_str());

 Vector3D scale;
 ParseVector(GetAttributeValue(rAttrs, "Scale"), scale);
 pObj->SetScale(scale);

 Vector3D shift;
 ParseVector(GetAttributeValue(rAttrs, "Shift"), shift);
 pObj->SetShift(shift);

 Vector3D rot;
 ParseVector(GetAttributeValue(rAttrs, "RotXYZ_deg"), rot);
 pObj->SetRot(rot);

 Vector3D trans;
 ParseVector(GetAttributeValue(rAttrs, "Trans_m"), trans);
 pObj->SetPosition_m(trans);

 Vector3D rgb;
 ParseVector(GetAttributeValue(rAttrs, "RGB"), rgb);
 pObj->SetRGB(rgb);

 _rConfig.GetScene().AddMobileObj(pObj);

 Sender* pSender = _rConfig.GetSender();
 if (pSender != nullptr) {
     pSender->Send(pObj->GetInitCmd());
 }
 

 cout << "Utworzono i wysłano obiekt: " << sName << endl;
}


void XMLInterp4Config::WhenStartElement( const std::string &rElemName,
                                         const xercesc::Attributes &rAttrs)
{
  if (rElemName == "Lib") {
    ProcessLibAttrs(rAttrs);   return;   
  }

  if (rElemName == "Cube") {
    ProcessCubeAttrs(rAttrs);  return;
  }

  if (rElemName == "Command") {
    ProcessCommandAttrs(rAttrs); return;
  }
}



void XMLInterp4Config::startElement(  const   XMLCh* const            pURI,
                                      const   XMLCh* const            pLocalName,
                                      const   XMLCh* const            pQName,
				      const   xercesc::Attributes&    rAttrs
                                    )
{
  char* sElemName = xercesc::XMLString::transcode(pLocalName);
  cout << "+++ Poczatek elementu: "<< sElemName << endl;

  WhenStartElement(sElemName, rAttrs);

  xercesc::XMLString::release(&sElemName);
}




void XMLInterp4Config::endElement(  const   XMLCh* const    pURI,
                                    const   XMLCh* const    pLocalName,
                                    const   XMLCh* const    pQName
                                 )
{
   char* sURI =  xercesc::XMLString::transcode(pURI);
   char* sElemName = xercesc::XMLString::transcode(pLocalName);
   char* sQName =  xercesc::XMLString::transcode(pQName);
   cout << "   URI: " << sURI << endl;
   cout << " QName: " << sQName << endl;
   cout << "----- Koniec elementu: "<< sElemName << endl;

   xercesc::XMLString::release(&sElemName);
}



void XMLInterp4Config::fatalError(const xercesc::SAXParseException&  rException)
{
   char* sMessage = xercesc::XMLString::transcode(rException.getMessage());
   char* sSystemId = xercesc::XMLString::transcode(rException.getSystemId());

   cerr << "Blad fatalny! " << endl
        << "    Plik:  " << sSystemId << endl
        << "   Linia: " << rException.getLineNumber() << endl
        << " Kolumna: " << rException.getColumnNumber() << endl
        << " Informacja: " << sMessage 
        << endl;

   xercesc::XMLString::release(&sMessage);
   xercesc::XMLString::release(&sSystemId);
}

void XMLInterp4Config::error(const xercesc::SAXParseException&  rException)
{
  cerr << "Blad ..." << endl;

}



void XMLInterp4Config::warning(const xercesc::SAXParseException&  rException)  
{
  cerr << "Ostrzezenie ..." << endl;

 
}
