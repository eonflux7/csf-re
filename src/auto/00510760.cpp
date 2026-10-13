#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::buy; survey cluster 433, stride 164.
using Value = legacy::vc71::element<164>;
using Vector = legacy::vc71::vector<Value>;

static uint8_t __fastcall Replica_00510760(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_buy(self, count);
}

HOOK(0x00510760, Replica_00510760, "stl");
