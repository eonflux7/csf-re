#pragma once
#include "abi.h"
namespace csf_rw {
// Game-side 7C2110/7C2640 and 6EBA90 read this ABI directly. Nodes contain
// two eight-byte child records: flags, byte count (255 = branch), u16 index,
// float split bound. Pointers widen only in native boundary tests.
struct CollisionTree {
    uint32_t flags;
    rw::BBox bounds;
    uint16_t numEntries, numNodes;
    uint8_t* nodes;
    uint16_t* remap;
};
static_assert(offsetof(CollisionTree, numEntries)==0x1c);
static_assert(sizeof(void*)!=4 || (sizeof(CollisionTree)==40 &&
              offsetof(CollisionTree,nodes)==0x20 && offsetof(CollisionTree,remap)==0x24));
}
