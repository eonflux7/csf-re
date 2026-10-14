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
struct ToolkitChildren { ToolkitObject* objects; uint32_t count; };
struct ToolkitObject {
    rw::Matrix matrix;
    // Type-dependent +40..+4f payload: vector, children pointer, render callback.
    unsigned char payload[16];
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
#if UINTPTR_MAX == 0xffffffff
static_assert(sizeof(ToolkitObject) == 96);
static_assert(offsetof(ToolkitObject, descriptor) == 92);
static_assert(offsetof(ToolkitDescriptor, flags) == 64);
static_assert(sizeof(ToolkitHandle) == 32);
#endif
}
