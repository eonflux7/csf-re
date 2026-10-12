#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

// Resets the 0x1C-byte pose scratch record (floats at +0x04/+0x08/+0x10, zeroed words at +0x0C/+0x14)
// and copies the dword at GetGlobalValue_8BC1E0()+0x1D98 into +0x18, which is also the return value.
static int __fastcall InitPoseScratchBuffer(void* self, void* /*edx*/)
{
    CSF_AT(float, self, 0x10) = -1.0f;
    CSF_AT(float, self, 0x08) = -1.0f;
    CSF_AT(int, self, 0x00) = -1;
    CSF_AT(int, self, 0x14) = 0;
    CSF_AT(int, self, 0x0C) = 0;
    CSF_AT(float, self, 0x04) = 255.0f;

    int globalValue = CSF_AT(int, GetGlobalValue_8BC1E0(), 0x1D98);
    CSF_AT(int, self, 0x18) = globalValue;
    return globalValue;
}

HOOK(0x4582E0, InitPoseScratchBuffer, "auto");
