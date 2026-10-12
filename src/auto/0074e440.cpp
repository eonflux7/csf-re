#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void*, __fastcall, NetDdl_GetPropertyObject, 0x0064F3A0, (void* self, void* edx, char a2));
CSF_FUNC(char, __stdcall, NetGameDdl_ProcessBaseField, 0x0065E750, (int a1, int a2));
CSF_FUNC(int, __fastcall, NetPlayerDdl_ProcessField, 0x0066CB70, (void* self, void* edx, int a2, int a3, int a4, int a5));

static char __fastcall NetGameDdl_ProcessField(
    void* self, void* /*edx*/, int context, int fieldIndex)
{
    const uint8_t propertyIndex = static_cast<uint8_t>(fieldIndex);
    uint32_t fieldOffset;

    switch (propertyIndex)
    {
    case 1:  fieldOffset = 0x44;  break;
    case 2:  fieldOffset = 0x50;  break;
    case 3:  fieldOffset = 0x5C;  break;
    case 4:  fieldOffset = 0x5F;  break;
    case 5:  fieldOffset = 0x64;  break;
    case 6:  fieldOffset = 0x2B8; break;
    case 7:  fieldOffset = 0x2C4; break;
    case 8:  fieldOffset = 0x2CA; break;
    case 9:  fieldOffset = 0x2D0; break;
    case 10: fieldOffset = 0x2D6; break;
    case 11: fieldOffset = 0x2DC; break;
    case 12: fieldOffset = 0x2E8; break;
    case 13: fieldOffset = 0x300; break;
    default:
        return NetGameDdl_ProcessBaseField(context, fieldIndex);
    }

    void* field = static_cast<uint8_t*>(self) + fieldOffset;
    void* ddlObject = reinterpret_cast<void*>(
        LookupObjectByIndex(CSF_AT(uint32_t, self, 0x2C) >> 22));
    void* propertyObject = NetDdl_GetPropertyObject(
        ddlObject, nullptr, static_cast<char>(propertyIndex));

    NetPlayerDdl_ProcessField(
        propertyObject, nullptr, reinterpret_cast<int>(self),
        reinterpret_cast<int>(field), propertyIndex, context);
    return 1;
}

HOOK(0x0074E440, NetGameDdl_ProcessField, "auto");
