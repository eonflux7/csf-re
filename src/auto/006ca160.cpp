#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00861508_ReleaseChildAndParentState, 0x006DC910, (C_00861508* self, void*));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Vector/scalar deleting destructor for C_00861508.
// flags bit 1 (0x2): array form, element count stored in the cookie at this-4,
//                    elements are 0x98 bytes each and destroyed back to front.
// flags bit 0 (0x1): release the memory block after destruction.
static char* __fastcall C_00861508_VectorDeletingDestructor(C_00861508* self, void* /*edx*/, char flags)
{
    if ((flags & 2) != 0)
    {
        int32_t elementCount = CSF_AT(int32_t, self, -4);
        char* arrayBase = reinterpret_cast<char*>(self) - 4;

        uint8_t* cursor = reinterpret_cast<uint8_t*>(self) + elementCount * 0x98;
        for (int32_t i = elementCount; i > 0; --i)
        {
            cursor -= 0x98;
            C_00861508_ReleaseChildAndParentState(reinterpret_cast<C_00861508*>(cursor), nullptr);
        }

        if ((flags & 1) != 0)
            CoreMemory_Free(arrayBase);

        return arrayBase;
    }

    C_00861508_ReleaseChildAndParentState(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x006CA160, C_00861508_VectorDeletingDestructor, "auto");
