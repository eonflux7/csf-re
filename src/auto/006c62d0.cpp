#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Original: 0x006C55B0, __cdecl, two int args, returns double in st0.
CSF_FUNC(double, __cdecl, GfxSkin_UpdateSkinBlock, 0x006C55B0, (int a1, int a2));

CSF_GLOBAL(int, g_Value_8BB4B8, 0x008BB4B8);

// Original: 0x006C62D0, __thiscall, ret 0.
// Passes the dword at this+4 and the dword at (*g_Value_8BB4B8)+0xF0 to GfxSkin_UpdateSkinBlock.
// The callee's double result stays in st0 and is returned unchanged.
static double __fastcall GfxSkin_ForwardSkinBlockUpdate(float* self, void* /*edx*/) {
    int blockValue = CSF_AT(int, self, 4);
    void* skinContext = reinterpret_cast<void*>(static_cast<uintptr_t>(g_Value_8BB4B8));
    int contextTable = CSF_AT(int, skinContext, 0xF0);
    return GfxSkin_UpdateSkinBlock(blockValue, contextTable);
}

HOOK(0x6C62D0, GfxSkin_ForwardSkinBlockUpdate, "auto");
