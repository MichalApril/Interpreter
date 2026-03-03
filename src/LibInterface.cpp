#include "LibInterface.hh"
#include <dlfcn.h>


LibInterface::~LibInterface()
{
    if (LibHandler) {
        dlclose(LibHandler); 
    }
}
