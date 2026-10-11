#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));

static uint32_t* __fastcall TConversacion_Construct(TConversacion* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    CharTask_InitBase(self, nullptr);
    self->vtbl = CSF_VTBL(0x0083DC90);
    self->field_0004 = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083DC74);

    uint8_t* sub0 = base + 0x14;
    CharTask_InitBase(sub0, nullptr);
    CSF_AT(uint32_t, sub0, 0) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083D978);
    CSF_AT(uint32_t, sub0, 4) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083D95C);

    uint8_t* sub1 = sub0 + 0x14;
    CharTask_InitBase(sub1, nullptr);
    CSF_AT(uint32_t, sub1, 0) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083D168);
    CSF_AT(uint32_t, sub1, 4) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083D14C);

    uint8_t* sub2 = sub0 + 0x28;
    CharTask_InitBase(sub2, nullptr);
    CSF_AT(uint32_t, sub2, 0) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083D228);
    CSF_AT(uint32_t, sub2, 4) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083D208);

    uint8_t* sub3 = base + 0x50;
    CharTask_InitBase(sub3, nullptr);
    CSF_AT(uint32_t, sub3, 0) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083D2E8);
    CSF_AT(uint32_t, sub3, 4) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083D2C8);

    uint8_t* sub4 = base + 0x64;
    CharTask_InitBase(sub4, nullptr);
    CSF_AT(uint32_t, sub4, 0) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083D3A8);
    CSF_AT(uint32_t, sub4, 4) = (uint32_t)(uintptr_t)CSF_ADDR_DATA(0x0083D388);

    self->field_007c = 0xFFFFFFFFu;
    self->field_0080 = 0xFFFFFFFFu;
    CSF_AT(void*, self, 0x78) = CSF_VTBL(0x0082D448);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004D98C0, TConversacion_Construct, "auto");
