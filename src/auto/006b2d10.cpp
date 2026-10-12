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

// Per-element state applier (original 0x6E5650). Called once per non-null array element.
CSF_FUNC(char, __fastcall, sub_6E5650, 0x006E5650, (void* self, void* edx, char a2));

// Applies a state flag to every non-null element of the object's pointer array.
// Layout: +0x18 = element pointer array, +0x1C = element count (uint16).
static int __fastcall GfxScene_SetClumpState_6B2D10(void* self, void* /*edx*/, int a2)
{
    int result = 0;
    for (unsigned int i = 0; i < CSF_AT(uint16_t, self, 0x1C); ++i)
    {
        uint32_t* elements = CSF_AT(uint32_t*, self, 0x18);
        result = elements[i];
        if (elements[i] != 0)
        {
            void* element = reinterpret_cast<void*>(static_cast<uintptr_t>(elements[i]));
            result = sub_6E5650(element, nullptr, static_cast<char>(a2));
        }
    }
    return result;
}

HOOK(0x006B2D10, GfxScene_SetClumpState_6B2D10, "auto");
