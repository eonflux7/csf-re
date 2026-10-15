#pragma once
#include "object.h"
namespace csf_rw {
// Original BSP headers; native owning objects never appear in these fields.
struct Sector {
    int type;
    rw::Triangle* triangles;
    rw::V3d* vertices;
    void* normals;
    rw::TexCoords* texCoords[8];
    rw::RGBA* colors;
    void* resource;
    Link atomics, lights;
    rw::BBox bounds, collisionBounds;
    MeshHeader* mesh;
    void* pipeline;
    uint16_t materialBase, numVertices, numTriangles, pad;
};
struct Plane { int type; float value; Sector *left, *right; float leftValue, rightValue; };
struct World;
struct Intersection {
    union { struct { rw::V3d start,end; } line; rw::V3d point; rw::Sphere sphere; rw::BBox box; Atomic* atomic; };
    uint32_t type;
};
struct CollisionTriangle {
    rw::V3d normal,point;uint32_t index;rw::V3d* vertices[3];
};
static_assert(offsetof(Intersection,type)==24);
static_assert(sizeof(void*)!=4 || sizeof(CollisionTriangle)==40);
using SectorRender = Sector* (__cdecl *)(Sector*);
struct World {
    Object object;
    uint32_t flags;
    int renderOrder;
    MaterialList materials;
    Sector* root;
    uint32_t clumpFrame;
    int numClumps; // original Add/RemoveClump updates +0x24
    Link *currentClump;
    Link clumps, localLights, globalLights;
    rw::V3d origin;
    rw::BBox bounds;
    SectorRender sectorRender;
    Sector* (__cdecl *render)(Sector*,void*);
    void* pipeline;
};
bool worldsEmpty();
bool worldDestroy(World*); // requires detached dynamic members
#if UINTPTR_MAX == 0xffffffff
static_assert(sizeof(World)==116 && offsetof(World,root)==0x1c);
static_assert(offsetof(World,sectorRender)==0x68 && offsetof(World,render)==0x6c);
static_assert(offsetof(World,numClumps)==0x24 && offsetof(World,clumps)==0x2c);
static_assert(sizeof(Sector)==136 && offsetof(Sector,bounds)==0x48);
static_assert(offsetof(Sector,numVertices)==0x82);
static_assert(sizeof(Plane)==24 && offsetof(Plane,left)==8);
#endif
}
