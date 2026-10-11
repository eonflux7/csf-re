#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Callee at 0x006C55B0: evaluates a skinned-geometry value from its float-bit argument.
CSF_FUNC(double, __cdecl, GfxSkin_EvaluateValue, 0x006C55B0, (int a1, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

// Returns the value evaluated from the object's float at +0x0C, or zero when the object is null.
static double __cdecl GfxSkin_GetLinkedValueOrZero(int object, int context)
{
    if (object != 0)
    {
        // The original pushes the dword at object+0x0C unchanged (raw bits), so read it as int.
        int valueBits = CSF_AT(int, reinterpret_cast<void*>(object), 0x0C);
        return GfxSkin_EvaluateValue(valueBits, context);
    }
    return static_cast<double>(g_fZero);
}

HOOK(0x006C56B0, GfxSkin_GetLinkedValueOrZero, "auto");
