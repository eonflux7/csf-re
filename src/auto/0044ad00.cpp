#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector helper; survey cluster 1076, stride 12.
using Value = legacy::vc71::element<12>;
using Operations = legacy::vc71::trivial_assignment<12, 4>;
static uint32_t __cdecl Replica_0044ad00(uint32_t begin, uint32_t end, uint32_t value) {
    return legacy::vc71::vector_fill_assign<Operations>(begin, end, value);
}

HOOK(0x0044AD00, Replica_0044ad00, "stl");
