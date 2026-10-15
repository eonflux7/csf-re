#pragma once
#include "abi.h"
#include <cstdint>

namespace csf_rw {
// Independent game-facing toolkit records. These are NOT librw objects.
struct ToolkitObject;
struct ToolkitDescriptor {
    uint32_t type;
    void* methods[15]; // destroy at +4; transform/output dispatch at +10
    uint32_t flags;
};
struct ToolkitChildren { ToolkitObject* objects; uintptr_t count; };
struct ToolkitObject {
    rw::Matrix matrix;
    // Type-dependent +40..+4f payload: vector, children pointer, render callback.
    unsigned char payload[3*sizeof(void*)+4];
    uint32_t multiplier, offset;
    uint16_t control0, control1;
    ToolkitDescriptor* descriptor;
};
struct ToolkitHandle {
    void* workspace;
    void* configuration;
    void* entries;
    uint32_t cursor, count;
    void* scratch;
    uint32_t nodeCount;
    void* nodes;
};
// Physics objects, not RpTeam players. Offsets are from the executable and
// callers 006ff9f0/0070a460. Pointer fields expand for native boundary tests.
struct PhysicsDictionary;
struct PhysicsBody {
    void** methods;
    uint32_t user, flags;
    uint16_t partCount, pad0;
    void* parts;
    void* reserved[3];
    uint16_t id, pad1;
    PhysicsDictionary* owner;
    void* reserved28;
    void* links;
};
struct PhysicsDictionary {
    void* spatial;
    uint32_t wordCount;
    uint32_t reserved[15];
    uint32_t capacity, activeLimit, activeCount;
    PhysicsBody** bodies;
    void* reserved54;
    uint32_t *occupied, *enabled, *active, *requested, *bounded;
};
struct PhysicsState {
    rw::Matrix* matrices;
    void* reserved04;
    void* reserved08;
    void* reserved0c;
    rw::Quat* rotations;
    void* reserved14[7];
    float* forces; // pairs of padded V3d: force at +0, torque at +16
};
struct PhysicsAggregate { void* reserved[4]; PhysicsState* state; };
struct PhysicsElement {
    PhysicsAggregate* aggregate;
    uint32_t index;
    rw::V3d translation;
};
struct PhysicsPart { uint32_t unused; PhysicsElement* element; rw::Matrix* matrix; };
struct PhysicsMass {
    ToolkitObject* shape;
    rw::V3d center;
    float mass;
    rw::V3d inertia;
    rw::Quat rotation;
    float sphericalInertia;
    uint32_t reserved[5];
    uint32_t flags;
};
struct PhysicsShapeProperties { rw::V3d inertia; rw::Quat rotation; };
// Spatial tree storage is opaque except for the bounds table and these tail
// fields consumed by the query/filter adapters. Not an owning librw object.
struct PhysicsSpatial {
    unsigned char storage[49172];
    float margin;
    void* collisionMatrix;
};
#if UINTPTR_MAX == 0xffffffff
static_assert(sizeof(ToolkitObject) == 96);
static_assert(offsetof(ToolkitObject, descriptor) == 92);
static_assert(offsetof(ToolkitDescriptor, flags) == 64);
static_assert(sizeof(ToolkitHandle) == 32);
static_assert(offsetof(PhysicsBody,id)==32 && offsetof(PhysicsBody,owner)==36);
static_assert(offsetof(PhysicsDictionary,bounded)==104);
static_assert(offsetof(PhysicsState,forces)==48);
static_assert(sizeof(PhysicsPart)==12);
static_assert(offsetof(PhysicsMass,flags)==72);
static_assert(offsetof(PhysicsSpatial,collisionMatrix)==49176);
#endif
}
