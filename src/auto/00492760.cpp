#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Sets the action mode at +0x1E0. If it changed, notifies the embedded
// sub-object at +4 through its vtable slot 14 (offset 0x38) and returns that result.
static int __fastcall C_00834050_SetActionMode(void* self, void* /*edx*/, int newActionMode)
{
    int result = newActionMode;
    if (newActionMode != CSF_AT(int, self, 0x1E0))
    {
        CSF_AT(int, self, 0x1E0) = newActionMode;
        void* subObject = reinterpret_cast<uint8_t*>(self) + 4;
        void** subVtbl = CSF_AT(void**, self, 4);
        return ((int(__fastcall*)(void*, void*))subVtbl[0x38 / 4])(subObject, nullptr);
    }
    return result;
}

HOOK(0x492760, C_00834050_SetActionMode, "auto");
