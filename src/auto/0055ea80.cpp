#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(void, __fastcall, MenuProgressInput_UpdateGlobalState, 0x00521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuOptions_ApplyTargetOffset, 0x0053AD90, (CMenuOptions* self, void*));
CSF_FUNC(int, __cdecl, GetGlobal8B0688, 0x00550CC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(int, __fastcall, CGuiListBoxBase_ClearStorageAndSendEvent, 0x00715660, (CGuiListBoxBase* self, void*));

static int __fastcall CMenuPlayerList_InitializePlayerList(
    CMenuPlayerList* self, void* /*edx*/, int context, int /*unused*/)
{
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->vClearField0010();

    self->field_0014 = context;
    void* progressInput = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    MenuProgressInput_UpdateGlobalState(progressInput, nullptr);

    void* screen = reinterpret_cast<void*>(self->field_001c);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, screen, 0)[0x16c / 4])(screen, nullptr);

    screen = reinterpret_cast<void*>(self->field_001c);
    void* playerList = reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, screen, 0)[0xfc / 4])(
            screen, nullptr, CSF_STR(0x008456DC, "ListaJugadores"));
    self->field_0024 = reinterpret_cast<std::uint32_t>(playerList);

    if (playerList)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, playerList, 0)[0x298 / 4])(playerList, nullptr);

        CGuiListBoxBase_ClearStorageAndSendEvent(
            reinterpret_cast<CGuiListBoxBase*>(self->field_0024), nullptr);

        void* globalManager = reinterpret_cast<void*>(GetGlobal8B0688());
        void* playerManager = CSF_AT(void*, globalManager, 0x1484);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, playerManager, 0)[0x10 / 4])(playerManager, nullptr);

        alignas(8) uint8_t menuBuffer[0xB4];
        MenuBuffer_Init(menuBuffer, nullptr);
        menuBuffer[24] = 1;
        void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
        GameBddLoader_DispatchMenuRequest(
            loader, nullptr, 1179792435, 1, 0,
            reinterpret_cast<int>(menuBuffer));
        self->field_002c = 1;
        SceneLoader_DestroyMenuBuffer(
            reinterpret_cast<std::uint32_t*>(menuBuffer), nullptr);
    }

    screen = reinterpret_cast<void*>(self->field_001c);
    self->field_0028 = reinterpret_cast<std::uint32_t>(
        reinterpret_cast<void*(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screen, 0)[0xfc / 4])(
                screen, nullptr, CSF_STR(0x00843B00, "BotonBack")));

    if (GetGlobalState_6A6D70())
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        void* menuTarget = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0xf4 / 4])(state, nullptr);
        if (menuTarget)
        {
            state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            menuTarget = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
                CSF_AT(void**, state, 0)[0xf4 / 4])(state, nullptr);
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, menuTarget, 0)[0x18 / 4])(menuTarget, nullptr, 1);
        }
    }

    self->vForwardCall(static_cast<int>(self->field_001c));
    CMenuOptions_ApplyTargetOffset(reinterpret_cast<CMenuOptions*>(self), nullptr);
    return static_cast<int>(self->field_001c);
}

HOOK(0x0055EA80, CMenuPlayerList_InitializePlayerList, "auto");
