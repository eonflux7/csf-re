#pragma once
#include "abi.h"
#include <cstdint>

namespace csf_rw {
// These callbacks use the same C signatures as librw, but the registry belongs
// to ABI objects, NEVER to native owning objects. Later slices must use it.
enum class Registry { Engine, Frame, Atomic, Geometry, Material, Light, Raster, Texture, Dictionary, Clump, Camera };
rw::PluginList& plugins(Registry);
bool constructPlugins(Registry, void*);
void destructPlugins(Registry, void*);
bool framesEmpty();

struct MemoryFunctions {
    void* (__cdecl *malloc)(uint32_t, uint32_t);
    void (__cdecl *free)(void*);
    void* (__cdecl *realloc)(void*, uint32_t, uint32_t);
    void* (__cdecl *calloc)(uint32_t, uint32_t, uint32_t);
};
struct Memory { uint8_t* start; uint32_t length; };
// No game caller in this slice dereferences the stream handle. It remains a
// private facade identity, never a pointer to the virtual librw Stream class.
struct Stream;
rw::Stream* streamNative(Stream*);
bool streamPrepareWrite(Stream*, uint32_t);
struct ChunkHeader { uint32_t type, length, version, build, isComplex; };
static_assert(sizeof(ChunkHeader) == 20);
}
