#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::buy; survey cluster 687, stride 20.
using Value = legacy::vc71::element<20>;
using Vector = legacy::vc71::vector<Value>;

static uint8_t __fastcall Replica_00558840(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_buy(self, count);
}

HOOK(0x00558840, Replica_00558840, "stl");
