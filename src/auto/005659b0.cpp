#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(std::uint32_t, __fastcall, SceneLoader_DestroyMenuBuffer, 0x004FF3C0, (std::uint32_t* self, void*));
CSF_FUNC(int, __fastcall, MenuBuffer_Init, 0x00501870, (uint8_t* self, void*));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GameBddLoader_ResetPending, 0x0050E1E0, (void* self, void*));
CSF_FUNC(int, __fastcall, GameBddLoader_DispatchMenuRequest, 0x0050F790, (void* self, void* , int entry, int entryArgument, int requestData, int configuration));
CSF_FUNC(int, __cdecl, CoreParser_ResolveEntryByKey, 0x00692440, (unsigned int key, int flags));
CSF_FUNC(int, __cdecl, StringFormat_CompareWideAgainstNarrow, 0x00695500, (uint16_t* a1, char* a2));
CSF_FUNC(int, __fastcall, S_004f21d0_SetPattern, 0x00697100, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(void, __cdecl, MenuAccounts_ResetPendingEdit, 0x006A6D40, ());
CSF_FUNC(int, __cdecl, GetGlobalState_6A6D70, 0x006A6D70, ());
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0, (void* self, void* edx, const void* text));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszMenu_P_MePrin, 0x00899AE4);
CSF_GLOBAL(const char*, g_pszMenu_P_LbSlCu, 0x00899B30);
CSF_GLOBAL(int, g_MpNetworkType, 0x0089A5F0);

using QueryInt = int (__fastcall*)(void*, void*);
using QueryBool = bool (__fastcall*)(void*, void*);
using SetWidgetState = int (__fastcall*)(void*, void*, int, int, int);
using SetMenuName = void (__fastcall*)(void*, void*, const char*);
using OpenMenu = void (__fastcall*)(void*, void*, CMenuMpNetworkType*, int);
using SetNetworkMode = void (__fastcall*)(void*, void*, int);

static int __fastcall CMenuMpNetworkType_Update(
    CMenuMpNetworkType* self, void* /*edx*/, int /*argument*/)
{
    SceneLoader_NoOp();

    if (self->field_0049)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        int result = reinterpret_cast<QueryInt>(CSF_AT(void**, state, 0)[28 / 4])(
            state, nullptr);
        if (result)
            return result;

        GameBddLoader_ResetPending(
            reinterpret_cast<void*>(GetGlobalState_50D190()), nullptr);
        MenuAccounts_ResetPendingEdit();
        result = 0;

        void* button = reinterpret_cast<void*>(self->field_001c);
        self->field_0049 = 0;
        if (button)
            result = reinterpret_cast<SetWidgetState>(
                CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 0, 1, 1);

        button = reinterpret_cast<void*>(self->field_0020);
        if (button)
            return reinterpret_cast<SetWidgetState>(
                CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 1, 1, 1);
        return result;
    }

    if (self->field_0024)
    {
        const char* menuName = g_pszMenu_P_LbSlCu;
        g_MpNetworkType = 2;
        void* menu = reinterpret_cast<void*>(self->field_0034);
        reinterpret_cast<SetMenuName>(CSF_AT(void**, menu, 0)[88 / 4])(
            menu, nullptr, menuName);
        menu = reinterpret_cast<void*>(self->field_0034);
        reinterpret_cast<OpenMenu>(CSF_AT(void**, menu, 0)[96 / 4])(
            menu, nullptr, self, 0);
        self->field_0024 = 0;
    }
    else if (self->field_0025)
    {
        const char* menuName = g_pszMenu_P_LbSlCu;
        g_MpNetworkType = 3;
        void* menu = reinterpret_cast<void*>(self->field_0034);
        reinterpret_cast<SetMenuName>(CSF_AT(void**, menu, 0)[88 / 4])(
            menu, nullptr, menuName);
        menu = reinterpret_cast<void*>(self->field_0034);
        reinterpret_cast<OpenMenu>(CSF_AT(void**, menu, 0)[96 / 4])(
            menu, nullptr, self, 0);
        self->field_0025 = 0;
    }
    else if (self->field_0026)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        reinterpret_cast<SetNetworkMode>(CSF_AT(void**, state, 0)[68 / 4])(
            state, nullptr, 1);
        state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        if (reinterpret_cast<QueryInt>(CSF_AT(void**, state, 0)[28 / 4])(
                state, nullptr))
        {
            alignas(8) uint8_t requestBytes[0xB4];
            MenuBuffer_Init(requestBytes, nullptr);
            requestBytes[24] = 1;
            void* loader = reinterpret_cast<void*>(GetGlobalState_50D190());
            GameBddLoader_DispatchMenuRequest(
                loader, nullptr, 1480735031, 128, 0,
                reinterpret_cast<int>(requestBytes));
            self->field_0049 = 1;
            SceneLoader_DestroyMenuBuffer(
                reinterpret_cast<std::uint32_t*>(requestBytes), nullptr);
        }
        else
        {
            MenuAccounts_ResetPendingEdit();
            void* button = reinterpret_cast<void*>(self->field_001c);
            if (button)
                reinterpret_cast<SetWidgetState>(
                    CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 0, 1, 1);
            button = reinterpret_cast<void*>(self->field_0020);
            if (button)
                reinterpret_cast<SetWidgetState>(
                    CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 1, 1, 1);
        }
        self->field_0026 = 0;
    }
    else if (!self->field_0027 && self->field_0028)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        CSF_AT(uint8_t, state, 0xD0) = 0;
        const char* menuName = g_pszMenu_P_MePrin;
        g_MpNetworkType = 4;
        void* menu = reinterpret_cast<void*>(self->field_0034);
        reinterpret_cast<SetMenuName>(CSF_AT(void**, menu, 0)[88 / 4])(
            menu, nullptr, menuName);
        menu = reinterpret_cast<void*>(self->field_0034);
        reinterpret_cast<OpenMenu>(CSF_AT(void**, menu, 0)[96 / 4])(
            menu, nullptr, self, 0);
        self->field_0028 = 0;
    }

    void* selection = reinterpret_cast<void*>(self->field_0038);
    uint32_t selectedButton = static_cast<uint32_t>(
        reinterpret_cast<QueryInt>(CSF_AT(void**, selection, 0)[260 / 4])(
            selection, nullptr));

    if (selectedButton == self->field_0014 ||
        selectedButton == self->field_0018)
    {
        uint32_t descriptionKey =
            selectedButton == self->field_0014 ? 1280782387 : 1280782388;
        if (descriptionKey != self->field_0030 && self->field_002c)
        {
            self->field_0030 = descriptionKey;
            int description = CoreParser_ResolveEntryByKey(descriptionKey, 1);
            if (StringFormat_CompareWideAgainstNarrow(
                    reinterpret_cast<uint16_t*>(description),
                    reinterpret_cast<char*>(&g_byte_82E65C)))
            {
                C_00720DF0_ForwardCallToChildVtblSlot16(
                    reinterpret_cast<void*>(self->field_002c), nullptr,
                    reinterpret_cast<const void*>(description));
            }
        }
    }
    else if (self->field_002c)
    {
        alignas(8) uint8_t emptyTextBytes[0x1C];
        void* text = reinterpret_cast<void*>(S_004f21d0_SetPattern(
            reinterpret_cast<S_004f21d0*>(emptyTextBytes), nullptr,
            &g_byte_82E65C));
        const void* characters;
        if (CSF_AT(uint32_t, text, 0x18) < 8u)
            characters = static_cast<uint8_t*>(text) + 4;
        else
            characters = reinterpret_cast<const void*>(CSF_AT(uint32_t, text, 4));
        C_00720DF0_ForwardCallToChildVtblSlot16(
            reinterpret_cast<void*>(self->field_002c), nullptr, characters);
        SaveGame_DestroyLocalizedString(emptyTextBytes, nullptr);
        self->field_0030 = -1;
    }

    int result = GetGlobalState_6A6D70();
    if (result)
    {
        void* state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
        result = reinterpret_cast<QueryBool>(CSF_AT(void**, state, 0)[88 / 4])(
            state, nullptr);
        if (!result && self->field_0048)
        {
            void* button = reinterpret_cast<void*>(self->field_0018);
            reinterpret_cast<SetWidgetState>(
                CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 1, 1, 1);

            state = reinterpret_cast<void*>(GetGlobalState_6A6D70());
            bool pending = reinterpret_cast<QueryInt>(
                CSF_AT(void**, state, 0)[28 / 4])(state, nullptr) != 0;

            button = reinterpret_cast<void*>(self->field_0020);
            if (!pending)
            {
                reinterpret_cast<SetWidgetState>(
                    CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 1, 1, 1);
                button = reinterpret_cast<void*>(self->field_001c);
                result = reinterpret_cast<SetWidgetState>(
                    CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 0, 1, 1);
            }
            else
            {
                reinterpret_cast<SetWidgetState>(
                    CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 0, 1, 1);
                button = reinterpret_cast<void*>(self->field_001c);
                result = reinterpret_cast<SetWidgetState>(
                    CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 1, 1, 1);
            }
            self->field_0048 = 0;
        }
    }
    else
    {
        void* button = reinterpret_cast<void*>(self->field_0018);
        if (button)
            result = reinterpret_cast<SetWidgetState>(
                CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 1, 1, 1);
        button = reinterpret_cast<void*>(self->field_0020);
        if (button)
            result = reinterpret_cast<SetWidgetState>(
                CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 1, 1, 1);
        button = reinterpret_cast<void*>(self->field_001c);
        if (button)
            return reinterpret_cast<SetWidgetState>(
                CSF_AT(void**, button, 0)[104 / 4])(button, nullptr, 0, 1, 1);
    }
    return result;
}

HOOK(0x005659B0, CMenuMpNetworkType_Update, "auto");
