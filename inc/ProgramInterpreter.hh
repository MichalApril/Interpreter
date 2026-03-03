#ifndef PROGRAMINTERPRETER_HH
#define PROGRAMINTERPRETER_HH

#include "Scene.hh"
#include "Sender.hh"
#include "Set4LibInterface.hh"
#include <Configuration.hh>
#include <string>



class ProgramInterpreter {
  private:
    Scene _Scn;
    Set4LibInterface _LibManager;
    Sender _Sender;
    Configuration _Config;
  public:
    bool ExecProgram(const char* FileName);
    bool ReadConfig(const char* FileName);
    void PrintStatus() const;
    Scene& GetScene() { return _Scn; }
    Set4LibInterface& GetLibManager() { return _LibManager; }
    void InitGraphic();
    void ShowScene();
    void Wait();
};

#endif