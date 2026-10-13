#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 782, stride 32.
using Value = legacy::vc71::element<32>;
static uint32_t __cdecl Replica_006fa150(uint32_t begin, uint32_t end, uint32_t dest) {
    return legacy::vc71::vector_copy_backward<32>(begin, end, dest);
}

HOOK(0x006FA150, Replica_006fa150, "stl");
