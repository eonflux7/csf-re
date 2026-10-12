#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));
CSF_FUNC(int, __fastcall, C_008310b8_InitializeSelection, 0x00550C40, (C_008310b8* self, void*));
CSF_FUNC(uint32_t*, __fastcall, ObjectBase_Initialize, 0x0064EBA0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Registry_Attach, 0x0065E310, (void* self, void* edx, int a2));

CSF_GLOBAL(std::uint32_t, g_NetGameClassId, 0x008BC178);

static int __fastcall C_00867fc0_Initialize(C_00867fc0* self, void* /*edx*/)
{
    auto* bytes = reinterpret_cast<std::uint8_t*>(self);

    ObjectBase_Initialize(self, nullptr);
    self->vtbl = CSF_VTBL(0x00867FC0);

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x44), nullptr);
    CSF_AT(std::uint8_t, self, 0x48) = 0;
    CSF_AT(std::uint32_t, self, 0x44) = 0;
    CSF_AT(std::uint32_t, self, 0x4C) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x50), nullptr);
    CSF_AT(std::uint8_t, self, 0x54) = 0;
    CSF_AT(std::uint32_t, self, 0x50) = 0;
    CSF_AT(std::uint32_t, self, 0x58) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x5C), nullptr);
    CSF_AT(std::uint8_t, self, 0x5D) = 0;
    CSF_AT(std::uint8_t, self, 0x5C) = 0;
    CSF_AT(std::uint8_t, self, 0x5E) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x5F), nullptr);
    CSF_AT(std::uint8_t, self, 0x60) = 0;
    CSF_AT(std::uint8_t, self, 0x5F) = 0;
    CSF_AT(std::uint8_t, self, 0x61) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x64), nullptr);
    CSF_AT(std::uint8_t, self, 0x202) = 0;
    CSF_AT(std::uint32_t, self, 0x21C) = 15;
    CSF_AT(std::uint32_t, self, 0x218) = 0;
    CSF_AT(std::uint8_t, self, 0x208) = 0;
    C_008310b8_InitializeSelection(reinterpret_cast<C_008310b8*>(bytes + 0x64), nullptr);

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x2B8), nullptr);
    CSF_AT(std::uint8_t, self, 0x2BC) = 0;
    CSF_AT(std::uint32_t, self, 0x2B8) = 0;
    CSF_AT(std::uint32_t, self, 0x2C0) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x2C4), nullptr);
    CSF_AT(std::uint8_t, self, 0x2C6) = 0;
    CSF_AT(std::uint16_t, self, 0x2C4) = 0;
    CSF_AT(std::uint16_t, self, 0x2C8) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x2CA), nullptr);
    CSF_AT(std::uint8_t, self, 0x2CC) = 0;
    CSF_AT(std::uint16_t, self, 0x2CA) = 0;
    CSF_AT(std::uint16_t, self, 0x2CE) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x2D0), nullptr);
    CSF_AT(std::uint8_t, self, 0x2D2) = 0;
    CSF_AT(std::uint16_t, self, 0x2D0) = 0;
    CSF_AT(std::uint16_t, self, 0x2D4) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x2D6), nullptr);
    CSF_AT(std::uint8_t, self, 0x2D8) = 0;
    CSF_AT(std::uint16_t, self, 0x2D6) = 0;
    CSF_AT(std::uint16_t, self, 0x2DA) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x2DC), nullptr);
    CSF_AT(std::uint8_t, self, 0x2DE) = 0;
    CSF_AT(std::uint16_t, self, 0x2DC) = 0;
    CSF_AT(std::uint16_t, self, 0x2E0) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x2E8), nullptr);
    CSF_AT(std::uint8_t, self, 0x2F0) = 0;
    CSF_AT(std::uint32_t, self, 0x2E8) = 0;
    CSF_AT(std::uint32_t, self, 0x2EC) = 0;
    CSF_AT(std::uint32_t, self, 0x2F8) = 0;
    CSF_AT(std::uint32_t, self, 0x2FC) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x300), nullptr);
    CSF_AT(std::uint8_t, self, 0x301) = 0;
    CSF_AT(std::uint8_t, self, 0x300) = 0;
    CSF_AT(std::uint8_t, self, 0x302) = 0;

    Registry_Attach(bytes + 0x2C, nullptr, static_cast<int>(g_NetGameClassId));
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x0074D180, C_00867fc0_Initialize, "auto");
