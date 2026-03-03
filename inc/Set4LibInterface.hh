#ifndef SET4LIBINTERFACE_HH
#define SET4LIBINTERFACE_HH

#include <string>
#include <map>     
#include <memory>   
#include "LibInterface.hh"


class Set4LibInterface {
public:
    bool AddPlugin(const std::string& LibName);
    std::shared_ptr<LibInterface> FindPlugin(const std::string& CmdName);


private:
    std::map<std::string, std::shared_ptr<LibInterface>> _pluginMap;
};

#endif 