#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, AppendStringSubstring, 0x0043EA30, (void* self, void* edx, uint32_t* a2, unsigned int a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __cdecl, CoreFiles_SelectArchive, 0x00698960, (char* filename));
CSF_FUNC(int*, __cdecl, GetGlobalIntPtr, 0x0069C580, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_pszMenu_P_Intro, 0x00899B2C);

static void __fastcall CMenuLanguage_ApplyPendingLanguage(
    CMenuLanguage* self, void* /*edx*/, int /*unused*/)
{
    if (self->field_0058)
    {
        void* archiveName = reinterpret_cast<uint8_t*>(self) + 0x3C;
        int configuration;

        if (self->field_0014 &&
            static_cast<int32_t>(self->field_0018 - self->field_0014) / 28 == 1)
        {
            AssignStringRange_429B60(
                archiveName, nullptr,
                const_cast<char*>(CSF_STR(0x00841C74, "Lang/")), 5);
            AppendStringSubstring(
                archiveName, nullptr,
                reinterpret_cast<uint32_t*>(self->field_0014), 0, -1);
            Profiles_AppendStringRange(
                archiveName, 0,
                const_cast<char*>(CSF_STR(0x00830FEC, ".pak")), 4);
            configuration = GetGlobalValue_8BC1E0();
        }
        else
        {
            if (self->field_0014 &&
                static_cast<int32_t>(self->field_0018 - self->field_0014) / 28)
            {
                self->field_0058 = 0;
                return SceneLoader_NoOp();
            }

            AssignStringRange_429B60(
                archiveName, nullptr,
                reinterpret_cast<char*>(&g_byte_82E65C), 0);
            configuration = GetGlobalValue_8BC1E0();
        }

        if (CSF_AT(uint8_t, reinterpret_cast<void*>(configuration), 0xA5C))
        {
            char* filename = reinterpret_cast<char*>(GetGlobalIntPtr());
            CoreFiles_SelectArchive(filename);
        }

        void* state = reinterpret_cast<void*>(GetGlobalState_50D190());
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, state, 0)[0x24 / 4])(state, nullptr);

        self->vReleaseSubObject();

        void* menuManager = reinterpret_cast<void*>(self->field_0034);
        reinterpret_cast<void(__fastcall*)(void*, void*, CMenuLanguage*, int)>(
            CSF_AT(void**, menuManager, 0)[0x60 / 4])(
                menuManager, nullptr, self, 0);

        menuManager = reinterpret_cast<void*>(self->field_0034);
        const char* introMenu = g_pszMenu_P_Intro;
        reinterpret_cast<void(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menuManager, 0)[0x58 / 4])(
                menuManager, nullptr, introMenu);

        self->field_0058 = 0;
    }

    SceneLoader_NoOp();
}

HOOK(0x00534110, CMenuLanguage_ApplyPendingLanguage, "auto");
