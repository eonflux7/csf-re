#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::buy; survey cluster 433, stride 552.
using Value = legacy::vc71::element<552>;
using Vector = legacy::vc71::vector<Value>;

static uint8_t __fastcall Replica_005111c0(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_buy(self, count);
}

HOOK(0x005111C0, Replica_005111c0, "stl");
