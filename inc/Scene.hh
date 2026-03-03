#ifndef SCENE_HH
#define SCENE_HH
#include "AbstractScene.hh"
#include "AbstractMobileObj.hh"
#include <map>
#include <memory>
#include "Sender.hh" 

class Scene : public AbstractScene{
    private:
    std::map<std::string, std::shared_ptr<AbstractMobileObj>> _Set_MobileObjs;
    public:
    virtual AbstractMobileObj *FindMobileObj(const char * sName) override;
    virtual void AddMobileObj(AbstractMobileObj* pModObj) override;
    void SendScene(Sender &sender) const;

};







#endif