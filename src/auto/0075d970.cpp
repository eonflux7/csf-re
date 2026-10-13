#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::buy; survey cluster 356, stride 44.
using Value = legacy::vc71::element<44>;
using Vector = legacy::vc71::vector<Value>;

static uint8_t __fastcall Replica_0075d970(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_buy(self, count);
}

HOOK(0x0075D970, Replica_0075d970, "stl");
