#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __stdcall, AiDistracted_UpdateLinkedCharacter, 0x004B3D30, (int arg0, float a3, float a4));
CSF_FUNC(int, __stdcall, AiCore_ActivateAssignedCoverState, 0x004BA3C0, (void* a1, char a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static void __stdcall C_0085e760_UpdateDistractedTask(C_0085e760* self, float deltaTime)
{
    int deltaTimeBits;
    std::memcpy(&deltaTimeBits, &deltaTime, sizeof(deltaTimeBits));
    AiCoverAttackTasks_UpdateCharacterTasks(reinterpret_cast<int>(self), deltaTimeBits);

    if (static_cast<uint8_t>(IsField10Null(self, nullptr)) != 0)
        return;

    void* linkedEntry = reinterpret_cast<void*>(ResolveEntryLookupValue(self, nullptr));
    GetLinkedObjectField10(self, nullptr);

    int linkedCharacter = ResolveEntryLookupValue(linkedEntry, nullptr);
    GetLinkedObjectField10(linkedEntry, nullptr);
    AiDistracted_UpdateLinkedCharacter(linkedCharacter, deltaTime, deltaTime);

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    void* linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);

    if (CSF_AT(int, linkedObject, 0x608) == -1 &&
        CSF_AT(int, linkedObject, 0x60C) == -1)
        return;

    linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);

    if ((CSF_AT(uint8_t, linkedObject, 0x680) & 1) != 0)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        linkedObject = reinterpret_cast<void*>(linkedValue ? linkedValue - 4 : 0);
        if (CSF_AT(float, linkedObject, 0x678) > g_TransitionOne)
            return;
    }

    int linkedState = GetLinkedObjectField10(self, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedState + 4);

    void* entry = reinterpret_cast<void*>(ResolveEntryLookupValue(self, nullptr));
    void* provider = GetCachedObjectProvider(entry, nullptr);
    GetLinkedObjectField10(self, nullptr);
    AiCore_ActivateAssignedCoverState(provider, 1);
}

HOOK(0x004BA5A0, C_0085e760_UpdateDistractedTask, "auto");
