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

CSF_FUNC(int, __cdecl, RwTextureDestroy, 0x006317E0, (int texture));
CSF_FUNC(int*, __fastcall, S_0070d0e0_EraseFromIndex, 0x0070D0E0, (void* self, void* edx, int* a4, int a1, uint32_t* arg8));
CSF_FUNC(int, __fastcall, S_0070d0e0_AppendFillCopies, 0x0070D140, (void* self, void* edx, char* a1, unsigned int a3, int* arg8));

// Reference to the vtable address listed by the packet (no runtime behavior).
[[maybe_unused]] static const void* const kS_0070d0e0VtableRef = CSF_VTBL(0x0070D0E0);

// Resizes the element range of S_0070d0e0 to a2 elements. Elements are 8 bytes each.
// Shrinks by erasing the tail from index a2, grows by appending copies of a3.
// a4 is an optional texture released at the end.
static int __fastcall S_0070d0e0_ResizeTo(S_0070d0e0* self, void* /*edx*/, unsigned int a2, int a3, int a4)
{
    uint32_t begin = self->field_0004;
    unsigned int currentCount = 0;
    if (begin)
        currentCount = static_cast<unsigned int>(static_cast<int32_t>(self->field_0008 - begin) >> 3);

    if (currentCount >= a2)
    {
        if (begin && a2 < static_cast<unsigned int>(static_cast<int32_t>(self->field_0008 - begin) >> 3))
        {
            S_0070d0e0_EraseFromIndex(
                self,
                nullptr,
                reinterpret_cast<int*>(&a2),
                static_cast<int>(static_cast<uintptr_t>(begin + 8 * a2)),
                reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(self->field_0008)));
        }
    }
    else
    {
        unsigned int countNow = 0;
        if (begin)
            countNow = static_cast<unsigned int>(static_cast<int32_t>(self->field_0008 - begin) >> 3);

        S_0070d0e0_AppendFillCopies(
            self,
            nullptr,
            reinterpret_cast<char*>(static_cast<uintptr_t>(self->field_0008)),
            a2 - countNow,
            reinterpret_cast<int*>(&a3));
    }

    if (a4)
        return RwTextureDestroy(a4);
    return a4;
}

HOOK(0x0070D4E0, S_0070d0e0_ResizeTo, "auto");
