#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, GfxMatrix_ApplyAxisTransform, 0x006ADA10,
         (void* self, void* edx, uint32_t* a2, uint32_t* a3, uint32_t* a4, uint32_t* a5));

CSF_GLOBAL(float, g_AxisX, 0x008A7D6C);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);

// Vtable constant referenced by this routine's class (S_006ada10).
[[maybe_unused]] static void* const kAxisTransformVtbl = CSF_VTBL(0x006ADA10);

// Builds the X-axis rotation pair (sin/cos of angle) and forwards it to the
// axis-transform helper together with the global X axis and zero vector.
static int __fastcall GfxMatrix_SetRotationAroundX(S_006ada10* self, void* /*edx*/, float angle)
{
    int translationFlag = 0;
    float rotationRow[3];
    rotationRow[0] = 0.0f;

    float sineValue = std::sin(angle);
    float cosineValue = std::cos(angle);
    rotationRow[1] = -sineValue;
    rotationRow[2] = cosineValue;

    return GfxMatrix_ApplyAxisTransform(
        self, nullptr,
        reinterpret_cast<uint32_t*>(&g_AxisX),
        reinterpret_cast<uint32_t*>(&translationFlag),
        reinterpret_cast<uint32_t*>(rotationRow),
        reinterpret_cast<uint32_t*>(&g_ZeroVectorX));
}

HOOK(0x006AE250, GfxMatrix_SetRotationAroundX, "auto");
