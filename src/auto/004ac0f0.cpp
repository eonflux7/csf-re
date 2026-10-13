#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, TaskCharAction_48CFE0, 0x0048CFE0, (void* self, void* edx, int a2, char a3, char a4));

static int __stdcall C_0085e7b8_IssueCharActionForProvider(C_0085e7b8* self)
{
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    int v2 = linkedValue ? linkedValue - 4 : 0;
    UnnamedApply_4DC180(self, nullptr, v2 + 428);

    GetLinkedObjectValue_4DB5C0(self, nullptr);
    void* provider = GetCachedObjectProvider(self, nullptr);

    int providerLinked = GetLinkedObjectValue_4DB5C0(provider, nullptr);
    int v5 = providerLinked ? providerLinked - 4 : 0;

    int providerLinkedAgain = GetLinkedObjectValue_4DB5C0(provider, nullptr);
    int v7 = providerLinkedAgain ? providerLinkedAgain - 4 : 0;

    void* base7 = reinterpret_cast<void*>(static_cast<uintptr_t>(v7));
    uint32_t ownerAddr = CSF_AT(uint32_t, base7, 0x2C);
    void* taskChar = CSF_AT(void*, reinterpret_cast<void*>(static_cast<uintptr_t>(ownerAddr)), 0x204);

    return TaskCharAction_48CFE0(taskChar, nullptr, v5, 1, 1);
}

HOOK(0x004AC0F0, C_0085e7b8_IssueCharActionForProvider, "auto");
