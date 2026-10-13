#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(char, __stdcall, NetGameDdl_ProcessBaseField, 0x0065E750, (int a1, int a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(int, __fastcall, NetPlayerDdl_ProcessField, 0x0066CB70, (void* self, void* edx, int a2, int a3, int a4, int a5));

static char __fastcall NetGhostDdl_ProcessField(
    void* self, void* /*edx*/, int context, int fieldCode)
{
    // The original initializes this output slot with its incoming ECX.
    alignas(8) uint8_t propertyBytes[0x4];
    CSF_AT(void*, propertyBytes, 0) = self;

    uint32_t fieldOffset;
    uint8_t propertyId;

    switch (static_cast<uint8_t>(fieldCode))
    {
    case 1:
        fieldOffset = 0x44;
        propertyId = 1;
        break;
    case 2:
        fieldOffset = 0x47;
        propertyId = 2;
        break;
    case 3:
        fieldOffset = 0x4A;
        propertyId = 3;
        break;
    case 4:
        fieldOffset = 0x50;
        propertyId = 4;
        break;
    case 5:
        fieldOffset = 0x5C;
        propertyId = 5;
        break;
    case 6:
        fieldOffset = 0x68;
        propertyId = 6;
        break;
    default:
        return NetGameDdl_ProcessBaseField(context, fieldCode);
    }

    const int objectIndex = static_cast<int>(
        CSF_AT(uint32_t, self, 0x2C) >> 22);
    reinterpret_cast<uint8_t*>(&fieldCode)[3] = propertyId;

    auto* object = reinterpret_cast<uint8_t*>(
        LookupObjectByIndex(objectIndex));

    NetCogibleDdl_FindProperty(
        object + 0x0C,
        nullptr,
        reinterpret_cast<void**>(propertyBytes),
        reinterpret_cast<uint8_t*>(&fieldCode) + 3);

    void* property = CSF_AT(void*, propertyBytes, 0);
    NetPlayerDdl_ProcessField(
        CSF_AT(void*, property, 0x10),
        nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(static_cast<uint8_t*>(self) + fieldOffset),
        propertyId,
        context);

    return 1;
}

HOOK(0x0074C260, NetGhostDdl_ProcessField, "auto");
