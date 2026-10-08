#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, OnlineMenu_PrepareEnvironment, 0x0054EC30, ());
CSF_FUNC(void*, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, OnlineMenu_SetSharedText, 0x00521E30,
         (void* self, void* edx, const char* text));
CSF_FUNC(void*, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160,
         (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __fastcall, CMenuMpOnline_ScrollViewToTarget, 0x00542CD0,
         (CMenuMpOnline* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMpOnline_RefreshOnlineMatchState, 0x0055EE30,
         (CMenuMpOnline* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(std::uint8_t, __fastcall, GlobalFlag_IsSet, 0x007A7B50,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuMpOnline_RefreshForState, 0x0055ECA0,
         (CMenuMpOnline* self, void* edx));
CSF_FUNC(void*, __fastcall, MenuBuffer_Init, 0x00501870,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, Game_DispatchMenuRequest, 0x0050F790,
         (void* self, void* edx, std::uint32_t id, int mode,
          int flags, void* request));
CSF_FUNC(unsigned int, __fastcall, MenuBuffer_Destroy, 0x004FF3C0,
         (void* self, void* edx));

static int __fastcall CMenuMpOnline_Initialize(
    CMenuMpOnline* self, void* /*edx*/, int context, int /*unused*/)
{
    if (!self->field_0014)
    {
        void* state = GetGlobalState_50D190();
        ((void(__fastcall*)(void*, void*, CMenuMpOnline*))
             CSF_AT(void**, state, 0)[0x18 / 4])(state, nullptr, self);
    }

    OnlineMenu_PrepareEnvironment();

    void* screen = reinterpret_cast<void*>(self->field_0014);
    if (screen)
    {
        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, screen, 0)[0xA8 / 4])(screen, nullptr, 0);
    }

    void** menuVtable = CSF_AT(void**, self, 0);
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->m_friendsButton = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0035 = 0;
    self->field_0036 = 0;
    self->field_0037 = 0;
    self->field_0038 = 0;
    self->field_0039 = 0;
    self->field_003c = 0;
    self->field_004c = 0;
    self->field_0018 = 0;
    self->field_0050 = 0;
    ((void(__fastcall*)(void*, void*))menuVtable[0x40 / 4])(self, nullptr);

    self->field_003c = static_cast<std::uint32_t>(context);
    MenuProgressInput_UpdateGlobalState(GetGlobalValue_521BB0(), nullptr);

    screen = reinterpret_cast<void*>(self->field_0014);
    ((void(__fastcall*)(void*, void*))
         CSF_AT(void**, screen, 0)[0x16C / 4])(screen, nullptr);

    screen = reinterpret_cast<void*>(self->field_0014);
    std::uint32_t button =
        ((std::uint32_t(__fastcall*)(void*, void*, const char*))
             CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x008457C4, "BotonQuickMatch"));
    screen = reinterpret_cast<void*>(self->field_0014);
    self->field_001c = button;

    button =
        ((std::uint32_t(__fastcall*)(void*, void*, const char*))
             CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x008457B4, "BotonOptiMatch"));
    screen = reinterpret_cast<void*>(self->field_0014);
    self->field_0020 = button;

    button =
        ((std::uint32_t(__fastcall*)(void*, void*, const char*))
             CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x008457A0, "BotonCreateMatch"));
    screen = reinterpret_cast<void*>(self->field_0014);
    self->field_0024 = button;

    button =
        ((std::uint32_t(__fastcall*)(void*, void*, const char*))
             CSF_AT(void**, screen, 0)[0xFC / 4])(
            screen, nullptr, CSF_STR(0x00841DDC, "BotonFriends"));
    self->m_friendsButton = button;
    if (button)
    {
        void* friendsButton = reinterpret_cast<void*>(button);
        ((void(__fastcall*)(void*, void*, int))
             CSF_AT(void**, friendsButton, 0)[0xA8 / 4])(
            friendsButton, nullptr, 1);
    }

    void* service = LookupObjectById(GetGlobalInt(), nullptr);
    ((void(__fastcall*)(void*, void*))
         CSF_AT(void**, service, 0)[0x18 / 4])(service, nullptr);

    service = LookupObjectById(GetGlobalInt(), nullptr);
    ((void(__fastcall*)(void*, void*))
         CSF_AT(void**, service, 0)[0x18 / 4])(service, nullptr);

    screen = reinterpret_cast<void*>(self->field_0014);
    menuVtable = CSF_AT(void**, self, 0);
    self->field_0048 = 1;
    ((void(__fastcall*)(void*, void*, void*))
         menuVtable[0x44 / 4])(self, nullptr, screen);

    void* state = GetGlobalState_50D190();
    void* stateMember = CSF_AT(void*, state, 0x10);
    ((void(__fastcall*)(void*, void*))
         CSF_AT(void**, stateMember, 0)[0x08 / 4])(stateMember, nullptr);

    state = GetGlobalState_50D190();
    CSF_AT(std::uint8_t, state, 0xD0) = 1;

    void* settings = GetGlobalValue_8BC1E0();
    const std::uint32_t capacity = CSF_AT(std::uint32_t, settings, 0xF9C);
    auto* sharedString = static_cast<std::uint8_t*>(settings) + 0xF84;
    const char* text = capacity < 16
        ? reinterpret_cast<const char*>(sharedString + 4)
        : CSF_AT(const char*, sharedString, 4);
    void* sharedState = GetGlobalValue_521BB0();
    OnlineMenu_SetSharedText(sharedState, nullptr, text);

    CMenuMpOnline_ScrollViewToTarget(self, nullptr);
    CMenuMpOnline_RefreshOnlineMatchState(self, nullptr);

    if (GlobalFlag_IsSet(GetGlobalDword_73C020(), nullptr))
    {
        MenuMpOnline_RefreshForState(self, nullptr);
    }
    else
    {
        alignas(4) std::uint8_t request[0xB4];
        self->field_0018 = 18;
        self->field_0050 = 0;
        MenuBuffer_Init(request, nullptr);
        CSF_AT(std::uint8_t, request, 0x18) = 1;
        state = GetGlobalState_50D190();
        Game_DispatchMenuRequest(state, nullptr, 0x444E5331, 2, 0, request);
        MenuBuffer_Destroy(request, nullptr);
    }

    return static_cast<int>(self->field_0014);
}

HOOK(0x0055FAD0, CMenuMpOnline_Initialize, "auto");
