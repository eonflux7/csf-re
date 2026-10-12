#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static unsigned int __fastcall C_008311c4_DispatchArray(
    C_008311c4* self, void* /*edx*/, std::uint32_t* array)
{
    unsigned int count = array[2];
    for (unsigned int index = 0; index < count; ++index)
    {
        const auto* elements = reinterpret_cast<const int*>(array[0]);
        self->vSlot0(elements[index]);
        count = array[2];
    }
    return count;
}

HOOK(0x004F9630, C_008311c4_DispatchArray, "auto");
