#include <iostream>
#include <iomanip>
#include <cmath>
#include <unistd.h> 
#include <sstream>  
#include "Interp4Set.hh"
#include "AbstractScene.hh"
#include "AbstractMobileObj.hh"

using std::cout;
using std::endl;

extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Set"; }
}


AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Set::CreateCmd();
}


Interp4Set::Interp4Set(): 
  _Name(""), 
  _X(0), _Y(0), _Z(0), 
  _AngX(0), _AngY(0), _AngZ(0)
{}


void Interp4Set::PrintCmd() const
{
  cout << GetCmdName() << " " << _Name << " " 
       << _X << " " << _Y << " " << _Z << " " 
       << _AngX << " " << _AngY << " " << _AngZ << endl;
}


const char* Interp4Set::GetCmdName() const
{
  return ::GetCmdName();
}


const std::string & Interp4Set::GetName() const
{
  return _Name;
}


bool Interp4Set::ExecCmd( AbstractScene      &rScn, 
                          const char         *sMobObjName,
                          AbstractComChannel &rComChann )
{
  AbstractMobileObj *pObj = rScn.FindMobileObj(_Name.c_str());
  
  if (!pObj) {
      std::cerr << "Błąd: Nie znaleziono obiektu do ustawienia: " << _Name << std::endl;
      return false;
  }

  Vector3D newPos;
  newPos[0] = _X;
  newPos[1] = _Y;
  newPos[2] = _Z;
  
  pObj->SetPosition_m(newPos);
  pObj->SetAng_Roll_deg(_AngX);
  pObj->SetAng_Pitch_deg(_AngY);
  pObj->SetAng_Yaw_deg(_AngZ);

  
  std::stringstream cmd;
  cmd << "UpdateObj Name=" << _Name 
      << " Trans_m=(" << _X << "," << _Y << "," << _Z << ")"
      << " RotXYZ_deg=(" << _AngX << "," << _AngY << "," << _AngZ << ")"
      << "\n";

  rComChann.LockAccess();
  std::cout << cmd.str() << std::flush; 
  rComChann.Send(cmd.str());
  rComChann.UnlockAccess();
  usleep(20000); 

  return true;
}


bool Interp4Set::ReadParams(std::istream& Strm_CmdsList)
{
  
  if (!(Strm_CmdsList >> _Name >> _X >> _Y >> _Z >> _AngX >> _AngY >> _AngZ)) {
      std::cerr << "Błąd wczytywania parametrów dla Set" << std::endl;
      return false; 
  }
  return true;
}

/*!
 *
 */
AbstractInterp4Command* Interp4Set::CreateCmd()
{
  return new Interp4Set();
}

/*!
 *
 */
void Interp4Set::PrintSyntax() const
{
  cout << "   Set  NazwaObiektu  X[m]  Y[m]  Z[m]  KatOX[deg]  KatOY[deg]  KatOZ[deg]" << endl;
}