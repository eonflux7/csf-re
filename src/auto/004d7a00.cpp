#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0083b958_Destroy, 0x004D1140, (void* self, void* edx));
CSF_FUNC(void, __fastcall, CharTaskBase_Init_4DB3C0, 0x004DB3C0, (void* self, void* edx));

static int __fastcall C_0083dbb0_Destroy(C_0083dbb0* self, void* /*edx*/)
{
    void* firstObject = reinterpret_cast<void*>(self->field_0308);
    self->vtbl = CSF_VTBL(0x0083DBB0);
    CSF_AT(void*, self, 0x04) = CSF_VTBL(0x0083DB94);

    if (firstObject)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, firstObject, 0)[0])(firstObject, nullptr, 1);
        self->field_0308 = 0;
    }

    void* secondObject = reinterpret_cast<void*>(self->field_030c);
    if (secondObject)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, secondObject, 0)[0])(secondObject, nullptr, 1);
        self->field_030c = 0;
    }

    auto* thirdObject = reinterpret_cast<C_00834fa8*>(self->field_0310);
    if (thirdObject)
    {
        thirdObject->vSlot0(1);
        self->field_0310 = 0;
    }

    auto* members = reinterpret_cast<std::uint8_t*>(self);
    CharTaskBase_Init_4DB3C0(members + 0x2D4, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x2C0, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x2AC, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x274, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x260, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x24C, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x238, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x224, nullptr);
    CSF_AT(void*, self, 0x218) = CSF_VTBL(0x0082D410);
    CharTaskBase_Init_4DB3C0(members + 0x204, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x1F0, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x1DC, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x1C8, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x1B4, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x1A0, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x18C, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x178, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x164, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x148, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x134, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x120, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x10C, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x0F8, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x0E4, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x0CC, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x0B8, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x0A4, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x090, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x07C, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x068, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x054, nullptr);
    CharTaskBase_Init_4DB3C0(members + 0x040, nullptr);

    return C_0083b958_Destroy(self, nullptr);
}

HOOK(0x004D7A00, C_0083dbb0_Destroy, "auto");
