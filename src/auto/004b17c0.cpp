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

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __fastcall, AiCore_UpdateLinkedTask, 0x004B1420, (void* self, void* edx, int a3, int a4, char a5));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static void* ToAddress(uint32_t address)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(address));
}

// Dispatches the macro order to the linked AI task: either updates the linked task
// (when the owner reports it can accept the order) or resets the linked task state.
static void __stdcall AiFncMacroOrder_DispatchOrder(C_0085e760* self, int a2)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int ownerBase = linkedValue ? linkedValue - 4 : 0;
    uint32_t owner = CSF_AT(uint32_t, ToAddress(static_cast<uint32_t>(ownerBase)), 0x2C);
    uint32_t taskObjectAddress = CSF_AT(uint32_t, ToAddress(owner), 0x30);
    void* taskObject = ToAddress(taskObjectAddress);

    bool needsReset = ((bool(__fastcall*)(void*, void*))CSF_AT(void**, taskObject, 0)[46])(taskObject, nullptr);
    if (!needsReset)
    {
        uint32_t* flagsPtr = ((uint32_t*(__fastcall*)(void*, void*, C_0085e760**))CSF_AT(void**, taskObject, 0)[47])(
            taskObject, nullptr, &self);
        needsReset = (*flagsPtr & 2u) == 0;
    }

    if (needsReset)
    {
        int field10 = GetLinkedObjectField10(self, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, field10 + 4);
        int resolved = ResolveEntryLookupValue(self, nullptr);
        GetCachedObjectProvider(ToAddress(static_cast<uint32_t>(resolved)), nullptr);
        GetLinkedObjectField10(self, nullptr);
    }
    else
    {
        int entryValue = ResolveEntryLookupValue(self, nullptr);
        int field10 = GetLinkedObjectField10(self, nullptr);
        AiCore_UpdateLinkedTask(ToAddress(static_cast<uint32_t>(field10)), ToAddress(taskObjectAddress),
                                entryValue, a2, static_cast<char>(1));
    }
}

HOOK(0x4B17C0, AiFncMacroOrder_DispatchOrder, "auto");
