#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "core/memory.h"

// Calls the virtual predicate at vtable slot 0x94/4 = 37 on the subobject at (self - 4).
// If it returns false, sets the byte at self+0x6AC to 1. Returns the predicate result.
static int __fastcall AiMoveTasks_SetFlagIfCheckFails(uint8_t* self, void* /*edx*/, int /*a2*/, int /*a3*/)
{
    uint8_t* subobject = self - 4;
    int result = ((int(__fastcall*)(void*, void*))CSF_AT(void**, subobject, 0)[0x94 / 4])(subobject, nullptr);
    if (!(uint8_t)result)
        CSF_AT(uint8_t, self, 0x6AC) = 1;
    return result;
}

HOOK(0x4AA740, AiMoveTasks_SetFlagIfCheckFails, "auto");
