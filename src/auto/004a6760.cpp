#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Maps the status code returned by the virtual slot at +0x80 to a result code:
//   0 -> 0, 1 -> 20, 2 -> 0, anything else -> -1.
static int __fastcall C_008346e8_MapSlot80StatusToResult(C_008346e8* self, void* /*edx*/, int a2)
{
    // Slot 0x80 / 4 = 32 on the object's vtable. The original passes a2 as the single stack argument.
    int status = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[32])(self, nullptr, a2);

    if (status != 0)
    {
        int statusMinusOne = status - 1;
        if (statusMinusOne == 0)
            return 20;
        if (statusMinusOne != 1)
            return -1;
    }
    return 0;
}

HOOK(0x4A6760, C_008346e8_MapSlot80StatusToResult, "auto");
