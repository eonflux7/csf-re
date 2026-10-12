#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, AiAttackTasks_IsWeaponActionAvailable, 0x0048D030, (void* self, void* edx, char a2));
CSF_FUNC(int, __stdcall, AiAlert_StartWeaponAction, 0x004AB990, (int a1, int a2));
CSF_FUNC(float, __fastcall, TEstatico_ScaleByGlobalIfHandleTargetValid, 0x004AF160, (void* self, void* , float value));
CSF_FUNC(void, __fastcall, AiAlert_UpdateLinkedState, 0x004B3430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AiAlert_UpdateWeaponAlert, 0x004B3700, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, AiCoverAttackTasks_UpdateCoverState, 0x004D1120, (float* self, void* , float a2));
CSF_FUNC(int, __fastcall, IsField10Null, 0x004DB5B0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __stdcall, AiCoverAttackTasks_UpdateCharacterTasks, 0x004DBD50, (int a1, int a2));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));

static int __fastcall AiAlert_UpdateCharacterAlert(
    void* self, void* /*edx*/, C_0085e7b8* character, float elapsedTime)
{
    int elapsedTimeBits;
    std::memcpy(&elapsedTimeBits, &elapsedTime, sizeof(elapsedTimeBits));
    AiCoverAttackTasks_UpdateCharacterTasks(
        reinterpret_cast<int>(character), elapsedTimeBits);

    const int nullResult = IsField10Null(character, nullptr);
    if (static_cast<std::uint8_t>(nullResult))
        return nullResult;

    std::uintptr_t linked =
        static_cast<std::uint32_t>(GetLinkedObjectValue_4DB5C0(character, nullptr));
    std::uintptr_t linkedBase = linked ? linked - 4 : 0;

    float coverTime;
    if (CSF_AT(std::uint8_t, reinterpret_cast<void*>(linkedBase), 0x684))
    {
        linked = static_cast<std::uint32_t>(
            GetLinkedObjectValue_4DB5C0(character, nullptr));
        linkedBase = linked ? linked - 4 : 0;
        coverTime = TEstatico_ScaleByGlobalIfHandleTargetValid(
            reinterpret_cast<void*>(linkedBase), nullptr, elapsedTime);
    }
    else
    {
        coverTime = -elapsedTime;
    }

    linked = static_cast<std::uint32_t>(
        GetLinkedObjectValue_4DB5C0(character, nullptr));
    linkedBase = linked ? linked - 4 : 0;
    AiCoverAttackTasks_UpdateCoverState(
        reinterpret_cast<float*>(linkedBase + 0x690), nullptr, coverTime);

    if (GetCachedObjectProvider(character, nullptr))
    {
        void* provider = GetCachedObjectProvider(character, nullptr);
        void* collection = reinterpret_cast<void*>(
            SceneLoader_GetWorldObjectCollection(provider, nullptr));
        return ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, collection, 0)[0x1C / 4])(collection, nullptr);
    }

    if (CSF_AT(std::uint8_t, self, 0x4))
    {
        linked = static_cast<std::uint32_t>(
            GetLinkedObjectValue_4DB5C0(character, nullptr));
        linkedBase = linked ? linked - 4 : 0;
        AiAlert_UpdateLinkedState(reinterpret_cast<void*>(linkedBase), nullptr);
    }

    linked = static_cast<std::uint32_t>(
        GetLinkedObjectValue_4DB5C0(character, nullptr));
    linkedBase = linked ? linked - 4 : 0;
    void* taskOwner = CSF_AT(void*, reinterpret_cast<void*>(linkedBase), 0x2C);
    if (AiAttackTasks_IsWeaponActionAvailable(
            CSF_AT(void*, taskOwner, 0x204), nullptr, 1))
    {
        void* entry = reinterpret_cast<void*>(
            ResolveEntryLookupValue(character, nullptr));
        GetLinkedObjectField10(character, nullptr);
        const int lookupValue = ResolveEntryLookupValue(entry, nullptr);
        void* linkedField = reinterpret_cast<void*>(
            GetLinkedObjectField10(entry, nullptr));
        return AiAlert_UpdateWeaponAlert(linkedField, nullptr, lookupValue);
    }

    linked = static_cast<std::uint32_t>(
        GetLinkedObjectValue_4DB5C0(character, nullptr));
    linkedBase = linked ? linked - 4 : 0;
    UnnamedApply_4DC180(character, nullptr, static_cast<int>(linkedBase + 0x1AC));
    GetLinkedObjectValue_4DB5C0(character, nullptr);
    void* provider = GetCachedObjectProvider(character, nullptr);
    return AiAlert_StartWeaponAction(reinterpret_cast<int>(provider), 0);
}

HOOK(0x004B38D0, AiAlert_UpdateCharacterAlert, "auto");
