#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(char, __fastcall, C_0083b038_DispatchIfFlagSet, 0x004CEF50, (C_0083b038* self, void* , int a2));
CSF_FUNC(char, __fastcall, C_0083c478_UpdatePrimaryObjectState, 0x004D6870, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, C_0083c478_UpdateSecondaryObjectState, 0x004D6900, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_0083c478_UpdateTertiaryObjectState, 0x004D6980, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_0083c478_UpdateFinalObjectState, 0x004D6A10, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, ReleaseSpawnObjectReference, 0x0073F750, (void* self, void*));
CSF_FUNC(char, __fastcall, CMenuMpStats_RefreshIfReadyAndSetFlag, 0x00740080, (void* self, void*));
CSF_FUNC(char, __fastcall, NetCogibleDDL_SetSignFlagAndStoreValue, 0x007400A0, (void* self, void* , float value));
CSF_FUNC(char, __fastcall, CMenuMpStats_SetScoreFieldAndRefresh, 0x007400F0, (CMenuMpStats* self, void*));
CSF_FUNC(char, __fastcall, NetCogibleDDL_GetLazyLatchedByte, 0x00740130, (uint8_t* self, void*));
CSF_FUNC(char, __fastcall, NetCogibleDDL_SyncByteFieldIfChanged, 0x00740360, (void* self, void*));
CSF_FUNC(char, __fastcall, C_00858180_SendHasBeenCaughtCommand, 0x00742140, (C_00858180* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0083c478_UpdateSpawnedObject(
    C_0083c478* self, void* /*edx*/, int elapsed)
{
    C_0083b038_DispatchIfFlagSet(
        reinterpret_cast<C_0083b038*>(self), nullptr, elapsed);

    const uint32_t countdown = self->field_0050;
    if (countdown != static_cast<uint32_t>(-1))
        self->field_0050 = countdown - 1;

    if (!self->field_0050)
    {
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            alignas(8) uint8_t recordBytes[0x34];

            CSF_AT(void**, recordBytes, 0x08) = CSF_VTBL(0x0082D448);
            CSF_AT(void**, recordBytes, 0x14) = CSF_VTBL(0x0082D448);
            auto* owner = reinterpret_cast<C_00833908*>(self->field_0014);
            CSF_AT(int, recordBytes, 0x0C) = -1;
            CSF_AT(int, recordBytes, 0x10) = -1;
            CSF_AT(void**, recordBytes, 0x00) = CSF_VTBL(0x0082D478);
            CSF_AT(int, recordBytes, 0x18) = -1;
            CSF_AT(int, recordBytes, 0x1C) = -1;
            CSF_AT(int, recordBytes, 0x04) = 20;
            CSF_AT(int, recordBytes, 0x20) = 0;
            CSF_AT(int, recordBytes, 0x24) = 0;
            CSF_AT(int, recordBytes, 0x2C) = 0;
            CSF_AT(int, recordBytes, 0x28) = 0;
            CSF_AT(uint8_t, recordBytes, 0x30) = 0;
            CSF_AT(uint32_t, recordBytes, 0x0C) = owner->field_0020;
            CSF_AT(uint32_t, recordBytes, 0x10) = owner->field_0024;
            if (owner->field_0050 == 11)
            {
                CSF_AT(uint32_t, recordBytes, 0x18) = CSF_AT(uint32_t, owner, 0x18C);
                CSF_AT(uint32_t, recordBytes, 0x1C) = CSF_AT(uint32_t, owner, 0x190);
            }

            void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
            DispatchRecordToGlobalObject(dispatcher, nullptr, recordBytes, 1);

            owner = reinterpret_cast<C_00833908*>(self->field_0014);
            const uint32_t link = CSF_AT(uint32_t, owner, 0x18);
            void* object = link ? reinterpret_cast<void*>(link - 120) : nullptr;
            float value;
            std::memcpy(&value, &self->field_0058, sizeof(value));
            NetCogibleDDL_SetSignFlagAndStoreValue(object, nullptr, value);
        }
        self->field_0050 = static_cast<uint32_t>(-1);
    }

    char result = static_cast<char>(self->field_004c);
    if (!result)
    {
        auto* owner = reinterpret_cast<C_00833908*>(self->field_0014);
        if (CSF_AT(uint32_t, owner, 0x18))
        {
            self->field_004c = 1;
            self->field_0050 = static_cast<uint32_t>(-1);
            const uint32_t link = CSF_AT(uint32_t, owner, 0x18);
            auto* object = link ? reinterpret_cast<uint8_t*>(link - 120) : nullptr;
            if (CSF_AT(uint8_t, object, 0x6F) & 2)
            {
                owner->vSlot6C(0);
                CSF_AT(uint8_t, object, 0xA0) = 0;
            }
        }
        result = static_cast<char>(self->field_004c);
    }

    if (!result)
        return result;

    auto* owner = reinterpret_cast<C_00833908*>(self->field_0014);
    const uint32_t link = CSF_AT(uint32_t, owner, 0x18);
    if (!link || !(link - 120))
    {
        return static_cast<char>(CScene_RemoveObjectSlot(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
            reinterpret_cast<int>(&owner->field_001c)));
    }

    auto* object = reinterpret_cast<uint8_t*>(link - 120);
    void* networkBase = object + 120;
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, networkBase, 0)[0x0C / 4])(
            networkBase, nullptr, elapsed);

    if (self->field_005c)
    {
        result = NetCogibleDDL_SyncByteFieldIfChanged(object, nullptr);
        if (result)
        {
            owner = reinterpret_cast<C_00833908*>(self->field_0014);
            // This path returns the virtual call's low byte, unlike
            // the void wrapper used when its result is discarded.
            result = static_cast<char>(
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    owner->vtbl[0x6C / 4])(owner, nullptr, 1));
            self->field_0040 = 0;
            self->field_005c = 0;
        }
        return result;
    }

    C_0083c478_UpdatePrimaryObjectState(
        self, nullptr, reinterpret_cast<uint32_t*>(object));
    C_0083c478_UpdateSecondaryObjectState(
        self, nullptr, reinterpret_cast<int>(object));
    C_0083c478_UpdateTertiaryObjectState(
        self, nullptr, reinterpret_cast<int>(object));
    C_0083c478_UpdateFinalObjectState(
        self, nullptr, reinterpret_cast<int>(object));

    if (self->field_0040 && !self->field_0056)
    {
        if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, networkBase, 0)[0x08 / 4])(networkBase, nullptr))
        {
            CMenuMpStats_RefreshIfReadyAndSetFlag(object, nullptr);
        }
        else
        {
            C_00858180_SendHasBeenCaughtCommand(
                reinterpret_cast<C_00858180*>(object), nullptr);
        }
        self->field_0056 = 1;
    }

    result = static_cast<char>(CSF_AT(uint8_t, object, 0xA0));
    CSF_AT(uint8_t, object, 0xA0) = 0;
    if (!result)
        return result;

    if (CSF_AT(uint8_t, object, 0x6F) & 1)
    {
        if (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, networkBase, 0)[0x08 / 4])(networkBase, nullptr))
        {
            CMenuMpStats_SetScoreFieldAndRefresh(
                reinterpret_cast<CMenuMpStats*>(object), nullptr);
        }
        else
        {
            NetCogibleDDL_GetLazyLatchedByte(object, nullptr);
        }
        reinterpret_cast<C_00833908*>(self->field_0014)->vSlot6C(0);
    }
    else
    {
        owner = reinterpret_cast<C_00833908*>(self->field_0014);
        CScene_RemoveObjectSlot(
            reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
            reinterpret_cast<int>(&owner->field_001c));
        owner = reinterpret_cast<C_00833908*>(self->field_0014);
        if (CSF_AT(uint32_t, owner, 0x18))
            ReleaseSpawnObjectReference(&owner->field_0014, nullptr);
    }

    self->field_0044 = static_cast<uint32_t>(-1);
    self->field_0048 = static_cast<uint32_t>(-1);
    self->field_0056 = 0;
    self->field_005c = 1;
    return -1;
}

HOOK(0x004D6B70, C_0083c478_UpdateSpawnedObject, "auto");
