#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00840754_InitializeBaseMenus, 0x00509960, (void* self, void* edx, int a2));
CSF_FUNC(CMenuMpLoadout*, __fastcall, CMenuMpLoadout_ConstructAndResetState, 0x0052F640, (CMenuMpLoadout* self, void* , int argument));
CSF_FUNC(CMenuEula*, __fastcall, CMenuEula_Construct, 0x00532D00, (CMenuEula* self, void* , int screenParam));
CSF_FUNC(CMenuMpPause*, __fastcall, CMenuMpPause_Construct, 0x00535310, (CMenuMpPause* self, void* , int ownerArg));
CSF_FUNC(void*, __fastcall, CMenuAccountEdit_Construct, 0x0055C950, (CMenuAccountEdit* self, void* , int ownerArg));
CSF_FUNC(CMenuMpOnline*, __fastcall, CMenuMpOnline_Construct, 0x0055FA60, (CMenuMpOnline* self, void* , int constructorArg));
CSF_FUNC(CMenuAccounts*, __fastcall, CMenuAccounts_Construct, 0x00565470, (CMenuAccounts* self, void* , int));
CSF_FUNC(CMenuMpSessionsPC*, __fastcall, CMenuMpSessionsPC_Construct, 0x00565FE0, (CMenuMpSessionsPC* self, void* , int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static C_008407a8* __fastcall C_00840754_InitializePcMenus(
    C_00840754* self, void* /*edx*/, int ownerArg)
{
    C_00840754_InitializeBaseMenus(self, nullptr, ownerArg);

    auto* pauseStorage = static_cast<CMenuMpPause*>(Mem_Alloc(80));
    auto* screen = pauseStorage
        ? reinterpret_cast<C_008407a8*>(
            CMenuMpPause_Construct(pauseStorage, nullptr, ownerArg))
        : nullptr;
    auto* owner = reinterpret_cast<void*>(self->field_0004);
    self->field_006c = reinterpret_cast<std::uint32_t>(screen);
    screen->field_0008 = CSF_AT(std::uint32_t, owner, 0x10);

    auto* onlineStorage = static_cast<CMenuMpOnline*>(Mem_Alloc(84));
    screen = onlineStorage
        ? reinterpret_cast<C_008407a8*>(
            CMenuMpOnline_Construct(onlineStorage, nullptr, ownerArg))
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0018 = reinterpret_cast<std::uint32_t>(screen);
    screen->field_0008 = CSF_AT(std::uint32_t, owner, 0x10);

    auto* accountEditStorage = static_cast<CMenuAccountEdit*>(Mem_Alloc(672));
    screen = accountEditStorage
        ? reinterpret_cast<C_008407a8*>(
            CMenuAccountEdit_Construct(accountEditStorage, nullptr, ownerArg))
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_000c = reinterpret_cast<std::uint32_t>(screen);
    screen->field_0008 = CSF_AT(std::uint32_t, owner, 0x10);

    accountEditStorage = static_cast<CMenuAccountEdit*>(Mem_Alloc(672));
    screen = accountEditStorage
        ? reinterpret_cast<C_008407a8*>(
            CMenuAccountEdit_Construct(accountEditStorage, nullptr, ownerArg))
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0010 = reinterpret_cast<std::uint32_t>(screen);
    screen->field_0008 = CSF_AT(std::uint32_t, owner, 0x10);

    accountEditStorage = static_cast<CMenuAccountEdit*>(Mem_Alloc(672));
    screen = accountEditStorage
        ? reinterpret_cast<C_008407a8*>(
            CMenuAccountEdit_Construct(accountEditStorage, nullptr, ownerArg))
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0014 = reinterpret_cast<std::uint32_t>(screen);
    screen->field_0008 = CSF_AT(std::uint32_t, owner, 0x10);

    auto* accountsStorage = static_cast<CMenuAccounts*>(Mem_Alloc(76));
    screen = accountsStorage
        ? reinterpret_cast<C_008407a8*>(
            CMenuAccounts_Construct(accountsStorage, nullptr, ownerArg))
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0008 = reinterpret_cast<std::uint32_t>(screen);
    screen->field_0008 = CSF_AT(std::uint32_t, owner, 0x10);

    auto* loadoutStorage = static_cast<CMenuMpLoadout*>(Mem_Alloc(92));
    screen = loadoutStorage
        ? reinterpret_cast<C_008407a8*>(
            CMenuMpLoadout_ConstructAndResetState(loadoutStorage, nullptr, ownerArg))
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0070 = reinterpret_cast<std::uint32_t>(screen);
    screen->field_0008 = CSF_AT(std::uint32_t, owner, 0x10);

    auto* sessionsStorage = static_cast<CMenuMpSessionsPC*>(Mem_Alloc(528));
    screen = sessionsStorage
        ? reinterpret_cast<C_008407a8*>(
            CMenuMpSessionsPC_Construct(sessionsStorage, nullptr, ownerArg))
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_001c = reinterpret_cast<std::uint32_t>(screen);
    screen->field_0008 = CSF_AT(std::uint32_t, owner, 0x10);
    CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_001c), 0x1E0) = 1;

    sessionsStorage = static_cast<CMenuMpSessionsPC*>(Mem_Alloc(528));
    screen = sessionsStorage
        ? reinterpret_cast<C_008407a8*>(
            CMenuMpSessionsPC_Construct(sessionsStorage, nullptr, ownerArg))
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0024 = reinterpret_cast<std::uint32_t>(screen);
    screen->field_0008 = CSF_AT(std::uint32_t, owner, 0x10);
    CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0024), 0x1E0) = 0;

    auto* eulaStorage = static_cast<CMenuEula*>(Mem_Alloc(360));
    screen = eulaStorage
        ? reinterpret_cast<C_008407a8*>(
            CMenuEula_Construct(eulaStorage, nullptr, ownerArg))
        : nullptr;
    owner = reinterpret_cast<void*>(self->field_0004);
    self->field_0074 = reinterpret_cast<std::uint32_t>(screen);
    screen->field_0008 = CSF_AT(std::uint32_t, owner, 0x10);
    return screen;
}

HOOK(0x0050A110, C_00840754_InitializePcMenus, "auto");
