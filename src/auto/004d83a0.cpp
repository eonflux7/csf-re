#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, C_DecrementCounterAt30, 0x0048CAC0, (void* self, void*, int));

// Vtable referenced by the AiCore task-target buffer accessor (see packet).
[[maybe_unused]] static void* const kAiCoreTargetVtbl = CSF_VTBL(0x0047AE10);

// Clears the task targets of both sub-tasks, decrements the owner's counter,
// and clears bit 0 of the owner's flag word. Returns the cleared flag word.
static uint32_t __fastcall TCharTask_ResetTargetsAndClearOwnerFlag(TCharTask* self, void* /*edx*/)
{
    CharTaskBase_Prepare(self, nullptr);

    // Primary sub-task at +0x18: reset its target name buffer, then call slot 40 (+0xA0).
    if (CSF_AT(uint32_t, self, 0x18) != 0) {
        void* primaryTask = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x18));
        char* targetNameBuffer = AiCore_GetTaskTargetNameBuffer(
            reinterpret_cast<S_0047ae10*>(primaryTask), nullptr);
        ResetState(targetNameBuffer, nullptr);

        void* primaryTaskAgain = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x18));
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, primaryTaskAgain, 0)[40])(primaryTaskAgain, nullptr);
    }

    // Secondary sub-task at +0x14: clear its field at +0x1C, then call slot 40 (+0xA0).
    if (CSF_AT(uint32_t, self, 0x14) != 0) {
        void* secondaryTask = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x14));
        CSF_AT(uint32_t, secondaryTask, 0x1C) = 0;

        void* secondaryTaskAgain = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x14));
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, secondaryTaskAgain, 0)[40])(secondaryTaskAgain, nullptr);
    }

    // Decrement the counter at owner->parent->[+0x204].
    void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x08));
    void* ownerParent = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
    void* counterObject = reinterpret_cast<void*>(CSF_AT(uint32_t, ownerParent, 0x204));
    C_DecrementCounterAt30(counterObject, nullptr, (int)reinterpret_cast<uintptr_t>(self));

    // Clear bit 0 of the owner's flag word and return the result.
    uint32_t ownerRecord = CSF_AT(uint32_t, self, 0x08);
    uint32_t flagWord = CSF_AT(uint32_t, ownerRecord, 0x18) & 0xFFFFFFFEu;
    CSF_AT(uint32_t, ownerRecord, 0x18) = flagWord;
    return flagWord;
}

HOOK(0x4D83A0, TCharTask_ResetTargetsAndClearOwnerFlag, "auto");
