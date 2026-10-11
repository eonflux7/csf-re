#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(void, __cdecl, Game_RaiseStatusNotice, 0x00646D30, (int arg0, int arg1, uint32_t code, int arg3));
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(int, __fastcall, MpSpawnPoints_InitializeEventDescriptor, 0x00747EB0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, MpSpawnPoints_CreateEvent, 0x0073C050, (void* self, void* edx, int arg0, int a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_AttachEvent, 0x0073F730, (void* self, void* edx, int a2));
CSF_FUNC(void, __cdecl, MpSpawnPoints_FinishEventDescriptor, 0x0040ECF0, ());

static int __stdcall MpSpawnPoints_SubmitSpawnEvent(void* spawnPoint)
{
    const uint32_t spawnType = CSF_AT(uint32_t, spawnPoint, 0x24);
    int eventType;

    switch (spawnType)
    {
    case 6:    eventType = 5;  break;
    case 7:    eventType = 6;  break;
    case 8:    eventType = 7;  break;
    case 0x10: eventType = 8;  break;
    case 0x11: eventType = 9;  break;
    case 0x12: eventType = 10; break;
    case 0x13: eventType = 11; break;
    default:
        return static_cast<int>(spawnType - 6u);
    }

    void* resolvedObject = reinterpret_cast<void*>(
        CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(static_cast<uint8_t*>(spawnPoint) + 8)));

    alignas(4) uint8_t descriptor[0x2C]{};
    MpSpawnPoints_InitializeEventDescriptor(descriptor, nullptr);

    void* sourceObject = CSF_AT(void*, spawnPoint, 0x4C);
    void* sourceHandle = CSF_AT(void*, sourceObject, 0x2C);
    const uint32_t descriptorHeader = CSF_AT(uint32_t, spawnPoint, 0x50);

    CSF_AT(uint32_t, descriptor, 0x20) = CSF_AT(uint32_t, sourceHandle, 0);
    CSF_AT(uint16_t, descriptor, 0x04) = CSF_AT(uint16_t, sourceObject, 0x50);
    CSF_AT(uint16_t, descriptor, 0x06) = static_cast<uint16_t>(eventType);
    CSF_AT(uint32_t, descriptor, 0x00) = descriptorHeader;

    uint8_t* embeddedObject = CSF_AT(uint8_t*, resolvedObject, 0x18);
    void* ownerObject = embeddedObject ? embeddedObject - 1712 : nullptr;
    uint32_t ownerHandle = CSF_AT(uint32_t, ownerObject, 0x2C);

    if ((ownerHandle & 0x3FFFFF) == 0)
    {
        Game_RaiseStatusNotice(0, 0, 0xE000000E, 0);
        ownerHandle = 0;
    }
    CSF_AT(uint32_t, descriptor, 0x24) = ownerHandle;

    void* position = reinterpret_cast<void*>(
        GetObjectPositionVector(CSF_AT(void*, spawnPoint, 0x4C), nullptr));
    CSF_AT(uint32_t, descriptor, 0x08) = CSF_AT(uint32_t, position, 0);

    position = reinterpret_cast<void*>(
        GetObjectPositionVector(CSF_AT(void*, spawnPoint, 0x4C), nullptr));
    CSF_AT(uint32_t, descriptor, 0x0C) = CSF_AT(uint32_t, position, 4);

    position = reinterpret_cast<void*>(
        GetObjectPositionVector(CSF_AT(void*, spawnPoint, 0x4C), nullptr));
    const uint32_t positionZ = CSF_AT(uint32_t, position, 8);
    const uint32_t parameter0 = CSF_AT(uint32_t, spawnPoint, 0x54);
    const uint32_t parameter1 = CSF_AT(uint32_t, spawnPoint, 0x58);
    CSF_AT(uint32_t, descriptor, 0x14) = parameter0;
    CSF_AT(uint32_t, descriptor, 0x10) = positionZ;
    CSF_AT(uint32_t, descriptor, 0x18) = parameter1;
    CSF_AT(uint32_t, descriptor, 0x1C) = CSF_AT(uint32_t, spawnPoint, 0x5C);

    void* eventManager = reinterpret_cast<void*>(GetGlobalDword_73C020());
    void* event = MpSpawnPoints_CreateEvent(
        eventManager, nullptr, eventType, reinterpret_cast<int>(descriptor));

    int result = reinterpret_cast<int>(event);
    if (event)
    {
        result = MpSpawnPoints_AttachEvent(
            static_cast<uint8_t*>(CSF_AT(void*, spawnPoint, 0x4C)) + 0x14,
            nullptr, reinterpret_cast<int>(event));
    }

    MpSpawnPoints_FinishEventDescriptor();
    return result;
}

HOOK(0x00550EA0, MpSpawnPoints_SubmitSpawnEvent, "auto");
