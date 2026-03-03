#include <iostream>
#include <iomanip>
#include <cmath>
#include <unistd.h> 
#include <sstream>  
#include "Interp4Rotate.hh"
#include "AbstractScene.hh"
#include "AbstractMobileObj.hh"

using std::cout;
using std::endl;

extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Rotate"; }
}

\
AbstractInterp4Command* CreateCmd(void)
{
  return Interp4Rotate::CreateCmd();
}


Interp4Rotate::Interp4Rotate(): 
    _Name(""), _Axis("OZ"), _RotSpeed_degS(0), _TargetAngle_deg(0)
{}


void Interp4Rotate::PrintCmd() const
{
  cout << GetCmdName() << " " << _Name << " " 
       << _Axis << " " << _RotSpeed_degS << " " << _TargetAngle_deg << endl;
}


const char* Interp4Rotate::GetCmdName() const
{
  return ::GetCmdName();
}

const std::string & Interp4Rotate::GetName() const
{
  return _Name; 
}


bool Interp4Rotate::ExecCmd( AbstractScene      &rScn, 
                             const char         *sMobObjName,
                             AbstractComChannel &rComChann )
{
  AbstractMobileObj *pObj = rScn.FindMobileObj(_Name.c_str());
  
  if (!pObj) {
      std::cerr << "Błąd: Nie znaleziono obiektu: " << _Name << std::endl;
      return false;
  }

  const int FPS = 50;                     
  const int USEC_PER_FRAME = 1000000 / FPS;

  double speed_abs = std::abs(_RotSpeed_degS);
  if (speed_abs < 0.001) speed_abs = 0.001;

  double total_time_s = std::abs(_TargetAngle_deg / speed_abs);
  

  int steps = static_cast<int>(total_time_s * FPS);
  if (steps < 1) steps = 1;

  double angle_step_deg = _TargetAngle_deg / steps;

  for (int i = 0; i < steps; ++i) {
      
      if (_Axis == "OX") {
          pObj->SetAng_Roll_deg(pObj->GetAng_Roll_deg() + angle_step_deg);
      } 
      else if (_Axis == "OY") {
          pObj->SetAng_Pitch_deg(pObj->GetAng_Pitch_deg() + angle_step_deg);
      } 
      else { 
          pObj->SetAng_Yaw_deg(pObj->GetAng_Yaw_deg() + angle_step_deg);
      }

     
      std::stringstream ss;
      ss << "UpdateObj Name=" << _Name 
         << " RotXYZ_deg=(" << pObj->GetAng_Roll_deg() << "," 
                            << pObj->GetAng_Pitch_deg() << "," 
                            << pObj->GetAng_Yaw_deg() << ")"
         << " Trans_m=("    << pObj->GetPosition_m()[0] << ","
                            << pObj->GetPosition_m()[1] << ","
                            << pObj->GetPosition_m()[2] << ")\n";

      rComChann.LockAccess();
      
      std::cout << ss.str() << std::flush; 
      rComChann.Send(ss.str());
      rComChann.UnlockAccess();
      
      usleep(USEC_PER_FRAME); 
  }

  return true;
}
bool Interp4Rotate::ReadParams(std::istream& Strm_CmdsList)
{
  if (!(Strm_CmdsList >> _Name >> _Axis >> _RotSpeed_degS >> _TargetAngle_deg)) {
      std::cerr << "Błąd wczytywania parametrów dla Rotate" << std::endl;
      return false; 
  }
  return true;
}


AbstractInterp4Command* Interp4Rotate::CreateCmd()
{
  return new Interp4Rotate();
}


void Interp4Rotate::PrintSyntax() const
{
  cout << "   Rotate  NazwaObiektu  Oś[OZ/OY/OX]  Szybkosc[deg/s]  KatObrotu[deg]" << endl;
}