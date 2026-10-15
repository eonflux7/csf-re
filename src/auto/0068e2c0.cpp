#include <cstdint>
#include "globals.h"
#include "hook.h"
static uint32_t* __fastcall SharedHelper(uint32_t* self, void*, uint32_t x, uint32_t y, uint32_t z) {
    self[0] = x; self[1] = y; self[2] = z; return self;
}
HOOK(0x0068E2C0, SharedHelper, "stl");
