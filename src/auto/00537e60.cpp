#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, StateRecord_Reset, 0x00401870, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00830578_InitializeCheatScreen, 0x0042B380, (C_00830578* self, void*));
CSF_FUNC(int, __cdecl, Interface_BeginSceneLoad, 0x00442790, ());
CSF_FUNC(int*, __cdecl, Interface_PrepareMapLoad, 0x004429D0, ());
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B0568, 0x004FDBB0, ());
CSF_FUNC(char, __fastcall, S_00500350_PrepareSceneLoad, 0x00501B90, (S_00500350* self, void*));
CSF_FUNC(char, __fastcall, CSceneLoader_LoadMap, 0x00504970, (C_0082d410* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_519420, 0x00519420, ());
CSF_FUNC(char, __fastcall, Interface_UpdateTransitionTime, 0x00519430, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(void, __fastcall, S_00525f70_RefreshThenSetLevel, 0x00526300, (S_00525f70* self, void*, float a2));
CSF_FUNC(int, __fastcall, Interface_SetSceneMenuMode, 0x00536650, (void* self, void* edx, char a2));
CSF_FUNC(char, __fastcall, Interface_InitializeLoadedScene, 0x00537650, (void* self, void* edx));
CSF_FUNC(uint32_t, __fastcall, C_00830578_PrepareSceneTransition, 0x00537D90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_ClearFlag14A2, 0x00542780, ());
CSF_FUNC(int, __cdecl, MenuMpCharacterSelect_CommitCharacterSelection, 0x00542820, ());
CSF_FUNC(int, __cdecl, SceneLoader_SetActiveContext, 0x00697360, (int a1));
CSF_FUNC(void*, __cdecl, CoreFiles_SetFileOption, 0x00698600, (char locked));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static inline __attribute__((always_inline)) void Interface_DispatchStateRecord(
    C_0082d478& record, int state)
{
    uint32_t* resetRecord = StateRecord_Reset(&record, nullptr, state);
    void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
    DispatchRecordToGlobalObject(dispatcher, nullptr, resetRecord, 1);
    record.field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    record.field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    record.vtbl = reinterpret_cast<decltype(record.vtbl)>(CSF_VTBL(0x0082D410));
}

static char __fastcall C_00830578_UpdateSceneTransition(
    C_00830578* self, void* /*edx*/, float elapsed)
{
    alignas(8) uint8_t activeRecordBytes[0x34];
    alignas(8) uint8_t specialRecordBytes[0x34];
    alignas(8) uint8_t transitionRecordBytes[0x34];
    C_0082d478& activeRecord =
        *reinterpret_cast<C_0082d478*>(activeRecordBytes);
    C_0082d478& specialRecord =
        *reinterpret_cast<C_0082d478*>(specialRecordBytes);
    C_0082d478& transitionRecord =
        *reinterpret_cast<C_0082d478*>(transitionRecordBytes);

    auto* scene = reinterpret_cast<S_00500350*>(GetGlobalValue_8B0568());
    if (scene->field_005c != 1)
    {
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            if (CSF_AT(uint8_t, self, 0x17F))
            {
                void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                int context = CSF_AT(int, state, 0xA4);
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    self->vtbl[0x7C / 4])(self, nullptr, context);
            }
            else
            {
                Interface_DispatchStateRecord(activeRecord, 0);
                CSF_AT(uint8_t, self, 0x17F) = 1;
            }
        }
        else if (CSF_AT(uint8_t, self, 0x108))
        {
            if (!CSF_AT(uint8_t, self, 0x17F))
            {
                if (CSF_AT(uint8_t,
                           reinterpret_cast<void*>(GetGlobalValue_75FDF0()), 0xCF))
                {
                    Interface_DispatchStateRecord(specialRecord, 43);
                }
                Interface_DispatchStateRecord(transitionRecord, 0);
                CSF_AT(uint8_t, self, 0x17F) = 1;
            }
            else
            {
                void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                int context = CSF_AT(int, state, 0xA4);
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    self->vtbl[0x7C / 4])(self, nullptr, context);
            }
        }

        if (elapsed != g_fZero)
        {
            auto* level = reinterpret_cast<S_00525f70*>(Get_008b0678());
            S_00525f70_RefreshThenSetLevel(level, nullptr, elapsed);
        }

        int elapsedBits;
        std::memcpy(&elapsedBits, &elapsed, sizeof(elapsedBits));
        void* transition = reinterpret_cast<void*>(GetGlobalValue_519420());
        Interface_UpdateTransitionTime(transition, nullptr, elapsedBits);

        char result = static_cast<char>(CSF_AT(uint8_t, self, 0x17C));
        if (result)
        {
            CSF_AT(uint8_t, self, 0x17C) = 0;
            int stateAddress = GetGlobalState_50D190();
            result = static_cast<char>(stateAddress);
            if (CSF_AT(uint8_t, reinterpret_cast<void*>(stateAddress), 0xD0))
            {
                CSF_AT(uint8_t, self, 0x182) = 1;
                void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
                int context = CSF_AT(int, state, 0xA4);
                result = static_cast<char>(
                    reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                        self->vtbl[0x7C / 4])(self, nullptr, context));
            }
        }
        return result;
    }

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        C_00830578_PrepareSceneTransition(self, nullptr);

    C_00830578_InitializeCheatScreen(self, nullptr);
    CoreFiles_SetFileOption(0);
    char prepared = S_00500350_PrepareSceneLoad(scene, nullptr);
    CSF_AT(char, self, 0xFC) = prepared;

    if (!prepared)
    {
        return static_cast<char>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                self->vtbl[0x9C / 4])(self, nullptr));
    }

    Interface_BeginSceneLoad();
    Interface_PrepareMapLoad();
    char loaded = CSceneLoader_LoadMap(
        reinterpret_cast<C_0082d410*>(scene), nullptr);
    CSF_AT(char, self, 0xFC) = loaded;

    if (!loaded)
    {
        return static_cast<char>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                self->vtbl[0x9C / 4])(self, nullptr));
    }

    if (reinterpret_cast<char(__fastcall*)(void*, void*)>(
            self->vtbl[0x8C / 4])(self, nullptr))
    {
        void* loadingMenu = CSF_AT(void*, self, 0x138);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, loadingMenu, 0)[0xA8 / 4])(
                loadingMenu, nullptr, 0);

        CSF_AT(uint8_t, self, 0x141) = 1;
        CSF_AT(uint8_t, self, 0x140) = 1;
        Interface_InitializeLoadedScene(self, nullptr);

        if (!CSF_AT(uint8_t,
                    reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                self->vtbl[0xA4 / 4])(self, nullptr);
        }

        SceneLoader_SetActiveContext(0);
        void* menu = reinterpret_cast<void*>(self->field_0018);
        CSF_AT(uint32_t, self, 0x134) = 1;
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, menu, 0)[0x9C / 4])(menu, nullptr, 1);

        menu = reinterpret_cast<void*>(self->field_0018);
        return static_cast<char>(
            reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, menu, 0)[0xA8 / 4])(menu, nullptr));
    }

    if (CSF_AT(uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0))
    {
        void* menu = reinterpret_cast<void*>(self->field_0018);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, menu, 0)[0xAC / 4])(menu, nullptr, 0);
    }

    void* loadingMenu = CSF_AT(void*, self, 0x138);
    if (loadingMenu)
    {
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, loadingMenu, 0)[0xA8 / 4])(
                loadingMenu, nullptr, 1);
        loadingMenu = CSF_AT(void*, self, 0x138);
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, loadingMenu, 0)[0x58 / 4])(
                loadingMenu, nullptr);
    }

    CSF_AT(uint8_t, self, 0x141) = 0;
    char result = static_cast<char>(
        Interface_SetSceneMenuMode(self, nullptr, 1));
    void* characterSelect = CSF_AT(void*, self, 0x14C);
    CSF_AT(uint32_t, self, 0x134) = 2;
    if (characterSelect)
    {
        MenuMpCharacterSelect_ClearFlag14A2();
        result = static_cast<char>(
            MenuMpCharacterSelect_CommitCharacterSelection());
    }
    return result;
}

HOOK(0x00537E60, C_00830578_UpdateSceneTransition, "auto");
