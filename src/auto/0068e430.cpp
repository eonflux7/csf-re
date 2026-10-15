#include <cstdint>
#include "globals.h"
#include "hook.h"
CSF_FUNC(uint32_t*, __fastcall, Construct, 0x0068E2C0, (uint32_t* self, void* edx, uint32_t x, uint32_t y, uint32_t z));
static uint32_t* __fastcall SharedHelper(uint32_t* self, void*, uint32_t* result) {
    Construct(result, nullptr, self[0], self[1], self[2]); return result;
}
HOOK(0x0068E430, SharedHelper, "stl");
