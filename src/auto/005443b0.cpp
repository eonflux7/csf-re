#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(const char*, g_pszMenu_P_Edit, 0x00899B00);

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, CMenuProfiles_SyncButtonsToProfileCount, 0x0053BF50, (CMenuProfiles* self, void*));
CSF_FUNC(char, __fastcall, CMenuProfiles_CheckEnteredProfileName, 0x0053C190, (void* self, void* edx, void* a2));
CSF_FUNC(char, __fastcall, CMenuProfiles_CheckProfileNameConflict, 0x0053C3C0, (void* self, void* edx, unsigned int* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static char __fastcall CMenuProfilesPC_HandleEditResult(
    CMenuProfilesPC* self, void* /*edx*/, int event)
{
    void* screens = reinterpret_cast<void*>(self->field_002c);
    void* editor =
        reinterpret_cast<void* (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, screens, 0)[0x5c / 4])(
                screens, nullptr, g_pszMenu_P_Edit);
    if (!editor)
        return 0;

    int editorId =
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, editor, 0)[0x20 / 4])(editor, nullptr);
    if (editorId != CSF_AT(int, reinterpret_cast<void*>(event), 0x04))
        return static_cast<char>(editorId);

    const char confirmed = CSF_AT(char, editor, 0x30);
    alignas(8) std::uint8_t profileNameBytes[0x1C];
    S_00696af0& profileName =
        *reinterpret_cast<S_00696af0*>(profileNameBytes);

    profileName.field_0018 = 7;
    CSF_AT(std::uint32_t, profileNameBytes, 0x14) = 0;
    *reinterpret_cast<std::uint16_t*>(&profileName.field_0004) = 0;
    AssignStringRange_488830(
        &profileName, nullptr,
        static_cast<std::uint8_t*>(editor) + 0x34, 0, 0xFFFFFFFF);

    char result;
    bool closeEditor = !confirmed;
    if (confirmed)
    {
        result = CMenuProfiles_CheckEnteredProfileName(
            self, nullptr, &profileName);
        if (!result)
        {
            result = CMenuProfiles_CheckProfileNameConflict(
                self, nullptr, reinterpret_cast<unsigned int*>(&profileName));
            if (!result)
            {
                void* saveRequest = reinterpret_cast<void*>(self->field_0028);
                reinterpret_cast<void (__fastcall*)(void*, void*, unsigned int*)>(
                    CSF_AT(void**, saveRequest, 0)[0x18 / 4])(
                        saveRequest, nullptr,
                        reinterpret_cast<unsigned int*>(&profileName));
                CMenuProfiles_SyncButtonsToProfileCount(self, nullptr);
                closeEditor = true;
            }
        }
    }

    if (closeEditor)
    {
        void* menu = reinterpret_cast<void*>(self->field_0024);
        result = static_cast<char>(
            reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, menu, 0)[0xa8 / 4])(menu, nullptr, 0));
        self->field_0038 = 0xFFFFFFFF;
    }

    if (profileName.field_0018 >= 8)
    {
        result = static_cast<char>(profileName.field_0004);
        if (profileName.field_0004)
            result = static_cast<char>(Mem_FreeDebug(
                reinterpret_cast<void*>(profileName.field_0004)));
    }
    return result;
}

HOOK(0x005443B0, CMenuProfilesPC_HandleEditResult, "auto");
