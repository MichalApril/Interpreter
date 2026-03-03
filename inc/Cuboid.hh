#ifndef CUBOID_HH
#define CUBOID_HH
#include "Vector3D.hh"
#include "AbstractMobileObj.hh"
#include <string>


class Cuboid : public AbstractMobileObj{
private:
std::string  _Name;      
Vector3D     _Shift;     
Vector3D     _Scale;    
Vector3D     _Rot;       
Vector3D     _Pos;       
Vector3D     _RGB;       
    
public:
Cuboid();
std::string GetInitCmd() const;
std::string Vec2Str(const Vector3D& v) const;

    void SetName(const char* sName);
    void SetScale(const Vector3D& scale);
    void SetShift(const Vector3D& shift);
    void SetRGB(const Vector3D& rgb);
    void SetRot(const Vector3D& rot);
    virtual double GetAng_Roll_deg() const override  { return _Rot[0]; }
    virtual double GetAng_Pitch_deg() const override { return _Rot[1]; }
    virtual double GetAng_Yaw_deg() const override   { return _Rot[2]; }
    virtual void SetAng_Roll_deg(double ang) override  { _Rot[0] = ang; }
    virtual void SetAng_Pitch_deg(double ang) override { _Rot[1] = ang; }
    virtual void SetAng_Yaw_deg(double ang) override   { _Rot[2] = ang; }
    virtual const Vector3D & GetPosition_m() const override { return _Pos; } 
    virtual void SetPosition_m(const Vector3D &rPos) override { _Pos = rPos; }

    virtual const std::string & GetName() const override { return _Name; }
};


#endif