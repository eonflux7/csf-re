#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char*, __fastcall, AiAttackTasks_FindSlotRecord, 0x00691860, (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_InsertSlotRecord, 0x00691FC0, (void* self, void* edx, uint32_t* a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_ServiceLoading, 0x0069AF70, (void* self, void* edx));

// S_00691860 vtable is referenced by the original's code (its ctor chain / class identity).
[[maybe_unused]] static const auto kS00691860Vtable = CSF_VTBL(0x00691860);

static int __fastcall AiAttackTasks_ApplySlotState(S_00691860* self, void* /*edx*/,
                                                   unsigned int a2, int a3, int /*a4*/, char a5)
{
    char* slot = AiAttackTasks_FindSlotRecord(self, nullptr, a2);

    if (slot && CSF_AT(uint32_t, slot, 0) == a2) {
        // Existing record: release the old payload if it owns one, then store the new state.
        if (CSF_AT(uint32_t, slot, 4) && CSF_AT(char, slot, 8)) {
            Mem_Free(reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, slot, 4))));
            CSF_AT(uint32_t, slot, 4) = 0;
        }
        CSF_AT(uint32_t, slot, 4) = static_cast<uint32_t>(a3);
        CSF_AT(char, slot, 8) = a5;
        return SceneLoader_ServiceLoading(nullptr, nullptr);
    }

    // No matching record: build a temporary entry on the stack and insert it.
    alignas(8) uint8_t recordBytes[0x10];
    CSF_AT(uint32_t, recordBytes, 0) = a2;
    CSF_AT(uint8_t, recordBytes, 9) = 0;
    CSF_AT(uint32_t, recordBytes, 0xC) = 0;
    CSF_AT(uint32_t, recordBytes, 4) = static_cast<uint32_t>(a3);
    CSF_AT(char, recordBytes, 8) = a5;

    AiAttackTasks_InsertSlotRecord(self, nullptr, reinterpret_cast<uint32_t*>(recordBytes),
                                   static_cast<int>(reinterpret_cast<uintptr_t>(slot)));
    return SceneLoader_ServiceLoading(nullptr, nullptr);
}

HOOK(0x00692200, AiAttackTasks_ApplySlotState, "auto");
