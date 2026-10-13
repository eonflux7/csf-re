#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

// Binary-derived vector::buy; survey cluster 00510510, stride 80.
using Value = legacy::vc71::element<80>;
using Vector = legacy::vc71::vector<Value>;

static uint8_t __fastcall Replica_00510510(Vector* self, void*, uint32_t count) {
    return legacy::vc71::vector_buy(self, count);
}

HOOK(0x00510510, Replica_00510510, "stl");
