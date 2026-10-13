#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void, __fastcall, SpawnPoints_ReleaseEntry, 0x00651B40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SpawnPoints_ApplyTempId, 0x00651D00, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __fastcall, NetCallbacks_ResolveSpawnEntry, 0x00743A10, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetValue200, 0x00751E00, ());
CSF_FUNC(uint32_t*, __fastcall, C_00868abc_Construct, 0x00751E10, (C_00868abc* self, void*));
CSF_FUNC(uint32_t*, __cdecl, NetSpawnMessages_CreateSpawnBuffer, 0x007522C0, (uint32_t* dimensions));
CSF_FUNC(char*, __cdecl, NetSpawnMessages_CreateThreeElementSpawnBuffer, 0x007524D0, (int parameters));
CSF_FUNC(uint8_t, __fastcall, NetCallbacks_SetSpawnValue, 0x007A79F0, (void* self, void* edx, int a2));
CSF_FUNC(char*, __fastcall, NetCallbacks_ConstructSpawnRegistration, 0x007A99F0, (void* self, void* edx, int a2, int a3, char a4));
CSF_FUNC(uint32_t*, __fastcall, NetCallbacks_RegisterSpawnBuffer, 0x007A9B00, (void* self, void* edx, uint8_t a2, int a3));

using SpawnBufferTable = uint32_t*[4];

CSF_GLOBAL(SpawnBufferTable, g_SpawnBuffers, 0x008BC12C);
CSF_GLOBAL(uint8_t*, g_SpawnParameters, 0x008BC13C);
CSF_GLOBAL(C_00868abc*, g_SpawnManager, 0x008BC140);
CSF_GLOBAL(char*, g_SpawnRegistration, 0x008BC144);
CSF_GLOBAL(char*, g_ThreeElementSpawnBuffer, 0x008BC148);
CSF_GLOBAL(void*, g_SpawnStorage, 0x008BC14C);
CSF_GLOBAL(int, g_TemporarySpawnId, 0x008BC188);
CSF_GLOBAL(uint32_t, g_SpawnObjectType, 0x008BC27C);

static void* NetCallbacks_GetValidatedSpawnData(C_00873b14& entry)
{
    if (!entry.vtbl)
    {
        Game_RaiseStatusNotice(0, 0, -1610416124, 0);
        return nullptr;
    }

    const uint32_t objectIndex = CSF_AT(uint32_t, entry.vtbl, 0x2C) >> 22;
    void* object = reinterpret_cast<void*>(LookupObjectByIndex(objectIndex));
    const bool matches =
        reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, object, 0)[0x3C / 4])(
                object, nullptr, g_SpawnObjectType);

    if (!matches)
    {
        Game_RaiseStatusNotice(0, 0, -536674292, 0);
        return nullptr;
    }

    return entry.vtbl;
}

static void __cdecl NetCallbacks_InitializeSpawnObjects(int objectAddress)
{
    alignas(8) uint8_t entryBytes[0xC];
    C_00873b14& entry = *reinterpret_cast<C_00873b14*>(entryBytes);

    SpawnPoints_ApplyTempId(&entry, nullptr, g_TemporarySpawnId);

    C_00868abc* managerMemory =
        static_cast<C_00868abc*>(Mem_Alloc(176));
    if (managerMemory)
    {
        g_SpawnManager = reinterpret_cast<C_00868abc*>(
            C_00868abc_Construct(managerMemory, nullptr));
    }
    else
    {
        g_SpawnManager = nullptr;
    }

    bool resolved = NetCallbacks_ResolveSpawnEntry(&entry, nullptr) != 0;
    g_SpawnManager->field_0020 =
        resolved ? reinterpret_cast<uint32_t>(entry.vtbl) : 0;

    resolved = NetCallbacks_ResolveSpawnEntry(&entry, nullptr) != 0;
    void* spawnData = resolved ? entry.vtbl : nullptr;
    CSF_AT(C_00868abc*, spawnData, 0x68) = g_SpawnManager;

    if (g_SpawnParameters)
    {
        g_ThreeElementSpawnBuffer =
            NetSpawnMessages_CreateThreeElementSpawnBuffer(
                reinterpret_cast<int>(g_SpawnParameters + 4));
    }

    resolved = NetCallbacks_ResolveSpawnEntry(&entry, nullptr) != 0;
    spawnData = resolved ? entry.vtbl : nullptr;
    const int spawnValue = GetValue200();
    NetCallbacks_SetSpawnValue(spawnData, nullptr, spawnValue);

    void* registrationMemory = NetZ_OperatorNew(120);
    if (registrationMemory)
    {
        const uint32_t objectHandle =
            CSF_AT(uint32_t, reinterpret_cast<void*>(objectAddress), 0x2C);
        int registrationHandle;
        if ((objectHandle & 0x3FFFFF) != 0)
        {
            registrationHandle = static_cast<int>(objectHandle);
        }
        else
        {
            Game_RaiseStatusNotice(0, 0, -536870898, 0);
            registrationHandle = 0;
        }

        resolved = NetCallbacks_ResolveSpawnEntry(&entry, nullptr) != 0;
        spawnData = resolved ? entry.vtbl : nullptr;
        g_SpawnRegistration = NetCallbacks_ConstructSpawnRegistration(
            registrationMemory, nullptr,
            reinterpret_cast<int>(spawnData), registrationHandle, 1);
    }
    else
    {
        g_SpawnRegistration = nullptr;
    }

    if (g_SpawnParameters)
    {
        for (int index = 0; ; ++index)
        {
            void* validatedData = NetCallbacks_GetValidatedSpawnData(entry);
            if (index >= CSF_AT(uint8_t, validatedData, 0x45))
                break;

            uint32_t* buffer = NetSpawnMessages_CreateSpawnBuffer(
                reinterpret_cast<uint32_t*>(g_SpawnParameters + 4));
            C_00868abc* manager = g_SpawnManager;
            g_SpawnBuffers[index] = buffer;
            NetCallbacks_RegisterSpawnBuffer(
                manager, nullptr, static_cast<uint8_t>(index),
                reinterpret_cast<int>(buffer));
        }
    }

    if (!g_SpawnStorage)
    {
        void* validatedData = NetCallbacks_GetValidatedSpawnData(entry);
        g_SpawnStorage = Mem_Alloc(CSF_AT(uint16_t, validatedData, 0x4C));
    }

    SpawnPoints_ReleaseEntry(&entry, nullptr);
}

HOOK(0x00743BA0, NetCallbacks_InitializeSpawnObjects, "auto");
