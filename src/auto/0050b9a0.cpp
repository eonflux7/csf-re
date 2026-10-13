#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());
CSF_FUNC(unsigned int, __cdecl, ProfilesSavegames_GetBaseDirectory, 0x0069D580, (wchar_t* pszPath));
CSF_FUNC(std::uint32_t*, __fastcall, S_004f21d0_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (S_004f21d0* self, void* , int16_t a2));
CSF_FUNC(void*, __fastcall, AppendNarrowToWideString, 0x00697150, (void* self, void* , uint8_t* a2));

using CreateDirectoryWFunction = int (__stdcall*)(const wchar_t*, void*);
CSF_GLOBAL(CreateDirectoryWFunction, g_CreateDirectoryW, 0x0082D218);

static int __fastcall COptionsFile_InitializeOptionsPaths(COptionsFile* self, void* /*edx*/)
{
    int result = GetGlobalValue_75FDF0();
    void* globalState =
        reinterpret_cast<void*>(static_cast<std::uintptr_t>(result));

    if (!CSF_AT(std::uint8_t, globalState, 0xCC))
        return result;

    alignas(8) std::uint8_t pszPath[0x200];
    result = static_cast<int>(ProfilesSavegames_GetBaseDirectory(
        reinterpret_cast<wchar_t*>(pszPath)));
    if (!result)
        return result;

    auto* selfBytes = reinterpret_cast<std::uint8_t*>(self);

    // v3 = &this->pad_0015[3]  (offset 0x18)
    auto* pathObjV3 = reinterpret_cast<S_004f21d0*>(selfBytes + 0x18);
    // v4 = S_004f21d0_AssignText(v3, pszPath)
    auto* assignedText = reinterpret_cast<std::uint32_t*>(
        S_004f21d0_AssignText(
            reinterpret_cast<std::uint32_t*>(pathObjV3), nullptr,
            reinterpret_cast<std::int16_t*>(pszPath)));

    // pad_0034 = this->pad_0034  (offset 0x34)
    auto* pathObjPad34 = reinterpret_cast<S_004f21d0*>(selfBytes + 0x34);
    AssignStringRange_488830(pathObjPad34, nullptr, assignedText, 0, 0xFFFFFFFF);
    SaveSlots_ApplyIndex(pathObjPad34, nullptr, 92);
    AppendNarrowToWideString(
        pathObjPad34, nullptr,
        reinterpret_cast<uint8_t*>(
            const_cast<char*>(CSF_STR(0x0083FFF4, "CSF Data"))));

    SaveSlots_ApplyIndex(pathObjV3, nullptr, 92);
    AppendNarrowToWideString(
        pathObjV3, nullptr,
        reinterpret_cast<uint8_t*>(
            const_cast<char*>(CSF_STR(0x0083FFE0, "CSF Data\\Options"))));

    if (CSF_AT(std::uint32_t, pathObjPad34, 0x18) < 8)
        return g_CreateDirectoryW(
            reinterpret_cast<const wchar_t*>(selfBytes + 0x38), nullptr);
    else
        return g_CreateDirectoryW(
            CSF_AT(const wchar_t*, pathObjPad34, 0x04), nullptr);
}

HOOK(0x0050B9A0, COptionsFile_InitializeOptionsPaths, "auto");
