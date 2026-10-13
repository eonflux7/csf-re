#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, MenuSubobjectBase_Init, 0x00712CC0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_00861bd8_ClearField0010, 0x006E5090,
         (void* self, void* edx));

[[maybe_unused]] static const auto kClearField0010Target =
    C_00861bd8_ClearField0010;


static CMenuMpOnline* __fastcall CMenuMpOnline_Construct(
    CMenuMpOnline* self, void* /*edx*/, int constructorArg)
{
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x0083042C);
    MenuSubobjectBase_Init(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    CSF_AT(uint32_t, self, 0x40) = static_cast<uint32_t>(constructorArg);
    CSF_AT(void*, self, 0x0) = CSF_VTBL(0x00845740);
    CSF_AT(uint32_t, self, 0x4) = static_cast<uint32_t>(CSF_ADDR_VT(0x008456F0));

    CSF_AT(uint32_t, self, 0x14) = 0;
    CSF_AT(uint32_t, self, 0x1C) = 0;
    CSF_AT(uint32_t, self, 0x20) = 0;
    CSF_AT(uint32_t, self, 0x24) = 0;
    CSF_AT(uint32_t, self, 0x28) = 0;
    CSF_AT(uint32_t, self, 0x2C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    CSF_AT(uint8_t, self, 0x34) = 0;
    CSF_AT(uint8_t, self, 0x35) = 0;
    CSF_AT(uint8_t, self, 0x36) = 0;
    CSF_AT(uint8_t, self, 0x37) = 0;
    CSF_AT(uint8_t, self, 0x38) = 0;
    CSF_AT(uint8_t, self, 0x39) = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x18) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;

    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_VTBL(0x00845780)[0])(self, nullptr);
    return self;
}

HOOK(0x0055FA60, CMenuMpOnline_Construct, "auto");
