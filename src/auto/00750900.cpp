#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void*, __fastcall, NetDdl_GetPropertyObject, 0x0064F3A0, (void* self, void* edx, char a2));
CSF_FUNC(char, __stdcall, NetGameDdl_ProcessBaseField, 0x0065E750, (int a1, int a2));
CSF_FUNC(int, __fastcall, NetPlayerDdl_ProcessField, 0x0066CB70, (void* self, void* edx, int a2, int a3, int a4, int a5));

static char __fastcall NetPostmanDdl_ProcessField(
    void* self, void* /*edx*/, int context, int fieldId)
{
    const uint8_t propertyId = static_cast<uint8_t>(fieldId);
    auto* bytes = static_cast<uint8_t*>(self);
    void* field;

    switch (propertyId)
    {
    case 1: field = bytes + 0x44; break;
    case 2: field = bytes + 0x50; break;
    case 3: field = bytes + 0x5C; break;
    case 4: field = bytes + 0x6C; break;
    case 5: field = bytes + 0x78; break;
    case 6: field = bytes + 0x88; break;
    case 7: field = bytes + 0xA0; break;
    default:
        return NetGameDdl_ProcessBaseField(context, fieldId);
    }

    void* object = reinterpret_cast<void*>(
        LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2C) >> 22));
    void* property = NetDdl_GetPropertyObject(
        object, nullptr, static_cast<char>(propertyId));
    NetPlayerDdl_ProcessField(
        property, nullptr, reinterpret_cast<int>(self),
        reinterpret_cast<int>(field), propertyId, context);
    return 1;
}

HOOK(0x00750900, NetPostmanDdl_ProcessField, "auto");
