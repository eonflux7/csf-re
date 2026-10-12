#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, AiAlert_UpdateLinkedState, 0x004B3430, (void* self, void* edx));
CSF_FUNC(void, __stdcall, AiAlert_UpdateResolvedEntry, 0x004B3D30, (int arg0, float a3, float a4));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);

static void __stdcall C_0085e760_UpdateDistractedAlert(C_0085e760* self, float deltaTime)
{
    void* resolvedEntry = reinterpret_cast<void*>(ResolveEntryLookupValue(self, nullptr));
    GetLinkedObjectField10(self, nullptr);

    int nestedEntry = ResolveEntryLookupValue(resolvedEntry, nullptr);
    GetLinkedObjectField10(resolvedEntry, nullptr);
    AiAlert_UpdateResolvedEntry(nestedEntry, deltaTime, deltaTime);

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint8_t* linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    bool resetLinkedTask = CSF_AT(uint8_t, linkedObject, 0x684) != 0;
    if (!resetLinkedTask)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        linkedObject = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4
            : nullptr;

        if (CSF_AT(int, linkedObject, 0x608) != -1 ||
            CSF_AT(int, linkedObject, 0x60C) != -1)
        {
            linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
            linkedObject = linkedValue
                ? reinterpret_cast<uint8_t*>(linkedValue) - 4
                : nullptr;
            resetLinkedTask = (CSF_AT(uint8_t, linkedObject, 0x680) & 1) == 0;
        }
    }

    if (resetLinkedTask)
    {
        int linkedState = GetLinkedObjectField10(self, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedState + 4);
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    AiAlert_UpdateLinkedState(linkedObject, nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint8_t* comparisonObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    if (CSF_AT(float, linkedObject, 0x698) * g_ValueScale >
        CSF_AT(float, comparisonObject, 0x694))
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        linkedObject = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4
            : nullptr;

        // Additional address bindings listed as vtables in the packet.
        (void)CSF_VTBL(0x0047AE10);
        char* targetName = AiCore_GetTaskTargetNameBuffer(
            CSF_AT(S_0047ae10*, linkedObject, 0x718), nullptr);
        ResetState(targetName, nullptr);

        void* owner = CSF_AT(void*, linkedObject, 0x2C);
        alignas(8) uint8_t targetHandleBytes[0xC];
        CSF_AT(void**, targetHandleBytes, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(int, targetHandleBytes, 4) = -1;
        CSF_AT(int, targetHandleBytes, 8) = -1;

        (void)CSF_VTBL(0x0048C9F0);
        AiAttackTasks_CopyTargetHandle(
            CSF_AT(void*, owner, 0x204), nullptr,
            reinterpret_cast<int>(targetHandleBytes));

        int linkedState = GetLinkedObjectField10(self, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedState + 16);
    }
}

HOOK(0x004B4010, C_0085e760_UpdateDistractedAlert, "auto");
