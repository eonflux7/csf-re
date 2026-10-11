#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(CMenuMpServerInfo*, __fastcall, CMenuMpServerInfo_Construct, 0x005309E0, (CMenuMpServerInfo* self, void* , int ownerHandle));
CSF_FUNC(CMenuMpScoreBar*, __fastcall, CMenuMpScoreBar_Construct, 0x00540A40, (CMenuMpScoreBar* self, void* , int ownerValue));
CSF_FUNC(CMenuMpStats*, __fastcall, CMenuMpStats_Construct, 0x00541980, (CMenuMpStats* self, void* , int ownerArg));
CSF_FUNC(CMenuPlayerInfo*, __fastcall, CMenuPlayerInfo_Construct, 0x0055DC90, (CMenuPlayerInfo* self, void* , int argument));
CSF_FUNC(CMenuPlayerList*, __fastcall, CMenuPlayerList_Construct, 0x0055DF60, (CMenuPlayerList* self, void* , int argument));
CSF_FUNC(int, __fastcall, CMenuMpSessions_Construct, 0x00561B90, (CMenuMpSessions* self, void* , int context));
CSF_FUNC(CMenuMpNetworkType*, __fastcall, CMenuMpNetworkType_Construct, 0x00565940, (CMenuMpNetworkType* self, void* , int argument));
CSF_FUNC(CMenuMpGameMode*, __fastcall, CMenuMpGameMode_Construct, 0x00565F50, (CMenuMpGameMode* self, void* , int parentMenu));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static int __fastcall C_00840754_InitializeMultiplayerScreens(
    C_00840754* self, void* /*edx*/, int context)
{
    void* allocation = Mem_Alloc(528);
    void* screen = allocation
        ? reinterpret_cast<void*>(CMenuMpSessions_Construct(
              static_cast<CMenuMpSessions*>(allocation), nullptr, context))
        : nullptr;
    self->field_0064 = reinterpret_cast<uint32_t>(screen);
    static_cast<CMenuMpSessions*>(screen)->field_01e0 = 3;
    void* owner = reinterpret_cast<void*>(self->field_0004);
    void* storedScreen = reinterpret_cast<void*>(self->field_0064);
    CSF_AT(uint32_t, storedScreen, 0x08) = CSF_AT(uint32_t, owner, 0x10);

    allocation = Mem_Alloc(528);
    screen = allocation
        ? reinterpret_cast<void*>(CMenuMpSessions_Construct(
              static_cast<CMenuMpSessions*>(allocation), nullptr, context))
        : nullptr;
    self->field_0068 = reinterpret_cast<uint32_t>(screen);
    static_cast<CMenuMpSessions*>(screen)->field_01e0 = 4;
    owner = reinterpret_cast<void*>(self->field_0004);
    storedScreen = reinterpret_cast<void*>(self->field_0068);
    CSF_AT(uint32_t, storedScreen, 0x08) = CSF_AT(uint32_t, owner, 0x10);

    allocation = Mem_Alloc(528);
    screen = allocation
        ? reinterpret_cast<void*>(CMenuMpSessions_Construct(
              static_cast<CMenuMpSessions*>(allocation), nullptr, context))
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0020 = reinterpret_cast<uint32_t>(screen);
    CSF_AT(uint32_t, screen, 0x08) = CSF_AT(uint32_t, owner, 0x10);
    reinterpret_cast<CMenuMpSessions*>(self->field_0020)->field_01e0 = 2;

    allocation = Mem_Alloc(48);
    screen = allocation
        ? CMenuMpGameMode_Construct(
              static_cast<CMenuMpGameMode*>(allocation), nullptr, context)
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_003c = reinterpret_cast<uint32_t>(screen);
    CSF_AT(uint32_t, screen, 0x08) = CSF_AT(uint32_t, owner, 0x10);

    allocation = Mem_Alloc(76);
    screen = allocation
        ? CMenuMpNetworkType_Construct(
              static_cast<CMenuMpNetworkType*>(allocation), nullptr, context)
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0044 = reinterpret_cast<uint32_t>(screen);
    CSF_AT(uint32_t, screen, 0x08) = CSF_AT(uint32_t, owner, 0x10);

    allocation = Mem_Alloc(520);
    screen = allocation
        ? CMenuMpServerInfo_Construct(
              static_cast<CMenuMpServerInfo*>(allocation), nullptr, context)
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_004c = reinterpret_cast<uint32_t>(screen);
    CSF_AT(uint32_t, screen, 0x08) = CSF_AT(uint32_t, owner, 0x10);

    allocation = Mem_Alloc(140);
    screen = allocation
        ? CMenuMpStats_Construct(
              static_cast<CMenuMpStats*>(allocation), nullptr, context)
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0050 = reinterpret_cast<uint32_t>(screen);
    CSF_AT(uint32_t, screen, 0x08) = CSF_AT(uint32_t, owner, 0x10);

    allocation = Mem_Alloc(60);
    screen = allocation
        ? CMenuMpScoreBar_Construct(
              static_cast<CMenuMpScoreBar*>(allocation), nullptr, context)
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0048 = reinterpret_cast<uint32_t>(screen);
    CSF_AT(uint32_t, screen, 0x08) = CSF_AT(uint32_t, owner, 0x10);

    allocation = Mem_Alloc(568);
    screen = allocation
        ? CMenuPlayerList_Construct(
              static_cast<CMenuPlayerList*>(allocation), nullptr, context)
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0030 = reinterpret_cast<uint32_t>(screen);
    CSF_AT(uint32_t, screen, 0x08) = CSF_AT(uint32_t, owner, 0x10);

    allocation = Mem_Alloc(76);
    screen = allocation
        ? CMenuPlayerInfo_Construct(
              static_cast<CMenuPlayerInfo*>(allocation), nullptr, context)
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0034 = reinterpret_cast<uint32_t>(screen);
    CSF_AT(uint32_t, screen, 0x08) = CSF_AT(uint32_t, owner, 0x10);
    return reinterpret_cast<int>(screen);
}

HOOK(0x00509960, C_00840754_InitializeMultiplayerScreens, "auto");
