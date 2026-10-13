#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

// Removes every light handle in this object's list from the RW manager, then clears the count.
static int __fastcall C_00862e88_RemoveAllLightsFromWorld(C_00862e88* self, void* /*edx*/)
{
    int result = self->field_004c;
    for (int i = 0; i < result; ++i)
    {
        int globalManagerId = GetGlobalInt();
        int rwManager = ResolveRwManager(globalManagerId, nullptr);
        void* manager = reinterpret_cast<void*>(rwManager);

        // Slot +0x24 of the RW manager vtable (index 9).
        int lightHandle = static_cast<int>(reinterpret_cast<uint32_t*>(self->field_0044)[i]);
        ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, manager, 0)[9])(manager, nullptr, lightHandle);

        result = self->field_004c;
    }
    self->field_004c = 0;
    return result;
}

HOOK(0x6FC510, C_00862e88_RemoveAllLightsFromWorld, "auto");
