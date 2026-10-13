#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 1076, stride 8.
using Value = legacy::vc71::element<8>;
using Operations = legacy::vc71::trivial_assignment<8, 0>;
static uint32_t __cdecl Replica_0070ce20(uint32_t begin, uint32_t end, uint32_t value) {
    return legacy::vc71::vector_fill_assign<Operations>(begin, end, value);
}

HOOK(0x0070CE20, Replica_0070ce20, "stl");
