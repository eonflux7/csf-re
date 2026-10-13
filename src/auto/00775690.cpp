#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::buy; survey cluster 356, stride 60.
using Value = legacy::vc71::element<60>;
using Vector = legacy::vc71::vector<Value>;

static uint8_t __fastcall Replica_00775690(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_buy(self, count);
}

HOOK(0x00775690, Replica_00775690, "stl");
