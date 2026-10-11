#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: 0x004E58E0, returns an index (-1 when not found) and takes one stack argument with ecx = this.
CSF_FUNC(int, __fastcall, C_0083eb70_FindIndex, 0x004E58E0, (void* self, void* edx, int a2));

// Returns true when the lookup in the base routine does not yield -1.
static bool __fastcall C_0083eb70_HasEntry(C_0083eb70* self, void* /*edx*/, int a2)
{
    return C_0083eb70_FindIndex(self, nullptr, a2) != -1;
}

HOOK(0x004E5B40, C_0083eb70_HasEntry, "auto");
