#include <iostream>
#include <iomanip>
#include <cmath>      
#include <unistd.h>   
#include <sstream>    
#include "Interp4Move.hh"
#include "AbstractScene.hh"   
#include "AbstractMobileObj.hh" 

using std::cout;
using std::endl;

extern "C" {
  AbstractInterp4Command* CreateCmd(void);
  const char* GetCmdName() { return "Move"; }
}

AbstractInterp4Command* CreateCmd(void) {
  return Interp4Move::CreateCmd();
}

Interp4Move::Interp4Move(): _Speed_mmS(0), _Distance_m(0), _Name("") {}

void Interp4Move::PrintCmd() const {
  cout << GetCmdName() << " " << _Name << " " << _Speed_mmS << " " << _Distance_m << endl;
}

const char* Interp4Move::GetCmdName() const {
  return ::GetCmdName();
}

bool Interp4Move::ReadParams(std::istream& Strm_CmdsList) {
  if (!(Strm_CmdsList >> _Name >> _Speed_mmS >> _Distance_m)) {
      return false; 
  }
  return true;
}

AbstractInterp4Command* Interp4Move::CreateCmd() {
  return new Interp4Move();
}

void Interp4Move::PrintSyntax() const {
  cout << "   Move  NazwaObiektu  Szybkosc[m/s]  DlugoscDrogi[m]" << endl;
}


bool Interp4Move::ExecCmd( AbstractScene &rScn, 
                           const char *sMobObjName,
                           AbstractComChannel &rComChann )
{
  AbstractMobileObj *pObj = rScn.FindMobileObj(_Name.c_str());
  if (!pObj) {
    std::cerr << "Blad: Nie znaleziono obiektu: " << _Name << endl;
    return false;
  }

  double speed_m_s = _Speed_mmS; 
  if (speed_m_s <= 0) return false;

  double time_s = std::abs(_Distance_m / speed_m_s);
  int fps = 50;
  int steps = std::ceil(time_s * fps);
  if (steps == 0) steps = 1;
  int step_us = (1000000 / fps);
  double step_dist = _Distance_m / steps;

  double roll  = pObj->GetAng_Roll_deg() * M_PI / 180.0;
  double pitch = pObj->GetAng_Pitch_deg() * M_PI / 180.0;
  double yaw   = pObj->GetAng_Yaw_deg() * M_PI / 180.0;

  double dx = step_dist * cos(pitch) * cos(yaw);
  double dy = step_dist * cos(pitch) * sin(yaw);
  double dz = step_dist * sin(pitch);

  for (int i = 0; i < steps; ++i) {
      Vector3D pos = pObj->GetPosition_m();
      pos[0] += dx;
      pos[1] += dy;
      pos[2] += dz;
      pObj->SetPosition_m(pos);

     
      std::stringstream cmd;
      cmd << "UpdateObj Name=" << _Name 
          << " Trans_m=(" << pos[0] << "," << pos[1] << "," << pos[2] << ")"
          << " RotXYZ_deg=(" << pObj->GetAng_Roll_deg() << "," 
                             << pObj->GetAng_Pitch_deg() << "," 
                             << pObj->GetAng_Yaw_deg() << ")"
         
          << "\n"; 

      rComChann.LockAccess();
      rComChann.Send(cmd.str());
      rComChann.UnlockAccess();

      usleep(step_us); 
  }

  return true;
}
const std::string & Interp4Move::GetName() const {
    return _Name;
}