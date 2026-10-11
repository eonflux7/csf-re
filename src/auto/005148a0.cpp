#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Bound by address: sub_516780, __fastcall with 3 stack args (ret 12).
CSF_FUNC(char, __fastcall, CProfileManager_ProcessSaveSlotRecord, 0x00516780,
         (void* self, void* edx, unsigned int a2, int a3, int a4));

// Bound by address: GetGlobalValue, __cdecl, no args.
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

// __thiscall method (ecx = this), 3 stack args + ret 12.
static int __fastcall CProfileManager_ApplySaveGameState(CProfileManager* self, void* /*edx*/,
                                                         uint32_t a2, uint32_t a3, char a4)
{
    uint8_t* slotRecord = reinterpret_cast<uint8_t*>(a3);

    // Branch taken when a4 is set, or when the global flag at +204 is clear.
    uint8_t* globals = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetGlobalValue_75FDF0()));
    if (a4 || !CSF_AT(uint8_t, globals, 204)) {
        CSF_AT(uint8_t, slotRecord, 77) = 1;
        return static_cast<int>(CProfileManager_ProcessSaveSlotRecord(
            self, nullptr, a2, static_cast<int>(a3), 0));
    }

    CSF_AT(uint8_t, slotRecord, 77) = 0;
    if (!CProfileManager_ProcessSaveSlotRecord(self, nullptr, a2, static_cast<int>(a3), 0))
        return 0;
    // Virtual slot +0x38 (index 14) on the object.
    char slotResult = ((char(__fastcall*)(void*, void*))self->vtbl[14])(self, nullptr);
    return slotResult ? 1 : 0;
}

HOOK(0x005148A0, CProfileManager_ApplySaveGameState, "auto");
