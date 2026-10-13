#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

// Returns the flags word of the linked task state when its bit 1 is set (and the
// first virtual check fails); otherwise resets the linked task state.
static uint32_t* __stdcall AiCoverAttackTasks_RefreshLinkedTaskState(int a1, int a2)
{
    (void)a2;
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(a1)));

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint32_t objectBase = linkedValue ? static_cast<uint32_t>(linkedValue - 4) : 0u;

    void* linkedNode = reinterpret_cast<void*>(static_cast<uintptr_t>(objectBase));
    void* taskObject = CSF_AT(void*, linkedNode, 0x2C);
    void* taskState = CSF_AT(void*, taskObject, 0x30);

    // vtable slot 0xB8 / 4 = 46
    bool isBusy = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, taskState, 0)[46])(taskState, nullptr);
    if (!isBusy) {
        // vtable slot 0xBC / 4 = 47, argument is the address of our first stack argument
        uint32_t* flags = reinterpret_cast<uint32_t*>(
            ((void* (__fastcall*)(void*, void*, void*))CSF_AT(void**, taskState, 0)[47])(taskState, nullptr, &a1));
        if ((flags[0] & 2u) != 0) {
            return flags;
        }
    }

    int field10 = GetLinkedObjectField10(self, nullptr);
    int resetResult = AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, field10 + 8);
    return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(resetResult)));
}

HOOK(0x004AD720, AiCoverAttackTasks_RefreshLinkedTaskState, "auto");
