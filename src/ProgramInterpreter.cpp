#include "ProgramInterpreter.hh"
#include "xmlinterp.hh"
#include "Configuration.hh"
#include "AbstractInterp4Command.hh"
#include <iostream>
#include <fstream>
#include <limits> 
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/util/XMLString.hpp>

using namespace std;
using namespace xercesc;

void ProgramInterpreter::Wait() {
    _Config.JoinAllThreads();
}

bool ProgramInterpreter::ReadConfig(const char* FileName) 
{
    try {
        XMLPlatformUtils::Initialize();
    }
    catch (const XMLException& toCatch) {
        return false;
    }

    SAX2XMLReader* parser = XMLReaderFactory::createXMLReader();
    parser->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);
    parser->setFeature(XMLUni::fgSAX2CoreValidation, false);
    parser->setFeature(XMLUni::fgXercesDynamic, false);
    parser->setFeature(XMLUni::fgXercesSchema, false);
    parser->setFeature(XMLUni::fgXercesSchemaFullChecking, false);

    _Config.SetSender(&_Sender);
    _Config.SetScene(&_Scn);

    XMLInterp4Config Handler(_Config);
    
    parser->setContentHandler(&Handler);
    parser->setErrorHandler(&Handler);

    bool result = true;
    try {
        parser->parse(FileName);
    }
    catch (...) {
        result = false;
    }

    delete parser;
    XMLPlatformUtils::Terminate();
    
    return result;
}
void ProgramInterpreter::PrintStatus() const {
    
    cout << "Status programu" << endl;
    
}
void ProgramInterpreter::ShowScene() {
    _Scn.SendScene(_Sender);
}
bool ProgramInterpreter::ExecProgram(const char* FileName) {
    ifstream inputFile(FileName);
    
    if (!inputFile.is_open()) {
        cerr << "Nie można otworzyć pliku komend: " << FileName << endl;
        return false;
    }

    string cmdName;

    while (inputFile >> cmdName) {
        cout << "Wczytano polecenie: " << cmdName << endl;
        
        std::shared_ptr<LibInterface> plugin = _LibManager.FindPlugin(cmdName);

        if (plugin != nullptr) {
            AbstractInterp4Command *pCmd = plugin->CreateCmd();

            if (pCmd != nullptr) {
                if (pCmd->ReadParams(inputFile)) {
                    cout << "Parametry: ";
                    pCmd->PrintCmd(); 
                    cout << endl;

                    
                    bool status = pCmd->ExecCmd(_Scn, pCmd->GetName().c_str(), _Sender);
                    
                    if (!status) {
                        cerr << "Blad podczas wykonywania komendy dla obiektu: " 
                             << pCmd->GetName() << endl;
                    }

                } else {
                    cerr << "Niewlasciwe parametry dla polecenia: " << cmdName << endl;
                    inputFile.clear(); 
                    inputFile.ignore(numeric_limits<streamsize>::max(), '\n');
                }

                delete pCmd;
            } else {
                cerr << "Nie udało się utworzyć obiektu komendy." << endl;
            }
        } else {
            cerr << "Nieznana komenda: " << cmdName << endl;
            inputFile.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return true;
}
void ProgramInterpreter::InitGraphic() {
    try {
        _Sender.OpenConnection();
        _Sender.Send("Clear\n");
        _Scn.SendScene(_Sender);
    }
    catch (const std::runtime_error& e) 
{        std::cerr << "Ostrzeżenie: " << e.what() << std::endl;
        std::cerr << "Program nie zadziała graficznie" << std::endl;
        
    }
}

