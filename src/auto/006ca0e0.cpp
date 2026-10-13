#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, RpPrtStd_ResetAtomicState, 0x006F3A90, (C_00862748* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Vector/scalar deleting destructor for C_00862748.
// flags bit 1 (0x2): array form, element count stored in the cookie at this-4,
//                    elements are 0x88 bytes each and destroyed back to front.
// flags bit 0 (0x1): release the memory block after destruction.
static char* __fastcall C_00862748_VectorDeletingDestructor(C_00862748* self, void* /*edx*/, char flags)
{
    if ((flags & 2) != 0)
    {
        int32_t elementCount = CSF_AT(int32_t, self, -4);
        char* arrayBase = reinterpret_cast<char*>(self) - 4;

        uint8_t* cursor = reinterpret_cast<uint8_t*>(self) + elementCount * 0x88;
        for (int32_t i = elementCount; i > 0; --i)
        {
            cursor -= 0x88;
            RpPrtStd_ResetAtomicState(reinterpret_cast<C_00862748*>(cursor), nullptr);
        }

        if ((flags & 1) != 0)
            CoreMemory_Free(arrayBase);

        return arrayBase;
    }

    RpPrtStd_ResetAtomicState(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x006CA0E0, C_00862748_VectorDeletingDestructor, "auto");
