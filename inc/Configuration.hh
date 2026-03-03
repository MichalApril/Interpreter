#ifndef CONFIGURATION_HH
#define CONFIGURATION_HH

#include "Scene.hh"
#include "Set4LibInterface.hh"
#include "Sender.hh" 
#include <thread>
#include <list>


class Configuration {
    Scene* _pScene = nullptr;
    Set4LibInterface _LibMgr;
    Sender* _pSender = nullptr;
    std::list<std::thread> _Threads;
public:
    
    Configuration() {}
    Scene& GetScene() { return *_pScene; }
    Set4LibInterface& GetLibManager() { return _LibMgr; }
    void SetSender(Sender* pSender) { _pSender = pSender; }
    Sender* GetSender() const { return _pSender; }
    void SetScene(Scene* pScene) { 
        _pScene = pScene; 
    }
    void AddThread(std::thread&& t) {
        _Threads.push_back(std::move(t));
    }

    void JoinAllThreads() {
        for (std::list<std::thread>::iterator it = _Threads.begin(); it != _Threads.end(); ++it) {
            if (it->joinable()) {
                it->join();
            }
        }
    }


};

#endif