#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(void*, __fastcall, NetPlayerDdl_GetFieldHandler, 0x0064F3A0,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, NetPlayerDdl_ProcessField, 0x0066CB70,
         (void* self, void* edx, int a2, int a3, int a4, int a5));
CSF_FUNC(char, __fastcall, NetPlayerDdl_DispatchBaseField, 0x007A3BC0,
         (void* self, void* edx, int a5, int a3));

static char __fastcall NetPlayerDdl_DispatchField(
    void* self, void* /*edx*/, int context, int fieldId)
{
    const char field = static_cast<char>(fieldId);
    std::uint8_t* fieldAddress;
    auto* bytes = static_cast<std::uint8_t*>(self);

    switch (field)
    {
    case 4:  fieldAddress = bytes + 1088; break;
    case 5:  fieldAddress = bytes + 1091; break;
    case 6:  fieldAddress = bytes + 1094; break;
    case 7:  fieldAddress = bytes + 1100; break;
    case 8:  fieldAddress = bytes + 1103; break;
    case 9:  fieldAddress = bytes + 1106; break;
    case 10: fieldAddress = bytes + 1112; break;
    case 11: fieldAddress = bytes + 1640; break;
    case 12: fieldAddress = bytes + 1652; break;
    case 13: fieldAddress = bytes + 1664; break;
    case 14: fieldAddress = bytes + 1670; break;
    case 15: fieldAddress = bytes + 1676; break;
    case 16: fieldAddress = bytes + 1682; break;
    case 17: fieldAddress = bytes + 1688; break;
    case 18: fieldAddress = bytes + 1694; break;
    case 19: fieldAddress = bytes + 1700; break;
    case 20: fieldAddress = bytes + 1704; break;
    default:
        return NetPlayerDdl_DispatchBaseField(self, nullptr, context, fieldId);
    }

    void* owner = reinterpret_cast<void*>(
        LookupObjectByIndex(CSF_AT(std::uint32_t, self, 0x2C) >> 22));
    void* handler = NetPlayerDdl_GetFieldHandler(owner, nullptr, field);
    NetPlayerDdl_ProcessField(
        handler, nullptr, reinterpret_cast<int>(self),
        reinterpret_cast<int>(fieldAddress), field, context);
    return 1;
}

HOOK(0x00747210, NetPlayerDdl_DispatchField, "auto");
