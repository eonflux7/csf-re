#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, C_00862640_ReleaseElementState, 0x006F2E10, (void* self, void* edx));

// Destroys a single object, or an array whose element count is stored in the
// 4 bytes before the first element (element size 0x54). Frees the block when
// the low flag bit is set. Returns the block base (array) or the object itself.
static char* __fastcall C_00862640_ScalarOrArrayDelete(C_00862640* self, void* /*edx*/, char deleteFlags)
{
    if ((deleteFlags & 2) != 0)
    {
        // Array form: element count lives just before the first element.
        int32_t elementCount = CSF_AT(int32_t, self, -4);
        char* blockBase = reinterpret_cast<char*>(self) - 4;

        // Destroy elements from last to first.
        uint8_t* cursor = reinterpret_cast<uint8_t*>(self) + static_cast<intptr_t>(elementCount) * 0x54;
        for (int32_t remaining = elementCount; remaining > 0; --remaining)
        {
            cursor -= 0x54;
            C_00862640_ReleaseElementState(cursor, nullptr);
        }

        if ((deleteFlags & 1) != 0)
            CoreMemory_Free(blockBase);

        return blockBase;
    }

    // Scalar form.
    C_00862640_ReleaseElementState(self, nullptr);

    if ((deleteFlags & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<char*>(self);
}

HOOK(0x006CA1E0, C_00862640_ScalarOrArrayDelete, "auto");
