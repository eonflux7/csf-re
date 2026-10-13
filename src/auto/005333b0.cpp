#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(int, __fastcall, MenuEula_FindWideSubstring, 0x00532BC0, (void* self, void* , std::uint16_t* pattern, std::uint64_t packedSearch));
CSF_FUNC(int, __fastcall, MenuEula_AddStringWithLength, 0x00532C80, (void* self, void* , wchar_t* String, unsigned int a3));
CSF_FUNC(uint32_t*, __fastcall, MenuEula_ComputeWriteCursorForEndPointer, 0x00532CB0, (void* thisPtr, void* , uint32_t* out, uint32_t endAddr));
CSF_FUNC(void*, __fastcall, ReplaceTextRange, 0x00533040, (void* self, void* edx, unsigned int a2, unsigned int a3, void* a4, unsigned int a5, unsigned int a6));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(std::uint32_t*, __fastcall, S_004f21d0_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(int, __fastcall, S_004f21d0_SetPattern, 0x00697100, (S_004f21d0* self, void* , uint8_t* a2));
CSF_FUNC(void, __cdecl, CoreFiles_CloseSceneFile, 0x00697980, (int file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(int, __cdecl, CoreFiles_CheckPageAvailability, 0x00698B20, (const char* path, char bypassList));
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0, (void* self, void* edx, void* text));
CSF_FUNC(size_t, __cdecl, _wcslen, 0x007B0499, (const wchar_t* str));

CSF_GLOBAL(const char, g_FirstWrapPattern, 0x0083AB1C);
CSF_GLOBAL(const uint8_t, g_EulaSearchPattern, 0x00841C18);

static int __fastcall CMenuEula_LoadLicenseText(CMenuEula* self, void* /*edx*/)
{
    // Explicit binding requested by the packet for the substring-search reference.
    (void)CSF_VTBL(0x00532BC0);

    int result = static_cast<int>(self->m_textControl);
    if (!result)
        return result;

    const char* filename =
        reinterpret_cast<const char*>(self) + 0x58;
    result = CoreFiles_CheckPageAvailability(filename, 0);
    if (static_cast<uint8_t>(result) == 0)
        return result;

    void* file = CoreFiles_OpenSceneFile(
        filename, const_cast<char*>(CSF_STR(0x0083E130, "r")), 0);
    if (!file)
        return 0;

    const uint32_t byteCount =
        2 * (((uint32_t(__fastcall*)(void*, void*))
            CSF_AT(void**, file, 0)[0x1C / 4])(file, nullptr) >> 1);

    auto* text = reinterpret_cast<int16_t*>(
        static_cast<uintptr_t>(CoreMemory_AllocUntagged(byteCount + 2)));

    ((void(__fastcall*)(void*, void*, int16_t*, uint32_t))
        CSF_AT(void**, file, 0)[0x08 / 4])(
            file, nullptr, text, byteCount);

    if (text[0] != static_cast<int16_t>(-257))
    {
        CoreMemory_Free(text);
        CoreFiles_CloseSceneFile(
            static_cast<int>(reinterpret_cast<uintptr_t>(file)));
        return 0;
    }

    text[byteCount / 2] = 0;
    auto* licenseString = reinterpret_cast<uint8_t*>(self) + 0x3C;
    S_004f21d0_AssignText(
        reinterpret_cast<uint32_t*>(licenseString), nullptr, text);
    CoreMemory_Free(text);

    const uint32_t stringAddress =
        CSF_AT(uint32_t, self, 0x54) < 8
            ? static_cast<uint32_t>(
                reinterpret_cast<uintptr_t>(
                    reinterpret_cast<uint8_t*>(self) + 0x40))
            : CSF_AT(uint32_t, self, 0x40);

    alignas(8) uint8_t cursorBytes[0x0C];
    MenuEula_ComputeWriteCursorForEndPointer(
        licenseString, nullptr,
        reinterpret_cast<uint32_t*>(cursorBytes), stringAddress);

    alignas(8) uint8_t replacementBytes[0x1C];
    alignas(8) uint8_t searchBytes[0x1C];
    auto& replacement =
        *reinterpret_cast<S_004f21d0*>(replacementBytes);
    auto& search =
        *reinterpret_cast<S_004f21d0*>(searchBytes);

    S_004f21d0_SetPattern(
        &replacement, nullptr,
        reinterpret_cast<uint8_t*>(const_cast<char*>(&g_FirstWrapPattern)));
    S_004f21d0_SetPattern(
        &search, nullptr, const_cast<uint8_t*>(&g_EulaSearchPattern));

    wchar_t* pattern = search.field_0018 < 8
        ? reinterpret_cast<wchar_t*>(&search.field_0004)
        : reinterpret_cast<wchar_t*>(search.field_0004);

    uint32_t position = static_cast<uint32_t>(
        MenuEula_AddStringWithLength(licenseString, nullptr, pattern, 0));

    while (position != 0xFFFFFFFF)
    {
        ReplaceTextRange(
            licenseString, nullptr, position, search.field_0014,
            &replacement, 0, 0xFFFFFFFF);

        pattern = search.field_0018 < 8
            ? reinterpret_cast<wchar_t*>(&search.field_0004)
            : reinterpret_cast<wchar_t*>(search.field_0004);

        const size_t patternLength = _wcslen(pattern);
        const uint64_t packedSearch =
            (static_cast<uint64_t>(patternLength) << 32) | position;
        position = static_cast<uint32_t>(
            MenuEula_FindWideSubstring(
                licenseString, nullptr,
                reinterpret_cast<uint16_t*>(pattern), packedSearch));
    }

    void* displayText = CSF_AT(uint32_t, licenseString, 0x18) < 8
        ? static_cast<void*>(licenseString + 4)
        : CSF_AT(void*, licenseString, 4);

    C_00720DF0_ForwardCallToChildVtblSlot16(
        reinterpret_cast<void*>(self->m_textControl), nullptr, displayText);
    SaveGame_DestroyLocalizedString(&search, nullptr);
    SaveGame_DestroyLocalizedString(&replacement, nullptr);
    CoreFiles_CloseSceneFile(
        static_cast<int>(reinterpret_cast<uintptr_t>(file)));
    return 0;
}

HOOK(0x005333B0, CMenuEula_LoadLicenseText, "auto");
