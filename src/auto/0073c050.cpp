#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(char, __fastcall, NetDdl_AssignEventContext, 0x0064E4F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_CopyEventRecord, 0x0073BDE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, NetCogibleDDL_CopyRecordBlock, 0x0073FA40, (void* self, void* , void* src));
CSF_FUNC(int, __fastcall, NetCogibleDDL_CopyRecord, 0x007403C0, (void* self, void* , void* source));
CSF_FUNC(uint16_t*, __fastcall, NetCogibleDDL_ForwardToInnerSubobject, 0x007411B0, (char* self, void* , char* a2));
CSF_FUNC(int, __cdecl, NetCogibleDdl_CallWithContext, 0x00742060, (int a1));
CSF_FUNC(int16_t, __fastcall, NetCallbacks_CopyRecordFromSource, 0x007435C0, (void* self, void* , const void* src));
CSF_FUNC(char, __cdecl, OptionsFile_ApplyFlagChange, 0x00743960, (char flagValue));
CSF_FUNC(void, __cdecl, NetCallbacks_InitializeSpawnObjects, 0x00743BA0, (int objectAddress));
CSF_FUNC(int, __cdecl, NetPlayerDdl_CallWithContext, 0x00744630, (int argument));
CSF_FUNC(int, __cdecl, NetCogibleDdl_CreateRecordBlock, 0x007474E0, (uint32_t a1));
CSF_FUNC(int, __fastcall, MpSpawnPoints_ApplySelection, 0x00747F00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, MpSpawnPoints_RefreshSelection, 0x007480A0, (C_008310b8* self, void*));
CSF_FUNC(int, __cdecl, MpSpawnPoints_CreateRecord, 0x0074B3A0, (uint32_t a1));
CSF_FUNC(int, __cdecl, NetGameDDL_DispatchOrRaiseError, 0x0074C520, (int a1));
CSF_FUNC(int, __cdecl, NetDdl_CreateSelectionEvent, 0x0074E6C0, (uint32_t* a1));
CSF_FUNC(int, __cdecl, NetDdl_CreateAuxiliaryEvent, 0x0074F340, (uint32_t a1));
CSF_FUNC(int, __cdecl, NetCallbacks_CreateRecord, 0x0074F7A0, (uint32_t a1));
CSF_FUNC(int, __cdecl, NetDdl_CreatePlayerEvent, 0x00750C90, (uint32_t* a1));

CSF_GLOBAL(uint8_t, g_AccountConnectionFlag, 0x008BBF98);

static uint32_t* __fastcall MpSpawnPoints_CreateEvent(
    void* self, void* /*edx*/, int eventType, int sourceAddress)
{
    if (CSF_AT(int, self, 0x98) != 6 &&
        eventType != 1 && eventType != 2 && eventType != 3)
        return nullptr;

    void* source = reinterpret_cast<void*>(sourceAddress);

    switch (eventType)
    {
    case 0:
    {
        int playerAddress = NetPlayerDdl_CallWithContext(-1);
        CSF_AT(int, self, 0xD4) = playerAddress;
        if (!playerAddress)
            break;

        NetCogibleDDL_ForwardToInnerSubobject(
            reinterpret_cast<char*>(playerAddress), nullptr,
            reinterpret_cast<char*>(source));
        NetDdl_AssignEventContext(
            reinterpret_cast<void*>(CSF_AT(int, self, 0xD4)), nullptr, -1);

        playerAddress = CSF_AT(int, self, 0xD4);
        uint32_t* result = playerAddress
            ? reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(playerAddress) + 0x6B0)
            : nullptr;

        NetCallbacks_InitializeSpawnObjects(playerAddress);
        void* globalObject = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        void* manager = CSF_AT(void*, globalObject, 0x18);
        void* options = CSF_AT(void*, manager, 0x10);
        bool flag = (reinterpret_cast<bool(__fastcall*)(void*, void*)>(
            CSF_AT(void**, options, 0)[0x60 / 4]))(options, nullptr);
        OptionsFile_ApplyFlagChange(flag == 0);
        return result;
    }

    case 1:
    {
        auto* event = reinterpret_cast<C_00858180*>(
            NetDdl_CreatePlayerEvent(
                reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BC194))));
        if (!event)
            break;
        NetDdl_AssignEventContext(event, nullptr, -1);
        return reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(event) + 0x440);
    }

    case 2:
    {
        auto* event = reinterpret_cast<C_008310b8*>(
            NetGameDDL_DispatchOrRaiseError(
                static_cast<int>(CSF_ADDR_DATA(0x008BC1A0))));
        if (!event)
            break;
        MpSpawnPoints_ApplySelection(event, nullptr, sourceAddress);
        MpSpawnPoints_RefreshSelection(event, nullptr);
        NetDdl_AssignEventContext(event, nullptr, -1);
        return reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(event) + 0x308);
    }

    case 3:
    {
        auto* event = reinterpret_cast<C_00858180*>(
            NetDdl_CreateSelectionEvent(
                reinterpret_cast<uint32_t*>(CSF_ADDR_DATA(0x008BC1AC))));
        if (!event)
            break;
        NetDdl_AssignEventContext(event, nullptr, -1);
        return reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(event) + 0xD8);
    }

    case 4:
    {
        auto* event = reinterpret_cast<C_00858180*>(
            NetDdl_CreateAuxiliaryEvent(-1));
        if (!event)
            break;
        NetDdl_AssignEventContext(event, nullptr, -1);
        return reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(event) + 0x154);
    }

    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    {
        auto* event = reinterpret_cast<C_00858180*>(
            NetCogibleDdl_CreateRecordBlock(-1));
        if (!event)
            break;
        NetCogibleDDL_CopyRecordBlock(event, nullptr, source);
        NetDdl_AssignEventContext(event, nullptr, -1);
        return reinterpret_cast<uint32_t*>(&event->field_0070);
    }

    case 12:
    {
        auto* event = reinterpret_cast<C_00858180*>(
            NetCogibleDdl_CallWithContext(-1));
        if (!event)
            break;
        NetCogibleDDL_CopyRecord(event, nullptr, source);
        NetDdl_AssignEventContext(event, nullptr, -1);
        return &event->field_0078;
    }

    case 13:
    {
        auto* event = reinterpret_cast<C_00858180*>(
            NetCallbacks_CreateRecord(-1));
        if (!event)
            break;
        NetCallbacks_CopyRecordFromSource(event, nullptr, source);

        uint32_t value = CSF_AT(uint16_t, source, 0xA);
        if (value != event->field_006c)
            event->field_006c = value;

        value = CSF_AT(uint16_t, source, 0xA);
        if (value != event->field_0078)
            event->field_0078 = value;

        value = CSF_AT(uint16_t, source, 0x8);
        if (value != event->field_0044)
            event->field_0044 = value;

        value = CSF_AT(uint16_t, source, 0x8);
        if (value != CSF_AT(uint32_t, event, 0x50))
            CSF_AT(uint32_t, event, 0x50) = value;

        NetDdl_AssignEventContext(event, nullptr, -1);
        return reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(event) + 0xB8);
    }

    case 14:
    {
        auto* event = reinterpret_cast<C_00858180*>(
            MpSpawnPoints_CreateRecord(-1));
        if (!event)
            break;
        MpSpawnPoints_CopyEventRecord(event, nullptr, sourceAddress);
        NetDdl_AssignEventContext(event, nullptr, -1);
        return &event->field_0080;
    }

    default:
        return nullptr;
    }

    g_AccountConnectionFlag = 0;
    return nullptr;
}

HOOK(0x0073C050, MpSpawnPoints_CreateEvent, "auto");
