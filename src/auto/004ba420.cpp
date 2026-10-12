#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, AiCore_GetTaskTargetNameBuffer, 0x0047AE10, (S_0047ae10* self, void*));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, AiDistracted_UpdateLinkedState, 0x004B3430, (void* self, void* edx));
CSF_FUNC(void, __stdcall, AiDistracted_UpdateResolvedEntry, 0x004B3D30, (int arg0, float a3, float a4));
CSF_FUNC(int, __stdcall, AiCore_ActivateAssignedCoverState, 0x004BA3C0, (void* a1, char a2));
CSF_FUNC(int, __fastcall, ResetState, 0x004D16F0, (void* self, void*));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);

static void __stdcall AiDistracted_UpdateCharacterDistraction(
    C_0085e760* task, float elapsedTime)
{
    int elapsedTimeBits;
    std::memcpy(&elapsedTimeBits, &elapsedTime, sizeof(elapsedTimeBits));
    AiCoverAttackTasks_UpdateCharacterTasks(
        reinterpret_cast<int>(task), elapsedTimeBits);

    if (static_cast<std::uint8_t>(IsField10Null(task, nullptr)) != 0)
        return;

    void* entry = reinterpret_cast<void*>(ResolveEntryLookupValue(task, nullptr));
    GetLinkedObjectField10(task, nullptr);
    int resolvedEntry = ResolveEntryLookupValue(entry, nullptr);
    GetLinkedObjectField10(entry, nullptr);
    AiDistracted_UpdateResolvedEntry(resolvedEntry, elapsedTime, elapsedTime);

    int linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    void* character = linkedValue
        ? reinterpret_cast<void*>(static_cast<std::uint32_t>(linkedValue) - 4)
        : nullptr;
    AiDistracted_UpdateLinkedState(character, nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    character = linkedValue
        ? reinterpret_cast<void*>(static_cast<std::uint32_t>(linkedValue) - 4)
        : nullptr;

    if (CSF_AT(int, character, 0x608) != -1 ||
        CSF_AT(int, character, 0x60C) != -1)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
        character = linkedValue
            ? reinterpret_cast<void*>(static_cast<std::uint32_t>(linkedValue) - 4)
            : nullptr;

        if ((CSF_AT(std::uint8_t, character, 0x680) & 1) == 0)
        {
            GetLinkedObjectValue_4DB5C0(task, nullptr);
            int linkedState = GetLinkedObjectField10(task, nullptr);
            AiCoverAttackTasks_ResetLinkedTaskState(task, nullptr, linkedState + 4);

            entry = reinterpret_cast<void*>(ResolveEntryLookupValue(task, nullptr));
            void* provider = GetCachedObjectProvider(entry, nullptr);
            GetLinkedObjectField10(task, nullptr);
            AiCore_ActivateAssignedCoverState(provider, 1);
        }
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    void* currentCharacter = linkedValue
        ? reinterpret_cast<void*>(static_cast<std::uint32_t>(linkedValue) - 4)
        : nullptr;

    linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
    void* referenceCharacter = linkedValue
        ? reinterpret_cast<void*>(static_cast<std::uint32_t>(linkedValue) - 4)
        : nullptr;

    if (CSF_AT(float, referenceCharacter, 0x698) * g_ValueScale >
        CSF_AT(float, currentCharacter, 0x694))
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(task, nullptr);
        character = linkedValue
            ? reinterpret_cast<void*>(static_cast<std::uint32_t>(linkedValue) - 4)
            : nullptr;

        // Retain the packet's additional vtable bindings for these callees.
        (void)CSF_VTBL(0x0047AE10);
        char* targetName = AiCore_GetTaskTargetNameBuffer(
            CSF_AT(S_0047ae10*, character, 0x718), nullptr);
        ResetState(targetName, nullptr);

        void* owner = CSF_AT(void*, character, 0x2C);
        alignas(8) std::uint8_t targetHandleBytes[0xC];
        CSF_AT(void**, targetHandleBytes, 0) = CSF_VTBL(0x0082D448);
        CSF_AT(int, targetHandleBytes, 4) = -1;
        CSF_AT(int, targetHandleBytes, 8) = -1;

        (void)CSF_VTBL(0x0048C9F0);
        AiAttackTasks_CopyTargetHandle(
            CSF_AT(void*, owner, 0x204), nullptr,
            reinterpret_cast<int>(targetHandleBytes));

        int linkedState = GetLinkedObjectField10(task, nullptr);
        AiCoverAttackTasks_ResetLinkedTaskState(task, nullptr, linkedState + 16);
    }
}

HOOK(0x004BA420, AiDistracted_UpdateCharacterDistraction, "auto");
