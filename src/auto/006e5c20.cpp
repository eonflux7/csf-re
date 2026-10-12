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

// Original callees, bound by address.
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* /*edx*/, int key));
CSF_FUNC(int, __fastcall, GfxMatrix_ResetAndTranslate, 0x006ADC10, (void* self, void* /*edx*/, int a2));

// Applies a 2D translation (x, y) to the matrix stored under payload key 7
// of the object referenced by the owner's field. Returns the value of the
// payload pointer slot (0 when nothing was applied).
static uint32_t* __fastcall C_00861ce0_ApplyAltTranslation(C_00861ce0* self, void* /*edx*/, float x, float y)
{
    // Virtual getter on the owner object (vtable slot 0x1C / 4 = 7); takes no arguments.
    void* owner = reinterpret_cast<void*>(self->field_0040);
    C_0085e7b8* entryTable = reinterpret_cast<C_0085e7b8*(__fastcall*)(void*, void*)>(
        CSF_AT(void**, owner, 0)[7])(owner, nullptr);

    // The key 7 stays on the stack for the lookup below.
    uint32_t* result = reinterpret_cast<uint32_t*>(
        C_0085e7b8_FindEntryPayloadByKey(entryTable, nullptr, 7));
    if (result)
    {
        result = reinterpret_cast<uint32_t*>(*result);
        if (result)
        {
            float translate[3];
            translate[0] = x;
            translate[1] = y;
            translate[2] = 0.0f;

            int matrixRet = GfxMatrix_ResetAndTranslate(
                reinterpret_cast<uint8_t*>(result) + 4, nullptr,
                static_cast<int>(reinterpret_cast<uintptr_t>(translate)));
            return reinterpret_cast<uint32_t*>(matrixRet);
        }
    }
    return result;
}

HOOK(0x6E5C20, C_00861ce0_ApplyAltTranslation, "auto");
