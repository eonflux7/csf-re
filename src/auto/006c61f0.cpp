#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char, __cdecl, sub_6EFA30, 0x006EFA30, (int a1));

static int __fastcall GfxSkin_ForwardFieldFourQuery(void* self, void* /*edx*/) {
    uint32_t innerValue = CSF_AT(uint32_t, self, 0x4);
    return sub_6EFA30(static_cast<int>(innerValue));
}

HOOK(0x006C61F0, GfxSkin_ForwardFieldFourQuery, "auto");
