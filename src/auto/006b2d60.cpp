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

CSF_FUNC(int, __cdecl, RpMorphAtomicAddTime, 0x005774B0, (int a1, float a2));

// Advances the morph time of every atomic in the object's atomic list.
// The list pointer lives at +0x18 and its element count is a 16-bit value at +0x1C.
static unsigned int __fastcall MorphObjectAddTime(void* self, void* /*edx*/, float deltaTime)
{
    unsigned int index = 0;
    unsigned int result = 0;

    if (CSF_AT(uint16_t, self, 0x1C) != 0)
    {
        do
        {
            uint32_t* atomicList = CSF_AT(uint32_t*, self, 0x18);
            void* atomic = reinterpret_cast<void*>(atomicList[index]);
            RpMorphAtomicAddTime(CSF_AT(int, atomic, 0x0C), deltaTime);
            result = CSF_AT(uint16_t, self, 0x1C);
            ++index;
        } while (index < result);
    }
    return result;
}

HOOK(0x006B2D60, MorphObjectAddTime, "auto");
