#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static uint8_t* AsBytePtr(int value) {
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(value));
}

static void* AsVoidPtr(int value) {
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

static int __stdcall AiOrder_ClearLinkedTaskFlagsAndReset(C_0085e760* a1, int a2) {
    (void)a2;

    GetLinkedObjectValue_4DB5C0(a1, nullptr);
    int entryValue = ResolveEntryLookupValue(a1, nullptr);

    if (CSF_AT(uint8_t, AsBytePtr(GetLinkedObjectField10(a1, nullptr)), 0xB0)) {
        int linkedValue = GetLinkedObjectValue_4DB5C0(AsVoidPtr(entryValue), nullptr);
        int adjusted = linkedValue ? linkedValue - 4 : 0;
        uint8_t* adjustedBase = AsBytePtr(adjusted);
        void* linkedObj = CSF_AT(void*, adjustedBase, 0x718);
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, linkedObj, 0)[40])(linkedObj, nullptr);
    }

    CSF_AT(uint8_t, AsBytePtr(GetLinkedObjectField10(a1, nullptr)), 0xB0) = 0;
    CSF_AT(uint8_t, AsBytePtr(GetLinkedObjectField10(a1, nullptr)), 0xB1) = 0;

    int linkedField10 = GetLinkedObjectField10(a1, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(a1, nullptr, linkedField10 + 4);

    int providerEntry = ResolveEntryLookupValue(a1, nullptr);
    auto* v6 = reinterpret_cast<C_0085e7b8*>(static_cast<uintptr_t>(providerEntry));
    GetCachedObjectProvider(v6, nullptr);

    return GetLinkedObjectField10(a1, nullptr);
}

HOOK(0x4ACD50, AiOrder_ClearLinkedTaskFlagsAndReset, "auto");
