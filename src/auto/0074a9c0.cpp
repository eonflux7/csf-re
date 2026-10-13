#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, NetSession_GetStationName, 0x0064B110, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NetDdlObjectManager_IsReferenceValid, 0x0064B310, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, NetDdlObjectManager_GetReferencedObject, 0x0064B620, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SpawnPoints_ReleaseEntry, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, NetSession_CopyObjectReference, 0x00651D30, (void* self, void* edx, int* a2));
CSF_FUNC(char, __fastcall, C_00873b14_CheckNetGameClassRegistered, 0x0073C980, (C_00873b14* self, void*));
CSF_FUNC(int, __fastcall, C_00873b14_GetVtableIfReady_73CDB0, 0x0073CDB0, (C_00873b14* self, void*));
CSF_FUNC(int, __fastcall, C_00873b14_GetVtableIfValid, 0x0073CDD0, (C_00873b14* self, void*));
CSF_FUNC(char, __fastcall, C_00873b14_CheckNetPlayerClassRegistered, 0x0073CDF0, (C_00873b14* self, void*));
CSF_FUNC(char, __fastcall, C_00873b14_CheckNetProyectilClassRegistered, 0x0073CE50, (C_00873b14* self, void*));
CSF_FUNC(void*, __fastcall, C_00873b14_CopyRefAndReturnSelf, 0x0074A750, (C_00873b14* self, void* , int* a2));
CSF_FUNC(char, __fastcall, C_00873b14_CheckDdlObjectClassSupport, 0x0074A790, (C_00873b14* self, void*));
CSF_FUNC(int, __fastcall, C_00873b14_GetVtableIfReady_74A960, 0x0074A960, (C_00873b14* self, void*));
CSF_FUNC(int, __fastcall, C_00873b14_GetVtableIfReady_74A980, 0x0074A980, (C_00873b14* self, void*));
CSF_FUNC(int, __fastcall, C_00873b14_GetVtableIfReady_74A9A0, 0x0074A9A0, (C_00873b14* self, void*));

CSF_GLOBAL(int, g_NetCogibleDdlContext, 0x008BC128);
CSF_GLOBAL(std::uint32_t, g_NetPlayerClassId, 0x008BC15C);
CSF_GLOBAL(std::uint32_t, g_NetProyectilClassId, 0x008BC160);
CSF_GLOBAL(std::uint32_t, g_NetGhostClassId, 0x008BC174);
CSF_GLOBAL(std::uint32_t, g_NetGameClassId, 0x008BC178);
CSF_GLOBAL(std::uint32_t, g_NetControlClassId, 0x008BC17C);
CSF_GLOBAL(std::uint32_t, g_DdlObjectClassId, 0x008BC180);
CSF_GLOBAL(std::uint32_t, g_RequiredObjectClassId, 0x008BC184);
CSF_GLOBAL(std::uint32_t, g_DdlObjectSupportClassId, 0x008BC1CC);

static std::uint32_t NetDdlObjectManager_GetCheckedReferenceClassId(
    C_00873b14* reference)
{
    void* object = NetDdlObjectManager_IsReferenceValid(reference, nullptr)
        ? reference->vtbl
        : nullptr;
    return CSF_AT(std::uint32_t, object, 0x2C) >> 22;
}

static void* __cdecl NetDdlObjectManager_GetSpawnRecordFromReference(
    C_00873b14* reference)
{
    alignas(8) std::uint8_t copiedReferenceBytes[0xC];
    C_00873b14& copiedReference =
        *reinterpret_cast<C_00873b14*>(copiedReferenceBytes);
    int* source = reinterpret_cast<int*>(reference);
    void* spawnRecord = nullptr;

    std::uint32_t classId =
        NetDdlObjectManager_GetCheckedReferenceClassId(reference);
    if (classId == g_NetPlayerClassId)
    {
        NetSession_CopyObjectReference(&copiedReference, nullptr, source);
        if (C_00873b14_CheckNetPlayerClassRegistered(&copiedReference, nullptr)
            && copiedReference.vtbl)
        {
            spawnRecord =
                reinterpret_cast<std::uint8_t*>(copiedReference.vtbl) + 0x6B0;
        }
        goto releaseReference;
    }

    classId = NetDdlObjectManager_GetCheckedReferenceClassId(reference);
    if (classId == g_NetProyectilClassId)
    {
        NetSession_CopyObjectReference(&copiedReference, nullptr, source);
        if (C_00873b14_CheckNetProyectilClassRegistered(&copiedReference, nullptr)
            && copiedReference.vtbl)
        {
            spawnRecord =
                reinterpret_cast<std::uint8_t*>(copiedReference.vtbl) + 0x70;
        }
        goto releaseReference;
    }

    classId = NetDdlObjectManager_GetCheckedReferenceClassId(reference);
    if (classId == g_DdlObjectSupportClassId)
    {
        NetSession_CopyObjectReference(&copiedReference, nullptr, source);
        if (C_00873b14_CheckDdlObjectClassSupport(&copiedReference, nullptr)
            && copiedReference.vtbl)
        {
            spawnRecord =
                reinterpret_cast<std::uint8_t*>(copiedReference.vtbl) + 0x440;
        }
        goto releaseReference;
    }

    classId = NetDdlObjectManager_GetCheckedReferenceClassId(reference);
    if (classId == g_NetGameClassId)
    {
        NetSession_CopyObjectReference(&copiedReference, nullptr, source);
        if (C_00873b14_CheckNetGameClassRegistered(&copiedReference, nullptr)
            && copiedReference.vtbl)
        {
            spawnRecord =
                reinterpret_cast<std::uint8_t*>(copiedReference.vtbl) + 0x308;
        }
        goto releaseReference;
    }

    classId = NetDdlObjectManager_GetCheckedReferenceClassId(reference);
    if (classId == g_NetControlClassId)
    {
        C_00873b14_CopyRefAndReturnSelf(&copiedReference, nullptr, source);
        int object = C_00873b14_GetVtableIfValid(&copiedReference, nullptr);
        if (object)
            spawnRecord = reinterpret_cast<std::uint8_t*>(object) + 216;
        goto releaseReference;
    }

    classId = CSF_AT(std::uint32_t,
        NetDdlObjectManager_GetReferencedObject(reference, nullptr), 0x2C) >> 22;
    if (classId == static_cast<std::uint32_t>(g_NetCogibleDdlContext))
    {
        C_00873b14_CopyRefAndReturnSelf(&copiedReference, nullptr, source);
        int object = C_00873b14_GetVtableIfReady_74A960(&copiedReference, nullptr);
        if (object)
            spawnRecord = reinterpret_cast<std::uint8_t*>(object) + 120;
        goto releaseReference;
    }

    classId = CSF_AT(std::uint32_t,
        NetDdlObjectManager_GetReferencedObject(reference, nullptr), 0x2C) >> 22;
    if (classId == g_RequiredObjectClassId)
    {
        C_00873b14_CopyRefAndReturnSelf(&copiedReference, nullptr, source);
        int object = C_00873b14_GetVtableIfReady_74A980(&copiedReference, nullptr);
        if (object)
            spawnRecord = reinterpret_cast<std::uint8_t*>(object) + 184;
        goto releaseReference;
    }

    classId = CSF_AT(std::uint32_t,
        NetDdlObjectManager_GetReferencedObject(reference, nullptr), 0x2C) >> 22;
    if (classId == g_NetGhostClassId)
    {
        C_00873b14_CopyRefAndReturnSelf(&copiedReference, nullptr, source);
        int object = C_00873b14_GetVtableIfReady_73CDB0(&copiedReference, nullptr);
        if (object)
            spawnRecord = reinterpret_cast<std::uint8_t*>(object) + 128;
        goto releaseReference;
    }

    classId = CSF_AT(std::uint32_t,
        NetDdlObjectManager_GetReferencedObject(reference, nullptr), 0x2C) >> 22;
    if (classId == g_DdlObjectClassId)
    {
        C_00873b14_CopyRefAndReturnSelf(&copiedReference, nullptr, source);
        int object = C_00873b14_GetVtableIfReady_74A9A0(&copiedReference, nullptr);
        if (object)
            spawnRecord = reinterpret_cast<std::uint8_t*>(object) + 340;
        goto releaseReference;
    }

    NetSession_GetStationName(
        NetDdlObjectManager_GetReferencedObject(reference, nullptr), nullptr);
    return nullptr;

releaseReference:
    SpawnPoints_ReleaseEntry(&copiedReference, nullptr);
    return spawnRecord;
}

HOOK(0x0074A9C0, NetDdlObjectManager_GetSpawnRecordFromReference, "auto");
