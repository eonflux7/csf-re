#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 827, stride 32.
using Value = legacy::vc71::element<32>;
using Operations = legacy::vc71::trivial_assignment<32, 0>;
static uint32_t __cdecl Replica_006f9fb0(uint32_t begin, uint32_t end, uint32_t value) {
    return legacy::vc71::vector_fill_assign<Operations>(begin, end, value);
}

HOOK(0x006F9FB0, Replica_006f9fb0, "stl");
