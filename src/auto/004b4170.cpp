#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __stdcall, AiDistracted_UpdateResolvedEntry, 0x004B3D30, (int arg0, float a3, float a4));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_TransitionOne, 0x0082D4D4);

static void __stdcall AiAlert_UpdateCharacterAlert(C_0085e760* character, float elapsedTime)
{
    int elapsedBits;
    std::memcpy(&elapsedBits, &elapsedTime, sizeof(elapsedBits));
    AiCoverAttackTasks_UpdateCharacterTasks(reinterpret_cast<int>(character), elapsedBits);

    if (static_cast<uint8_t>(IsField10Null(character, nullptr)) != 0)
        return;

    void* resolvedEntry =
        reinterpret_cast<void*>(ResolveEntryLookupValue(character, nullptr));
    GetLinkedObjectField10(character, nullptr);

    int entryValue = ResolveEntryLookupValue(resolvedEntry, nullptr);
    GetLinkedObjectField10(resolvedEntry, nullptr);
    AiDistracted_UpdateResolvedEntry(entryValue, elapsedTime, elapsedTime);

    int linkedValue = GetLinkedObjectValue_4DB5C0(character, nullptr);
    uint8_t* linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    if (CSF_AT(int, linkedObject, 0x608) == -1 &&
        CSF_AT(int, linkedObject, 0x60C) == -1)
        return;

    linkedValue = GetLinkedObjectValue_4DB5C0(character, nullptr);
    linkedObject = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    if ((CSF_AT(uint8_t, linkedObject, 0x680) & 1) != 0)
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(character, nullptr);
        linkedObject = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4
            : nullptr;

        if (CSF_AT(float, linkedObject, 0x678) > g_TransitionOne)
            return;
    }

    int linkedField = GetLinkedObjectField10(character, nullptr);
    AiCoverAttackTasks_ResetLinkedTaskState(character, nullptr, linkedField + 4);
}

HOOK(0x004B4170, AiAlert_UpdateCharacterAlert, "auto");
