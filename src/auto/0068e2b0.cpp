#include <cstdint>
#include "globals.h"
#include "hook.h"
static uint32_t* __fastcall SharedHelper(uint32_t* self, void*) {
    self[0] = self[1] = self[2] = 0; return self;
}
HOOK(0x0068E2B0, SharedHelper, "stl");
