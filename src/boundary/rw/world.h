#pragma once
#include "object.h"
namespace csf_rw {
// Empty-world startup subset. No BSP streamer or nonempty sector renderer.
struct Sector {
    int type;
    // Empty-sector pointer words stay zero. Rings are only traversable in
    // the x86 game ABI; native tests do not truncate then dereference them.
    uint32_t triangles, vertices, normals, texCoords[8], colors, resource;
    struct { uint32_t next,prev; } atomics, lights;
    rw::BBox bounds, collisionBounds;
    uint32_t mesh, pipeline;
    uint16_t materialBase, numVertices, numTriangles, pad;
};
struct World;
using SectorRender = Sector* (__cdecl *)(Sector*);
struct World {
    Object object;
    uint32_t flags;
    int renderOrder;
    MaterialList materials;
    Sector* root;
    int numClumps;
    uint32_t clumpFrame; // original Create clears +0x24, currentClump is +0x28
    Link *currentClump;
    Link clumps, localLights, globalLights;
    rw::V3d origin;
    rw::BBox bounds;
    SectorRender sectorRender;
    Sector* (__cdecl *render)(Sector*,void*);
    void* pipeline;
};
bool worldsEmpty();
bool worldDestroy(World*); // restricted to tracked, detached empty worlds
#if UINTPTR_MAX == 0xffffffff
static_assert(sizeof(World)==116 && offsetof(World,root)==0x1c);
static_assert(offsetof(World,sectorRender)==0x68 && offsetof(World,render)==0x6c);
static_assert(sizeof(Sector)==136 && offsetof(Sector,bounds)==0x48);
static_assert(offsetof(Sector,numVertices)==0x82);
#endif
}
