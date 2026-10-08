#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees (bound by address).
// 0x712CC0: __thiscall, no stack args; constructs the embedded member at this+4.
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x712CC0, (void* self, void* edx));
// 0x41E5F0: __thiscall, one stack arg (ret 4); grows the uint32 slot array at this+0x158.
CSF_FUNC(void*, __fastcall, GrowButtonArray, 0x41E5F0, (void* self, void* edx, int amount));

// Constructor of CMenuEula (__thiscall, one stack arg, ret 4).
// Vtable addresses are written as the same immediates the original stores:
//   0x0083042C = CMenuScreen base vtable (written to +0x00 first)
//   0x00841BC8 = embedded member vtable (written to +0x04)
//   0x00841B78 = CMenuEula vtable (written to +0x00 last)
static CMenuEula* __fastcall CMenuEula_Construct(CMenuEula* self, void* /*edx*/, int screenParam)
{
    // Base vtable first, then construct the embedded member at +4 (its address is this+4).
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x0083042C);
    C_00830438_Construct(reinterpret_cast<uint8_t*>(self) + 0x04, nullptr);

    // Embedded member's vtable lives at +4; then CMenuEula's vtable at +0.
    self->field_0004 = CSF_ADDR_VT(0x00841BC8);
    CSF_AT(uint32_t, self, 0x00) = CSF_ADDR_VT(0x00841B78);

    self->field_0054 = 7;
    self->field_0050 = 0;
    *reinterpret_cast<uint16_t*>(&self->field_0040) = 0; // 16-bit store only

    // Dynamic uint32 slot array embedded at +0x158:
    // buffer @+0x158, capacity @+0x15C, count @+0x160, growth step @+0x164.
    self->field_0158 = 0;
    CSF_AT(uint32_t, self, 0x15C) = 0;
    self->field_0160 = 0;
    CSF_AT(uint32_t, self, 0x164) = 10;

    self->field_0020 = static_cast<uint32_t>(screenParam);
    self->field_001c = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;

    // Ensure capacity of at least 1000 (unsigned compare).
    uint32_t initialCapacity = CSF_AT(uint32_t, self, 0x15C);
    if (initialCapacity < 0x3E8u) {
        GrowButtonArray(reinterpret_cast<uint8_t*>(self) + 0x158, nullptr,
                               static_cast<int>(1000u - initialCapacity));
    }

    self->field_0038 = 0;
    self->field_0160 = 0;

    // Append one zeroed slot; grow first if count >= capacity (unsigned compare).
    uint32_t countBeforeGrow = self->field_0160;
    if (countBeforeGrow >= CSF_AT(uint32_t, self, 0x15C)) {
        GrowButtonArray(reinterpret_cast<uint8_t*>(self) + 0x158, nullptr,
                               static_cast<int>(CSF_AT(uint32_t, self, 0x164)));
    }

    // Re-read count and buffer after a possible growth call.
    uint32_t appendIndex = self->field_0160;
    uint32_t* appendSlot = reinterpret_cast<uint32_t*>(self->field_0158 + appendIndex * 4u);
    self->field_0160 = appendIndex + 1;
    *appendSlot = 0;

    self->field_0018 = 0;
    return self;
}

HOOK(0x00532D00, CMenuEula_Construct, "auto");
