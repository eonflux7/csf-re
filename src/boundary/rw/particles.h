#pragma once
#include "object.h"
#include <cstdint>

namespace csf_rw {
// Binary-derived, not SDK declarations. Numeric property sizes/offsets remain
// the x86 values even in native fixtures; particleProperty widens pointer-bearing
// emitter properties there. The production x86 layout needs no translation.
struct ParticlePropTable {
    void* allocation;
    ParticlePropTable* next;
    uint32_t id;
    int32_t refCount, numProperties, reserved;
    int32_t *ids, *offsets, *sizes, *unroundedSizes;
};
struct ParticleClass {
    ParticleClass* next;
    uint32_t id;
    int32_t refCount, objectSize;
    ParticlePropTable* properties;
    int32_t numCallbackSets;
    uintptr_t* callbacks;
};
struct ParticleBatch {
    ParticleBatch* next;
    ParticleClass* particleClass;
    int32_t numParticles, maxParticles, newParticle, dataOffset;
    uint32_t reserved[2];
};
struct ParticleEmitter {
    ParticleEmitter* next;
    uint32_t flags;
    Atomic* atomic;
    ParticleClass* emitterClass;
    int32_t numParticles, batchSize;
    ParticleClass* particleClass;
    ParticleBatch *inactiveBatches, *activeBatches;
    float boundingRadiusSquared; // +24h, accumulated by 57CFE0
};
struct EmitterStandard {
    uint32_t seed;
    int32_t maxParticles;
    float currentTime, previousTime;
    rw::V3d force, position, positionBias;
    float emitGap, emitGapBias, previousEmit, nextEmit;
    int32_t emitCount, emitCountBias;
    float speed, speedBias, life, lifeBias;
    rw::V3d direction, directionBias;
    rw::V2d size;
    rw::RGBA color;
    float rotation;
    rw::V2d uv0, uv1;
    uint32_t reserved[4];
    Texture* texture;
    rw::Matrix* ltm;
};
struct EmitterTank {
    uint32_t dataFlags, updateFlags;
    int32_t numParticles, maxParticles, flags, platformFlags;
    Atomic* atomic;
    unsigned char **input, **output;
    int32_t* strides;
    int32_t srcBlend, dstBlend, vertexAlpha;
};
struct TankLock { void* data; int32_t stride; };
struct TankFormat {
    int32_t numClusters;
    TankLock position, matrix, normal, size, color, texCoords, rotation, colors, texCoords4;
    uint32_t reserved;
    int32_t clusterCount, stride;
    uint32_t flags;
};
// The lock adapter accepts this prefix even for a tank constructed by a later
// backend. It does not cast the atomic or its extension to a librw object.
struct Tank {
    int32_t maxParticles, numParticles;
    uint32_t reserved;
    void* allocation;
    uintptr_t render;
    uintptr_t callbacks[4];
    uint32_t reserved2[6];
    uint32_t locked, dirty, platformFlags;
    TankFormat format;
    uint32_t tail[37];
};
using EmitterCallback = ParticleEmitter* (__cdecl *)(Atomic*, ParticleEmitter*, void*);
using BatchCallback = ParticleBatch* (__cdecl *)(ParticleEmitter*, ParticleBatch*, void*);
void* particleProperty(ParticleEmitter*, int32_t id);
ParticleEmitter* particleEmitterCreate(ParticleClass*, void*);
int particleEmitterDestroy(ParticleEmitter*);
bool particlesEmpty();
void particleBoundaryTerm();
void* particleAtomicCtor(void*, int, int);
void* particleAtomicDtor(void*, int, int);
void* particleAtomicCopy(void*, void*, int, int);
#if UINTPTR_MAX == 0xffffffff
static_assert(sizeof(ParticlePropTable)==0x28 && offsetof(ParticlePropTable, ids)==0x18);
static_assert(offsetof(ParticlePropTable, sizes)==0x20);
static_assert(sizeof(ParticleClass)==0x1c && offsetof(ParticleClass, properties)==0x10);
static_assert(sizeof(ParticleBatch)==0x20 && offsetof(ParticleBatch, dataOffset)==0x14);
static_assert(sizeof(ParticleEmitter)==0x28 && offsetof(ParticleEmitter, activeBatches)==0x20);
static_assert(offsetof(ParticleEmitter, boundingRadiusSquared)==0x24);
static_assert(sizeof(EmitterStandard)==0xac && offsetof(EmitterStandard, texture)==0xa4);
static_assert(sizeof(EmitterTank)==0x34 && offsetof(EmitterTank, atomic)==0x18);
static_assert(sizeof(TankFormat)==0x5c && offsetof(TankFormat, flags)==0x58);
static_assert(sizeof(Tank)==0x138 && offsetof(Tank, locked)==0x3c);
static_assert(offsetof(Tank, format)==0x48);
#endif
}
