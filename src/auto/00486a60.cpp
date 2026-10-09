#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4855C0, 0x004855C0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, sub_483F70, 0x00483F70, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_483FC0, 0x00483FC0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_484D30, 0x00484D30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4863B0, 0x004863B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_48D7E0, 0x0048D7E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_48D860, 0x0048D860, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_498F50, 0x00498F50, (void* self, void* edx, int a2, char a3));

static uint32_t ReplaceLowByte(uint32_t value, uint32_t low)
{
    return (value & 0xFFFFFF00u) | (low & 0xFFu);
}

static char __fastcall CCharActionFsm_DispatchStateAction(CCharActionFsm* self, void* /*edx*/, int a2)
{
    const uint32_t state = self->field_0010;
    uint32_t returnValue = state - 10;

    switch (state)
    {
    case 0xAu:
        returnValue = ReplaceLowByte(returnValue, static_cast<uint32_t>(sub_4855C0(self, nullptr)));
        break;
    case 0x22u:
        returnValue = ReplaceLowByte(returnValue, static_cast<uint32_t>(sub_484D30(self, nullptr)));
        break;
    case 0x24u:
        returnValue = ReplaceLowByte(returnValue,
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(sub_483F70(self, nullptr))));
        break;
    case 0x25u:
        returnValue = ReplaceLowByte(returnValue, static_cast<uint32_t>(sub_4863B0(self, nullptr)));
        break;
    case 0x26u:
        returnValue = ReplaceLowByte(returnValue, static_cast<uint32_t>(sub_483FC0(self, nullptr)));
        break;
    case 0x27u:
        returnValue = ReplaceLowByte(returnValue,
            static_cast<uint32_t>(sub_48D7E0(reinterpret_cast<void*>(self->field_00d4), nullptr)));
        break;
    case 0x28u:
    case 0x29u:
        returnValue = ReplaceLowByte(returnValue,
            static_cast<uint32_t>(sub_48D860(reinterpret_cast<void*>(self->field_00d4), nullptr)));
        break;
    default:
        break;
    }

    if (a2 != 18)
    {
        void* weaponObj = reinterpret_cast<void*>(self->field_0008);
        CSF_AT(uint32_t, weaponObj, 0x2E4) &= ~0x20u;
        returnValue = ReplaceLowByte(returnValue,
            static_cast<uint32_t>(sub_498F50(reinterpret_cast<void*>(self->field_0008), nullptr, 32, 0)));
        self->field_00bc = 0;
    }

    return static_cast<char>(returnValue & 0xFFu);
}

HOOK(0x00486A60, CCharActionFsm_DispatchStateAction, "auto");
