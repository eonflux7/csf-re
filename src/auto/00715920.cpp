#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::buy; survey cluster 687, stride 12.
using Value = legacy::vc71::element<12>;
using Vector = legacy::vc71::vector<Value>;

static uint8_t __fastcall Replica_00715920(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_buy(self, count);
}

HOOK(0x00715920, Replica_00715920, "stl");
