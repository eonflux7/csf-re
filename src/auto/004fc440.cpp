#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));

// Flushes up to three pending global scene slots (field_0060..+0x68) through the
// entry lookup object and the global RW manager, clearing each slot after use.
static void __fastcall SceneLoader_PrepareGlobalSceneState(C_0083f448* self, void* /*edx*/) {
    uint32_t* slot = &self->field_0060;
    int remaining = 3;
    do {
        if (*slot) {
            if (self->field_001c) {
                C_0085e760* entryObject = *reinterpret_cast<C_0085e760**>(self->field_0014);
                if (entryObject) {
                    int entryValue = ResolveEntryLookupValue(entryObject, nullptr);
                    if (entryValue) {
                        void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(entryValue));
                        // vtable slot +0x3C (15): void entry->vfn(uint32_t slotValue)
                        reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                            CSF_AT(void**, entry, 0)[15])(entry, nullptr, *slot);
                    }
                    int globalInt = GetGlobalInt();
                    int rwManagerValue = ResolveRwManager(globalInt, nullptr);
                    void* rwManager = reinterpret_cast<void*>(static_cast<uintptr_t>(rwManagerValue));
                    // vtable slot +0x24 (9): void rwManager->vfn(uint32_t slotValue)
                    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t)>(
                        CSF_AT(void**, rwManager, 0)[9])(rwManager, nullptr, *slot);
                    *slot = 0;
                }
            }
        }
        ++slot;
        --remaining;
    } while (remaining);
}

HOOK(0x4FC440, SceneLoader_PrepareGlobalSceneState, "auto");
