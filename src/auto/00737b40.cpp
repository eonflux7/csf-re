#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base-class cleanup step run before the object is released (0x737360).
CSF_FUNC(void, __fastcall, CleanupBaseObject_737360, 0x00737360, (void* self, void* edx));

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Deleting destructor for C_00867260: runs the base cleanup, then frees
// the object through the game heap when the low bit of the flags argument is set.
// Returns the object pointer as char*, as the original does (mov eax, esi).
static char* __fastcall C_00867260_Destroy(C_00867260* self, void* /*edx*/, char flags)
{
    CleanupBaseObject_737360(self, nullptr);
    if ((flags & 1) != 0)
        Mem_Free(self);
    return reinterpret_cast<char*>(self);
}

HOOK(0x737B40, C_00867260_Destroy, "auto");
