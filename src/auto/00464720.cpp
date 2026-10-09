#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, ComputeObjectIndex, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, TryGetSubEntryIndex, 0x00471ED0, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint8_t*, __fastcall, DispatchWithDefaultEntry, 0x00472B20, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, DispatchWithSubEntry, 0x00472B60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetObjectSlotCount, 0x00672AC0, (void* self, void* edx));

// __thiscall(this, a2, a3, a4) with ret 12  ->  __fastcall(self, edx, a2, a3, a4)
static int __fastcall DispatchEntryFromTable(void* self, void* /*edx*/, C_008313f8* a2, int a3, void* a4)
{
    (void)a3;

    // Flags word of the record that a4 points to.
    uint32_t v4 = CSF_AT(uint32_t, CSF_AT(void*, a4, 0), 8);

    // Owner of the table: self+0x20, or the current scene's child when the flag is set.
    uint32_t tableOwner = CSF_AT(uint32_t, self, 0x20);
    if ((v4 & 0x800000u) != 0)
    {
        void* scene = g_pCurrentScene;
        uint32_t sceneChild = CSF_AT(uint32_t, scene, 0xBC);
        tableOwner = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(sceneChild)), 0x38);
        v4 &= ~0x800000u;
    }

    // Each table entry is 12 bytes: [0] unused here, [4] object pointer.
    uintptr_t tableBase = CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(tableOwner)), 8);
    uint8_t* entry = reinterpret_cast<uint8_t*>(tableBase + 12u * v4);

    uint32_t v7 = static_cast<uint32_t>(ComputeObjectIndex(CSF_AT(void*, entry, 4), nullptr));
    if (v7 < static_cast<uint32_t>(GetObjectSlotCount(CSF_AT(void*, entry, 4), nullptr)))
    {
        int v9 = TryGetSubEntryIndex(CSF_AT(void*, entry, 4), nullptr, v7);
        if (v9 != 0)
        {
            return static_cast<int>(reinterpret_cast<uintptr_t>(
                DispatchWithSubEntry(a2, nullptr, v9)));
        }
    }

    int objectField4 = CSF_AT(int, CSF_AT(void*, entry, 4), 4);
    return static_cast<int>(reinterpret_cast<uintptr_t>(
        DispatchWithDefaultEntry(a2, nullptr, objectField4)));
}

HOOK(0x00464720, DispatchEntryFromTable, "auto");
