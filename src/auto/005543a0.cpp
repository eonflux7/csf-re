#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharActionFsm_ReleaseThreeOwnedObjects, 0x00482A00, (void* self, void*));
CSF_FUNC(std::uint32_t, __fastcall, CCharActionFsm_RemoveActionAtIndex, 0x00486CC0, (CCharActionFsm* self, void* , int index));
CSF_FUNC(int, __fastcall, C_00495140_ForwardInnerObjectVirtualCall, 0x00495140, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_558150, 0x00558150, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_FindLoadoutEntries, 0x005586C0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int16_t, __fastcall, MpSpawnPoints_GetLoadoutThreshold, 0x00740CB0, (void* self, void* edx, int a2));

static void* MpSpawnPoints_GetActionFsm(void* self)
{
    return CSF_AT(void*, CSF_AT(void*, self, 0xC8C), 0x44);
}

static char __fastcall MpSpawnPoints_RebuildSpawnLoadout(void* self, void* /*edx*/)
{
    void* character = CSF_AT(void*, self, 0xC8C);
    if (!character)
        return 0;

    auto* characterBase = CSF_AT(std::uint8_t*, character, 0x18);
    void* owner = characterBase ? characterBase - 1712 : nullptr;
    if (!CSF_AT(std::int8_t, owner, 0x443))
        return static_cast<char>(reinterpret_cast<std::uintptr_t>(character));

    void* fsm = CSF_AT(void*, character, 0x44);
    int count = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, fsm, 0)[0xA0 / 4])(fsm, nullptr);
    while (count > 0)
    {
        fsm = MpSpawnPoints_GetActionFsm(self);
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, fsm, 0)[0xB4 / 4])(fsm, nullptr, 0);
        --count;
    }

    auto* actionFsm = static_cast<CCharActionFsm*>(MpSpawnPoints_GetActionFsm(self));
    const std::uint32_t begin = actionFsm->field_0078;
    count = begin
        ? (static_cast<std::int32_t>(actionFsm->field_007c - begin) >> 2)
        : 0;
    while (count > 0)
    {
        CCharActionFsm_RemoveActionAtIndex(
            static_cast<CCharActionFsm*>(MpSpawnPoints_GetActionFsm(self)), nullptr, 0);
        --count;
    }

    CharActionFsm_ReleaseThreeOwnedObjects(MpSpawnPoints_GetActionFsm(self), nullptr);
    void* provider = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC0);

    for (unsigned int i = 0; i < 3; ++i)
    {
        void* definitions = reinterpret_cast<void*>(GetGlobalValue_558150());
        int baseIndex = 3 * CSF_AT(std::int8_t, owner, 0x443)
                      + CSF_AT(int, self, 0x13CC) + 23;
        int definition = CSF_AT(int, definitions, 4 * (baseIndex + i + 2 * baseIndex));
        int item = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, provider, 0)[0xC / 4])(provider, nullptr, definition);
        if (item != -1)
        {
            fsm = MpSpawnPoints_GetActionFsm(self);
            reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, fsm, 0)[0xA8 / 4])(fsm, nullptr, item, 0);
        }
    }

    fsm = MpSpawnPoints_GetActionFsm(self);
    reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, fsm, 0)[0x104 / 4])(fsm, nullptr, 0);

    int specialItem = CSF_AT(int, self, 0x13D4);
    if (specialItem != -1)
    {
        fsm = MpSpawnPoints_GetActionFsm(self);
        reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, fsm, 0)[0xA8 / 4])(fsm, nullptr, specialItem, 0);

        fsm = MpSpawnPoints_GetActionFsm(self);
        void** savedVtable = CSF_AT(void**, fsm, 0);
        int index = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            savedVtable[0xC0 / 4])(fsm, nullptr, CSF_AT(int, self, 0x13D4));
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            savedVtable[0xF8 / 4])(fsm, nullptr, index);
    }

    fsm = MpSpawnPoints_GetActionFsm(self);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, fsm, 0)[0x94 / 4])(fsm, nullptr, 0);
    C_00495140_ForwardInnerObjectVirtualCall(CSF_AT(void*, self, 0xC8C), nullptr);

    int category = CSF_AT(std::int8_t, owner, 0x443);
    int spawnType = CSF_AT(int, CSF_AT(void*, self, 0xC8C), 0x54);
    void* definitions = reinterpret_cast<void*>(GetGlobalValue_558150());
    void* entries = reinterpret_cast<void*>(
        MpSpawnPoints_FindLoadoutEntries(definitions, nullptr, category, spawnType));
    if (!entries)
        return 0;

    int threshold = static_cast<std::uint16_t>(
        MpSpawnPoints_GetLoadoutThreshold(owner, nullptr, 0));
    std::uint32_t entriesBegin = CSF_AT(std::uint32_t, entries, 4);
    int entryIndex = entriesBegin
        ? (static_cast<std::int32_t>(CSF_AT(std::uint32_t, entries, 8) - entriesBegin) >> 3) - 1
        : -1;
    int selectedThreshold = -1;

    if (entryIndex < 0)
        return static_cast<char>(entriesBegin);

    while (entryIndex >= 0)
    {
        auto* entry = CSF_AT(std::uint8_t*, entries, 4) + 8 * entryIndex;
        if (selectedThreshold == -1)
        {
            if (threshold < CSF_AT(int, entry, 0))
            {
                --entryIndex;
                continue;
            }
            selectedThreshold = CSF_AT(int, entry, 0);
        }
        else if (selectedThreshold != CSF_AT(int, entry, 0))
        {
            return 1;
        }

        int item = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, provider, 0)[0xC / 4])(
                provider, nullptr, CSF_AT(int, entry, 4));
        if (item != -1)
        {
            fsm = MpSpawnPoints_GetActionFsm(self);
            reinterpret_cast<void (__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, fsm, 0)[0xA8 / 4])(fsm, nullptr, item, 0);
        }
        --entryIndex;
    }
    return 0;
}

HOOK(0x005543A0, MpSpawnPoints_RebuildSpawnLoadout, "auto");
