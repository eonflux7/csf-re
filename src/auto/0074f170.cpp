#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(char, __stdcall, NetGameDdl_ProcessBaseField, 0x0065E750, (int a1, int a2));
CSF_FUNC(void**, __fastcall, NetCogibleDdl_FindProperty, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(int, __fastcall, NetPlayerDdl_ProcessField, 0x0066CB70, (void* self, void* edx, int a2, int a3, int a4, int a5));

static char __fastcall NetPlayerListDdl_ProcessField(
    void* self, void* /*edx*/, int context, int field)
{
    alignas(8) uint8_t propertyBytes[0x4];
    // The original reserves this output slot with push ecx.
    *reinterpret_cast<void**>(propertyBytes) = self;

    int fieldNumber;
    uint8_t* fieldAddress;

    switch (static_cast<uint8_t>(field))
    {
    case 1:
        fieldNumber = 1;
        fieldAddress = static_cast<uint8_t*>(self) + 0x44;
        break;
    case 2:
        fieldNumber = 2;
        fieldAddress = static_cast<uint8_t*>(self) + 0x58;
        break;
    case 3:
        fieldNumber = 3;
        fieldAddress = static_cast<uint8_t*>(self) + 0xA8;
        break;
    default:
        return NetGameDdl_ProcessBaseField(context, field);
    }

    uint8_t propertyKey = static_cast<uint8_t>(fieldNumber);
    const int object = LookupObjectByIndex(
        static_cast<int>(CSF_AT(uint32_t, self, 0x2C) >> 22));

    NetCogibleDdl_FindProperty(
        reinterpret_cast<uint8_t*>(object) + 0xC,
        nullptr,
        reinterpret_cast<void**>(propertyBytes),
        &propertyKey);

    void* property = *reinterpret_cast<void**>(propertyBytes);
    NetPlayerDdl_ProcessField(
        CSF_AT(void*, property, 0x10),
        nullptr,
        reinterpret_cast<int>(self),
        reinterpret_cast<int>(fieldAddress),
        fieldNumber,
        context);
    return 1;
}

HOOK(0x0074F170, NetPlayerListDdl_ProcessField, "auto");
