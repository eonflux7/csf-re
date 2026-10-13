#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Forwards to the object stored at +0x48 through its vtable slot 18 (+0x48) when set;
// otherwise the original returns 0.0 (loaded from g_fZero).
static hook::Tail __fastcall C_00833908_GetQueryRate(void* self, const uint32_t* args) {
    (void)args;
    void* queryObj = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x48));
    if (queryObj == nullptr) {
        uint32_t zeroBits = 0;
        std::memcpy(&zeroBits, &g_fZero, sizeof(zeroBits));
        return hook::tail_ret(zeroBits);
    }
    return hook::tail_jump(queryObj, (*reinterpret_cast<void***>(queryObj))[0x48 / 4]);
}

HOOK_TAIL(0x0048E4F0, C_00833908_GetQueryRate, "auto");
