#ifndef LIBINTERFACE_HH
#define LIBINTERFACE_HH
#include "AbstractInterp4Command.hh"
#include <string>


class LibInterface{
    friend class Set4LibInterface;
private:
    void* LibHandler;
    std::string CmdName;
    AbstractInterp4Command* (*pCreateInterp)(void);

    
public:
~LibInterface();
AbstractInterp4Command* CreateCmd() const { return pCreateInterp(); }
};


#endif