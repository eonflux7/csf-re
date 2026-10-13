#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::buy; survey cluster 00410bd0, stride 16.
using Value = legacy::vc71::element<16>;
using Vector = legacy::vc71::vector<Value>;

static uint8_t __fastcall Replica_00410bd0(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_buy(self, count);
}

HOOK(0x00410BD0, Replica_00410bd0, "stl");
