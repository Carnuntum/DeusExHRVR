#pragma once
#include <cmath>
#include <cstdint>
#include <cstring>
namespace NativeBounds {
// Replace a controller-held gun's cull volume with a sphere around where it is
// drawn. The native volume sits where the flat game holds the gun, in front of
// the game camera, so it leaves the view when the head turns away from it.
inline bool ExpandWeapon(void* volume,const float gun[3],float reach) {
    // DXHRDC 2.0.66.0: union occupies 0x50 bytes, followed by its type.
    // Type 10 (Everything) is never culled, and SceneCellContainer's volume
    // callbacks do not support it: leave it alone. Every other type, including
    // the ones whose layout is unknown, becomes a sphere (type 0).
    uint32_t type;std::memcpy(&type,static_cast<char*>(volume)+0x50,4);
    if(type==10)return false;
    if(!std::isfinite(gun[0])||!std::isfinite(gun[1])||!std::isfinite(gun[2])||!std::isfinite(reach)||reach<=0)return false;
    float sphere[4]{gun[0],gun[1],gun[2],reach};type=0;
    std::memcpy(volume,sphere,sizeof(sphere));std::memcpy(static_cast<char*>(volume)+0x50,&type,4);
    return true;
}
}
