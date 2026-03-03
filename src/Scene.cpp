#include "Scene.hh"
#include "Sender.hh"
#include "Cuboid.hh"
#include <iostream>

using namespace std;

AbstractMobileObj * Scene::FindMobileObj(const char * sName) {
    std::map<std::string, std::shared_ptr<AbstractMobileObj>>::iterator it = _Set_MobileObjs.find(sName);

    if (it == _Set_MobileObjs.end()) {
        return nullptr;
    }

    return it->second.get(); 
}

void Scene::AddMobileObj(AbstractMobileObj* pMobObj) {
    std::shared_ptr<AbstractMobileObj> pPtr(pMobObj);

    std::string name = pPtr->GetName();
    _Set_MobileObjs[name] = pPtr;

}


void Scene::SendScene(Sender &sender) const {
    std::map<std::string, std::shared_ptr<AbstractMobileObj>>::const_iterator it;
    
    for (it = _Set_MobileObjs.begin(); it != _Set_MobileObjs.end(); ++it) {
        auto pCuboid = std::dynamic_pointer_cast<Cuboid>(it->second);
        if (pCuboid) {
            sender.Send(pCuboid->GetInitCmd());
        }
    }
}