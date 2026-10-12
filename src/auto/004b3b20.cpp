#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, AiAttackTasks_IsWeaponActionAvailable, 0x0048D030, (void* self, void* edx, char a2));
CSF_FUNC(float, __fastcall, TEstatico_ScaleByGlobalIfHandleTargetValid, 0x004AF160, (void* self, void* , float value));
CSF_FUNC(void, __fastcall, AiDistracted_UpdateLinkedState, 0x004B3430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAlert_UpdateLinkedActionState, 0x004B3700, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (float* self, void* , float a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));

static char __stdcall AiAlert_UpdateLinkedCharacterState(
    C_0085e760* entry, float delta)
{
    int deltaBits;
    std::memcpy(&deltaBits, &delta, sizeof(deltaBits));
    AiCoverAttackTasks_UpdateCharacterTasks(
        reinterpret_cast<int>(entry), deltaBits);

    char result = static_cast<char>(IsField10Null(entry, nullptr));
    if (result)
        return result;

    int linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
    uint8_t* character = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;

    float coverDelta;
    if (CSF_AT(uint8_t, character, 0x684))
    {
        linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
        character = linkedValue
            ? reinterpret_cast<uint8_t*>(linkedValue) - 4
            : nullptr;
        coverDelta = TEstatico_ScaleByGlobalIfHandleTargetValid(
            character, nullptr, delta);
    }
    else
    {
        coverDelta = -delta;
    }

    linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
    character = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    AiCoverAttackTasks_UpdateCoverState(
        reinterpret_cast<float*>(
            reinterpret_cast<uintptr_t>(character) + 0x690),
        nullptr, coverDelta);

    linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
    character = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    AiDistracted_UpdateLinkedState(character, nullptr);

    linkedValue = GetLinkedObjectValue_4DB5C0(entry, nullptr);
    character = linkedValue
        ? reinterpret_cast<uint8_t*>(linkedValue) - 4
        : nullptr;
    void* characterState = CSF_AT(void*, character, 0x2C);
    result = AiAttackTasks_IsWeaponActionAvailable(
        CSF_AT(void*, characterState, 0x204), nullptr, 1);

    if (result)
    {
        auto* relatedEntry = reinterpret_cast<C_0085e760*>(
            ResolveEntryLookupValue(entry, nullptr));
        GetLinkedObjectField10(entry, nullptr);
        int lookupValue = ResolveEntryLookupValue(relatedEntry, nullptr);
        void* linkedField = reinterpret_cast<void*>(
            GetLinkedObjectField10(relatedEntry, nullptr));
        return static_cast<char>(
            AiAlert_UpdateLinkedActionState(linkedField, nullptr, lookupValue));
    }

    return result;
}

HOOK(0x004B3B20, AiAlert_UpdateLinkedCharacterState, "auto");
