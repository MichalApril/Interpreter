#include "Cuboid.hh"
#include <iostream>
#include <sstream>
#include <string>
#include <locale>

using namespace std;

Cuboid::Cuboid() {
    _Scale[0] = 1; _Scale[1] = 1; _Scale[2] = 1;
    _RGB[0] = 128; _RGB[1] = 128; _RGB[2] = 128;
}
void Cuboid::SetName(const char* sName) { _Name = sName; }
void Cuboid::SetScale(const Vector3D& scale) { _Scale = scale; }
void Cuboid::SetShift(const Vector3D& shift) { _Shift = shift; }
void Cuboid::SetRGB(const Vector3D& rgb) { _RGB = rgb; }
void Cuboid::SetRot(const Vector3D& rot) { _Rot = rot; }

std::string Cuboid::Vec2Str(const Vector3D& v) const{
    
    std::ostringstream strm;
    strm.imbue(std::locale::classic());
    strm << "(" << v[0] << "," << v[1] << "," << v[2] << ")";
    return strm.str();
}

std::string Cuboid::GetInitCmd() const {
    std::ostringstream strm;
    
    
    strm << "AddObj Name=" << _Name 
         << " RGB=" << Vec2Str(_RGB)
         << " Scale=" << Vec2Str(_Scale)
         << " Shift=" << Vec2Str(_Shift)
         << " RotXYZ_deg=" << Vec2Str(_Rot)
         << " Trans_m=" << Vec2Str(_Pos)
         << "\n";

    return strm.str();
}