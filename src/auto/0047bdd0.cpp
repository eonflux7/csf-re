#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __fastcall, CharTask_InitBase, 0x004DB4C0, (void* self, void* edx));

static uint32_t* __fastcall TDisparar_Construct(void* self, void* /*edx*/) {
    CharTask_InitBase(self, nullptr);
    *static_cast<void***>(self) = CSF_VTBL(0x00832CC0);
    CSF_AT(uint32_t, self, 0x04) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00832CA0)));

    void* subA = static_cast<uint8_t*>(self) + 0x14;
    CharTask_InitBase(subA, nullptr);
    CSF_AT(uint32_t, self, 0x14) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00832318)));
    CSF_AT(uint32_t, self, 0x18) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008322F8)));

    void* subB = static_cast<uint8_t*>(self) + 0x28;
    CharTask_InitBase(subB, nullptr);
    CSF_AT(uint32_t, self, 0x28) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008323D8)));
    CSF_AT(uint32_t, self, 0x2C) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x008323BC)));

    void* subC = static_cast<uint8_t*>(self) + 0x3C;
    CharTask_InitBase(subC, nullptr);
    CSF_AT(uint32_t, self, 0x3C) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00832498)));
    CSF_AT(uint32_t, self, 0x40) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x0083247C)));

    return static_cast<uint32_t*>(self);
}

HOOK(0x0047BDD0, TDisparar_Construct, "auto");
