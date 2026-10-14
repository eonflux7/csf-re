#pragma once
// Game-visible layout, derived from exported boundary ABI and game accesses.
// No SDK declarations. librw stays unchanged behind this facade.
#include "../../../work/tools/librw/rw.h"
#include <cstddef>

namespace csf_rw {
struct Link { Link *next, *prev; };
struct Object { unsigned char type, subType, flags, privateFlags; void *parent; };
struct Frame {
    Object object;
    Link dirty;
    rw::Matrix matrix, ltm;
    Link objects;
    Frame *child, *next, *root;
};
struct ObjectWithFrame { Object object; Link inFrame; void (__cdecl *sync)(ObjectWithFrame*); };
struct Image {
    int flags, width, height, depth, stride;
    unsigned char *pixels, *palette;
};
struct Raster {
    Raster* parent;
    unsigned char *pixels, *palette;
    int width, height, depth, stride;
    short offsetX, offsetY;
    unsigned char type, flags, privateFlags, format;
    unsigned char* originalPixels;
    int originalWidth, originalHeight, originalStride;
};
struct TexDictionary;
struct Texture {
    Raster* raster;
    TexDictionary* dict;
    Link inDict;
    char name[32], mask[32];
    unsigned int filterAddressing;
    int refCount;
};
struct TexDictionary { Object object; Link textures, inGlobalList; };
struct Material {
    Texture* texture;
    rw::RGBA color;
    void* pipeline;
    rw::SurfaceProperties surfaceProps;
    short refCount, pad;
};
struct Geometry;
struct MorphTarget { Geometry* parent; rw::Sphere boundingSphere; rw::V3d *vertices, *normals; };
struct MaterialList { Material** materials; int numMaterials, space; };
struct Mesh { unsigned short* indices; unsigned int numIndices; Material* material; };
struct MeshHeader { unsigned int flags; unsigned short numMeshes, serialNum; unsigned int totalIndices, firstMeshOffset; };
struct Geometry {
    Object object;
    unsigned int flags;
    unsigned short lockedSinceInst;
    short refCount;
    int numTriangles, numVertices, numMorphTargets, numTexCoordSets;
    MaterialList matList;
    rw::Triangle* triangles;
    rw::RGBA* colors;
    rw::TexCoords* texCoords[8];
    MeshHeader* mesh;
    void* resource;
    MorphTarget* morphTargets;
    unsigned char reserved[40]; // existing slice-2 registry's built-in tail
};
struct Clump;
struct Atomic {
    ObjectWithFrame object;
    void* resource;
    Geometry* geometry;
    rw::Sphere boundingSphere, worldBoundingSphere;
    Clump* clump;
    Link inClump;
    Atomic* (__cdecl *renderCB)(Atomic*);
    void* world;
    struct { unsigned int flags; short start, end; float time, reciprocalTime, position; } interpolator;
    unsigned short renderFrame, pad;
    Link worldSectors;
    void* pipeline;
};
struct Light {
    ObjectWithFrame object;
    float radius;
    rw::RGBAf color;
    float minusCosAngle;
    Link worldSectors;
    Link inWorld;
    unsigned short lightFrame, pad;
};
struct Camera {
    ObjectWithFrame object;
    int projection;
    Camera* (__cdecl *beginUpdate)(Camera*);
    Camera* (__cdecl *endUpdate)(Camera*);
    rw::Matrix viewMatrix;
    Raster *frameBuffer, *zBuffer;
    rw::V2d viewWindow, reciprocalViewWindow, viewOffset;
    float nearPlane, farPlane, fogPlane, zScale, zShift;
    rw::FrustumPlane frustumPlanes[6];
    rw::BBox frustumBoundBox;
    rw::V3d frustumCorners[8];
};
struct Clump { Object object; Link atomics, lights, cameras, inWorld; void* callback; };
static_assert(sizeof(rw::Matrix) == 64);
static_assert(offsetof(rw::Matrix, flags) == 12);
static_assert(offsetof(rw::Matrix, pos) == 48);
#if UINTPTR_MAX == 0xffffffff
static_assert(sizeof(Material) == 28 && offsetof(Material, refCount) == 24);
static_assert(sizeof(MorphTarget) == 28);
static_assert(sizeof(Geometry) == 136 && offsetof(Geometry, refCount) == 14);
static_assert(offsetof(Geometry, matList) == 0x20 && offsetof(Geometry, morphTargets) == 0x5c);
static_assert(sizeof(Atomic) == 116 && offsetof(Atomic, geometry) == 0x18);
static_assert(offsetof(Atomic, renderCB) == 0x48 && offsetof(Atomic, pipeline) == 0x70);
static_assert(sizeof(Light) == 64 && offsetof(Light, color) == 0x18);
static_assert(sizeof(Camera) == 388 && offsetof(Camera, frameBuffer) == 0x60);
static_assert(offsetof(Camera, viewWindow) == 0x68 && offsetof(Camera, frustumPlanes) == 0x94);
static_assert(sizeof(Clump) == 44);
static_assert(sizeof(Image) == 0x1c && offsetof(Image, pixels) == 0x14);
static_assert(sizeof(Raster) == 0x34 && offsetof(Raster, width) == 0xc);
static_assert(offsetof(Raster, format) == 0x23);
static_assert(sizeof(Texture) == 0x58 && offsetof(Texture, refCount) == 0x54);
static_assert(offsetof(Texture, filterAddressing) == 0x50);
static_assert(sizeof(TexDictionary) == 0x18);
static_assert(offsetof(Frame, matrix) == 0x10);
static_assert(offsetof(Frame, ltm) == 0x50);
static_assert(offsetof(Frame, objects) == 0x90);
static_assert(offsetof(Frame, child) == 0x98);
static_assert(sizeof(Frame) == 0xa4);
static_assert(offsetof(rw::Frame, matrix) == 0x18);
#endif
}

// Slice 7 additions: the game writes these value vertices directly (006e3080:
// stride 0x24, colour +0x18; Im2D dispatch callers: reciprocal Z +0x0c).
// They are not owning librw objects. Keep them distinct from backend vertices.
namespace csf_rw {
struct Im2DVertex { float x, y, z, recipZ; uint32_t color; float u, v; };
struct Im3DVertex { rw::V3d position, normal; uint32_t color; float u, v; };
struct VertexStream { void* buffer; uint32_t offset, stride; uint16_t flags; uint8_t managed, dynamicLock; };
static_assert(sizeof(Im2DVertex) == 0x1c);
static_assert(sizeof(Im3DVertex) == 0x24 && offsetof(Im3DVertex, color) == 0x18);
#if UINTPTR_MAX == 0xffffffff
static_assert(sizeof(VertexStream) == 16 && offsetof(VertexStream, stride) == 8);
#endif
}
