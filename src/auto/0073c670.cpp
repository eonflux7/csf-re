#include <cstdint>

#include "globals.h"
#include "hook.h"

// ---------------------------------------------------------------------------
// Reconstruction of 0x0073C670 (14 bytes).
//
// The original is a __stdcall thunk: it loads 0x008BABFC into ecx and tail
// jumps into the __thiscall routine at 0x0068F670. That routine is not a
// function start for the gate, so its body is inlined below instead.
// ---------------------------------------------------------------------------

// Singleton object the thunk keeps in ecx; the thunk itself never dereferences
// it, and it is forwarded as `this` to the range method that is called.
CSF_GLOBAL(uint8_t, g_FloatRangeContext_008BABFC, 0x008BABFC);

// Float constant the step argument is compared against (`fcomp [0x0082D4CC]`).
CSF_GLOBAL(float, g_FloatZero_0082D4CC, 0x0082D4CC);

// 0x0068F560: consumes a validated float range; ecx = context object, four
// float stack arguments, callee pops 16 bytes.
CSF_FUNC(void, __fastcall, C_008BABFC_AddRangedFloat, 0x0068F560,
         (void* self, void* /*edx*/, float from, float value, float zero, float to));

// 0x00646D30: error/report helper, __cdecl with four arguments (pops nothing).
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30,
         (int, int, uint32_t, int));

// Replaces 0x0073C670. Returns 1 after registering the range, 0 after reporting
// an invalid range (step <= 0 or to < from).
static char __stdcall ProcessRangedFloat(float from, float to, float step)
{
    // Exact x87 branch semantics (`test ah,0x41; jnp` then `test ah,5; jp`):
    // the error path is entered iff step <= 0 (ordered) or to < from;
    // unordered/NaN comparisons never take it.
    if (step <= g_FloatZero_0082D4CC || to < from)
    {
        Game_RaiseStatusNotice(0, 0, 0xE0030005u, 0);
        return 0;
    }

    const float value = (to - from) / step;
    C_008BABFC_AddRangedFloat(&g_FloatRangeContext_008BABFC, nullptr, from, value, 0.0f, to);
    return 1;
}

HOOK(0x73C670, ProcessRangedFloat, "auto");
