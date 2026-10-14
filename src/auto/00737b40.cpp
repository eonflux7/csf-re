#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00867260_Destroy_737360, 0x00737360, (C_00867260* self, void*));

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

// Deleting destructor for C_00867260: runs the destructor body, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer as char*, as the original does (mov eax, esi).
static char* __fastcall C_00867260_Destroy(C_00867260* self, void* /*edx*/, char flags)
{
    C_00867260_Destroy_737360(self, nullptr);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);
    return reinterpret_cast<char*>(self);
}

HOOK(0x737B40, C_00867260_Destroy, "auto");
