#include <cstdint>
#include "globals.h"
#include "hook.h"
static uint32_t __fastcall SharedHelper(uint32_t* self, void*, const uint32_t* source) {
    self[0] = source[0]; self[1] = source[1]; self[2] = source[2]; return source[2];
}
HOOK(0x0068E450, SharedHelper, "stl");
