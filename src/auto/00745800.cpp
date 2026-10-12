#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, C_008310b8_GetSelf, 0x004B2980, (C_008310b8* self, void*));
CSF_FUNC(int, __fastcall, Registry_Attach, 0x0065E310, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MpSpawnPoints_InitializeDescriptor, 0x00749AC0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, NetPlayerDdl_InitializeBaseDescriptor, 0x007A2DC0, (void* self, void* edx));

CSF_GLOBAL(std::uint32_t, g_NetPlayerClassId, 0x008BC15C);

static int __fastcall C_008678a8_InitializeBase(C_008678a8* self, void* /*edx*/)
{
    NetPlayerDdl_InitializeBaseDescriptor(self, nullptr);
    self->vtbl = CSF_VTBL(0x008678A8);

    auto* bytes = reinterpret_cast<std::uint8_t*>(self);

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x440), nullptr);
    CSF_AT(std::uint8_t, self, 0x441) = 0;
    CSF_AT(std::uint8_t, self, 0x440) = 0;
    CSF_AT(std::uint8_t, self, 0x442) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x443), nullptr);
    CSF_AT(std::uint8_t, self, 0x444) = 0;
    CSF_AT(std::uint8_t, self, 0x443) = 0;
    CSF_AT(std::uint8_t, self, 0x445) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x446), nullptr);
    CSF_AT(std::uint8_t, self, 0x448) = 0;
    CSF_AT(std::uint16_t, self, 0x446) = 0;
    CSF_AT(std::uint16_t, self, 0x44A) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x44C), nullptr);
    CSF_AT(std::uint8_t, self, 0x44D) = 0;
    CSF_AT(std::uint8_t, self, 0x44C) = 0;
    CSF_AT(std::uint8_t, self, 0x44E) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x44F), nullptr);
    CSF_AT(std::uint8_t, self, 0x450) = 0;
    CSF_AT(std::uint8_t, self, 0x44F) = 0;
    CSF_AT(std::uint8_t, self, 0x451) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x452), nullptr);
    CSF_AT(std::uint8_t, self, 0x454) = 0;
    CSF_AT(std::uint16_t, self, 0x452) = 0;

    MpSpawnPoints_InitializeDescriptor(bytes + 0x458, nullptr);

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x668), nullptr);
    CSF_AT(std::uint8_t, self, 0x66C) = 0;
    CSF_AT(std::uint32_t, self, 0x668) = 0;
    CSF_AT(std::uint32_t, self, 0x670) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x674), nullptr);
    CSF_AT(std::uint8_t, self, 0x678) = 0;
    CSF_AT(std::uint32_t, self, 0x674) = 0;
    CSF_AT(std::uint32_t, self, 0x67C) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x680), nullptr);
    CSF_AT(std::uint8_t, self, 0x682) = 0;
    CSF_AT(std::uint16_t, self, 0x680) = 0;
    CSF_AT(std::uint16_t, self, 0x684) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x686), nullptr);
    CSF_AT(std::uint8_t, self, 0x688) = 0;
    CSF_AT(std::uint16_t, self, 0x686) = 0;
    CSF_AT(std::uint16_t, self, 0x68A) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x68C), nullptr);
    CSF_AT(std::uint8_t, self, 0x68E) = 0;
    CSF_AT(std::uint16_t, self, 0x68C) = 0;
    CSF_AT(std::uint16_t, self, 0x690) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x692), nullptr);
    CSF_AT(std::uint8_t, self, 0x694) = 0;
    CSF_AT(std::uint16_t, self, 0x692) = 0;
    CSF_AT(std::uint16_t, self, 0x696) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x698), nullptr);
    CSF_AT(std::uint8_t, self, 0x69A) = 0;
    CSF_AT(std::uint16_t, self, 0x698) = 0;
    CSF_AT(std::uint16_t, self, 0x69C) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x69E), nullptr);
    CSF_AT(std::uint8_t, self, 0x6A0) = 0;
    CSF_AT(std::uint16_t, self, 0x69E) = 0;
    CSF_AT(std::uint16_t, self, 0x6A2) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x6A4), nullptr);
    CSF_AT(std::uint8_t, self, 0x6A5) = 0;
    CSF_AT(std::uint8_t, self, 0x6A4) = 0;
    CSF_AT(std::uint8_t, self, 0x6A6) = 0;

    C_008310b8_GetSelf(reinterpret_cast<C_008310b8*>(bytes + 0x6A8), nullptr);
    CSF_AT(std::uint8_t, self, 0x6AC) = 0;
    CSF_AT(std::uint32_t, self, 0x6A8) = 0;

    Registry_Attach(bytes + 0x2C, nullptr, static_cast<int>(g_NetPlayerClassId));
    return static_cast<int>(reinterpret_cast<std::intptr_t>(self));
}

HOOK(0x00745800, C_008678a8_InitializeBase, "auto");
