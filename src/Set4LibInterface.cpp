#include "Set4LibInterface.hh"
#include "AbstractInterp4Command.hh" 
#include <dlfcn.h>
#include <iostream>

typedef AbstractInterp4Command* (*FunPtr_CreateCmd)(void);
typedef const char* (*FunPtr_GetCmdName)(void);

bool Set4LibInterface::AddPlugin(const std::string& LibName) {
    void* pLibHandler = dlopen(LibName.c_str(), RTLD_LAZY);
    if (!pLibHandler) {
        std::cerr << "!!! BŁĄD: Nie można załadować biblioteki: " << LibName
                  << " -- " << dlerror() << std::endl;
        return false;
    }

    FunPtr_CreateCmd pCreateCmd = reinterpret_cast<FunPtr_CreateCmd>(
        dlsym(pLibHandler, "CreateCmd")
    );
    
    if (!pCreateCmd) {
        std::cerr << "Brak funkcji CreateCmd w bibliotece: " << LibName << std::endl;
        dlclose(pLibHandler); 
        return false;
    }

   
    FunPtr_GetCmdName pGetCmdName = reinterpret_cast<FunPtr_GetCmdName>(
        dlsym(pLibHandler, "GetCmdName")
    );
    
    if (!pGetCmdName) {
        std::cerr << "Brak funkcji GetCmdName w bibliotece: " << LibName << std::endl;
        dlclose(pLibHandler);
        return false;
    }
    std::shared_ptr<LibInterface> pNewPlugin = std::make_shared<LibInterface>();

    pNewPlugin->LibHandler = pLibHandler;
    pNewPlugin->pCreateInterp = pCreateCmd; 
    pNewPlugin->CmdName = pGetCmdName();    
    _pluginMap[pNewPlugin->CmdName] = pNewPlugin;

    std::cout << "  Załadowano wtyczkę: " << LibName 
              << " ==> Polecenie: " << pNewPlugin->CmdName << std::endl;

    return true;
}

std::shared_ptr<LibInterface> Set4LibInterface::FindPlugin(const std::string& CmdName) {
    auto iter = _pluginMap.find(CmdName);
    
    if (iter == _pluginMap.end()) {
        return nullptr;
        } 
    
    return iter->second;
}