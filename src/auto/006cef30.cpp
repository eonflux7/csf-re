#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// SceneMissionSetup_ApplyWorldSetup: called first, returns an index.
CSF_FUNC(int, __fastcall, SceneMissionSetup_ApplyWorldSetup, 0x006CEE30, (void* self, void* edx, char* a2));

// Allocates/initializes the per-entry record stored at entry+0x60 (__cdecl, one stack arg).
CSF_FUNC(uint32_t*, __cdecl, CollisionShapes_InitShapeEntry, 0x006B23E0, (int a1));

static int __fastcall CollisionShapes_ApplySetupAndInitEntry(S_006cee30* self, void* /*edx*/, char* a2)
{
    int entryIndex = SceneMissionSetup_ApplyWorldSetup(self, nullptr, a2);

    // Entry records are 100 bytes each, starting at self->field_0008.
    uint32_t entryAddr = self->field_0008 + static_cast<uint32_t>(100 * entryIndex);
    uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entryAddr));

    if (CSF_AT(uint32_t, entry, 0x60) == 0)
    {
        uint32_t* initialized = CollisionShapes_InitShapeEntry(static_cast<int>(entryAddr));
        CSF_AT(uint32_t, entry, 0x60) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(initialized));
    }

    return entryIndex;
}

HOOK(0x006CEF30, CollisionShapes_ApplySetupAndInitEntry, "auto");
