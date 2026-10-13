#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::buy; survey cluster 678, stride 8.
using Value = legacy::vc71::element<8>;
using Vector = legacy::vc71::vector<Value>;

static uint8_t __fastcall Replica_005587e0(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_buy(self, count);
}

HOOK(0x005587E0, Replica_005587e0, "stl");
