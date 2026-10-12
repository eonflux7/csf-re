#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(int, __stdcall, AiCore_ActivateAssignedCoverState, 0x004BA3C0, (void* a1, char a2));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (float* self, void* , float a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __stdcall AiDistracted_UpdateAssignedCoverState(
    C_0085e760* self, float deltaTime)
{
    int deltaTimeBits;
    std::memcpy(&deltaTimeBits, &deltaTime, sizeof(deltaTimeBits));
    AiCoverAttackTasks_UpdateCharacterTasks(
        reinterpret_cast<int>(self), deltaTimeBits);

    int result = IsField10Null(self, nullptr);
    if (static_cast<std::uint8_t>(result) != 0)
        return result;

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    result = linkedValue ? linkedValue - 4 : 0;
    AiCoverAttackTasks_UpdateCoverState(
        reinterpret_cast<float*>(static_cast<std::uintptr_t>(result) + 0x690),
        nullptr, deltaTime);

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    result = linkedValue ? linkedValue - 4 : 0;
    void* linkedObject = reinterpret_cast<void*>(result);

    if (CSF_AT(int, linkedObject, 0x608) != -1 ||
        CSF_AT(int, linkedObject, 0x60C) != -1)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        result = linkedValue ? linkedValue - 4 : 0;
        linkedObject = reinterpret_cast<void*>(result);

        if ((CSF_AT(std::uint8_t, linkedObject, 0x680) & 1) == 0)
        {
            int linkedTask = GetLinkedObjectField10(self, nullptr);
            AiCoverAttackTasks_ResetLinkedTaskState(
                self, nullptr, linkedTask + 4);

            int entry = ResolveEntryLookupValue(self, nullptr);
            void* provider = GetCachedObjectProvider(
                reinterpret_cast<void*>(entry), nullptr);

            GetLinkedObjectField10(self, nullptr);
            return AiCore_ActivateAssignedCoverState(provider, 1);
        }
    }

    return result;
}

HOOK(0x004BA000, AiDistracted_UpdateAssignedCoverState, "auto");
