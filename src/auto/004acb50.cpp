#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ForwardToAttachedObject, 0x0048E5F0, (void* self, void* edx, int a2));

static int __stdcall AiOrder_ResetLinkedStateAndGetField10(C_0085e760* self)
{
    int firstLinkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int firstTarget = 0;
    if (firstLinkedValue)
        firstTarget = firstLinkedValue - 4;

    int secondLinkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int secondBase = 0;
    if (secondLinkedValue)
        secondBase = secondLinkedValue - 4;

    void* secondBasePtr = reinterpret_cast<void*>(static_cast<uintptr_t>(secondBase));
    void* forwardOwner = CSF_AT(void*, secondBasePtr, 0x2C);
    ForwardToAttachedObject(forwardOwner, nullptr, firstTarget);

    int linkedField10 = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField10 + 4);

    int entryValue = ResolveEntryLookupValue(self, nullptr);
    GetCachedObjectProvider(reinterpret_cast<void*>(static_cast<uintptr_t>(entryValue)), nullptr);

    return GetLinkedObjectField10(self, nullptr);
}

HOOK(0x004ACB50, AiOrder_ResetLinkedStateAndGetField10, "auto");
