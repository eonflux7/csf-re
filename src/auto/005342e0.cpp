#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(void*, __fastcall, AppendStringSubstring, 0x0043EA30, (void* self, void* edx, uint32_t* a2, unsigned int a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(void, __cdecl, CoreFiles_SelectArchive, 0x00698960, (char* filename));
CSF_FUNC(int*, __cdecl, GetGlobalIntPtr, 0x0069C580, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

CSF_GLOBAL(const char*, g_pszMenu_P_Intro, 0x00899B2C);

static int __fastcall CMenuLanguage_DispatchCommand(
    CMenuLanguage* self, void* /*edx*/, int command)
{
    void* input = reinterpret_cast<void*>(self->field_0030);
    int result =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, input, 0)[0x104 / 4])(input, nullptr);
    if (!result)
        return result;

    uint32_t entryCount = 0;
    const uint32_t entriesBegin = self->field_0024;
    if (entriesBegin)
    {
        entryCount = static_cast<uint32_t>(
            static_cast<int32_t>(self->field_0028 - entriesBegin) >> 2);
    }

    uint32_t languageOffset = 0;
    for (uint32_t index = 0; index < entryCount; ++index, languageOffset += 28)
    {
        const uint32_t entry = CSF_AT(
            uint32_t, reinterpret_cast<void*>(self->field_0024), index * 4);
        if (entry && entry == CSF_AT(uint32_t, reinterpret_cast<void*>(command), 4))
        {
            void* archiveName = reinterpret_cast<uint8_t*>(self) + 0x3C;
            AssignStringRange_429B60(
                archiveName, nullptr, const_cast<char*>(CSF_STR(0x00841C74, "Lang/")), 5);
            AppendStringSubstring(
                archiveName, nullptr,
                reinterpret_cast<uint32_t*>(self->field_0014 + languageOffset),
                0, -1);
            Profiles_AppendStringRange(
                archiveName, 0, const_cast<char*>(CSF_STR(0x00830FEC, ".pak")), 4);

            void* globalValues = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            if (CSF_AT(uint8_t, globalValues, 0xA5C))
            {
                char* filename = reinterpret_cast<char*>(GetGlobalIntPtr());
                CoreFiles_SelectArchive(filename);
            }

            void* globalState = reinterpret_cast<void*>(GetGlobalState_50D190());
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, globalState, 0)[0x24 / 4])(globalState, nullptr);
        }
    }

    self->vReleaseSubObject();

    void* menuManager = reinterpret_cast<void*>(self->field_0034);
    reinterpret_cast<void(__fastcall*)(void*, void*, CMenuLanguage*, int)>(
        CSF_AT(void**, menuManager, 0)[0x60 / 4])(menuManager, nullptr, self, 0);

    const char* introMenu = g_pszMenu_P_Intro;
    menuManager = reinterpret_cast<void*>(self->field_0034);
    return reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, menuManager, 0)[0x58 / 4])(menuManager, nullptr, introMenu);
}

HOOK(0x005342E0, CMenuLanguage_DispatchCommand, "auto");
