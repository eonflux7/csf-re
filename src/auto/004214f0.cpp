#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));

static uint32_t* __fastcall TFncGirar_Construct(void* self, void* /*edx*/) {
    CharTask_InitBase(self, nullptr);
    *static_cast<void***>(self) = CSF_VTBL(0x0082F938);
    CSF_AT(uint32_t, self, 0x04) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082F91C)));

    void* subA = static_cast<uint8_t*>(self) + 0x14;
    CharTask_InitBase(subA, nullptr);
    CSF_AT(uint32_t, self, 0x14) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082F4C0)));
    CSF_AT(uint32_t, self, 0x18) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082F4A4)));

    void* subB = static_cast<uint8_t*>(self) + 0x28;
    CharTask_InitBase(subB, nullptr);
    CSF_AT(uint32_t, self, 0x28) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082F580)));
    CSF_AT(uint32_t, self, 0x2C) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082F564)));

    void* subC = static_cast<uint8_t*>(self) + 0x3C;
    CharTask_InitBase(subC, nullptr);
    CSF_AT(uint32_t, self, 0x3C) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082F640)));
    CSF_AT(uint32_t, self, 0x40) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0082F624)));

    return static_cast<uint32_t*>(self);
}

HOOK(0x004214F0, TFncGirar_Construct, "auto");
