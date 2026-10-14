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
static_assert(sizeof(rw::Matrix) == 64);
static_assert(offsetof(rw::Matrix, flags) == 12);
static_assert(offsetof(rw::Matrix, pos) == 48);
#if UINTPTR_MAX == 0xffffffff
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
