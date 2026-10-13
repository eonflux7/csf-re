#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int a2, void* a3, unsigned int a4));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void* , void* a3));
CSF_FUNC(int, __fastcall, MenuMpPause_FindSubstring, 0x00533BC0, (void* self, void* , char* pattern, std::uint64_t searchArguments));
CSF_FUNC(int, __fastcall, MenuMpPause_FindFirstCharacterNotInSet, 0x00533C90, (void* self, void* , void* characterSet, std::uint32_t position, std::size_t setLength));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __cdecl MenuMpPause_AppendSpaceSeparatedPageNames(
    C_0082d410* source, S_00470ab0* destination)
{
    alignas(8) std::uint8_t tokenBytes[0x1C];
    alignas(8) std::uint8_t textBytes[0x1C];
    C_0082d410& token = *reinterpret_cast<C_0082d410*>(tokenBytes);
    C_0082d410& text = *reinterpret_cast<C_0082d410*>(textBytes);
    char pattern[7];

    // The original scans beyond its single-byte delimiter. Use zero
    // for the otherwise indeterminate trailing bytes.
    char characterSet[2] = {32, 0};

    CSF_AT(std::uint32_t, &text, 0x18) = 15;
    CSF_AT(std::uint32_t, &text, 0x14) = 0;
    *reinterpret_cast<std::uint8_t*>(&text.field_0004) = 0;

    AssignStringSubstring(&text, nullptr, source, 0, -1);
    Profiles_AppendStringRange(
        &text, 0, const_cast<char*>(CSF_STR(0x0083F284, " ")), 1);

    std::uint32_t tokenStart = MenuMpPause_FindFirstCharacterNotInSet(
        &text, nullptr, characterSet, 0, std::strlen(characterSet));

    pattern[0] = 32;
    int result = MenuMpPause_FindSubstring(
        &text, nullptr, pattern,
        static_cast<std::uint64_t>(tokenStart) | 0x100000000ULL);

    while (result != -1)
    {
        const std::uint32_t tokenEnd = static_cast<std::uint32_t>(result);

        CSF_AT(std::uint32_t, &token, 0x18) = 15;
        CSF_AT(std::uint32_t, &token, 0x14) = 0;
        *reinterpret_cast<std::uint8_t*>(&token.field_0004) = 0;

        AssignStringSubstring(
            &token, nullptr, &text, tokenStart, tokenEnd - tokenStart);
        MenuManager_AppendPageName(destination, nullptr, &token);

        if (CSF_AT(std::uint32_t, &token, 0x18) >= 0x10 &&
            token.field_0004 != 0)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(token.field_0004));
        }

        CSF_AT(std::uint32_t, &token, 0x18) = 15;
        CSF_AT(std::uint32_t, &token, 0x14) = 0;
        *reinterpret_cast<std::uint8_t*>(&token.field_0004) = 0;

        result = MenuMpPause_FindFirstCharacterNotInSet(
            &text, nullptr, characterSet, tokenEnd,
            std::strlen(characterSet));
        if (result == -1)
            break;

        tokenStart = static_cast<std::uint32_t>(result);
        pattern[0] = 32;
        result = MenuMpPause_FindSubstring(
            &text, nullptr, pattern,
            static_cast<std::uint64_t>(tokenStart) | 0x100000000ULL);
    }

    if (CSF_AT(std::uint32_t, &text, 0x18) >= 0x10)
    {
        result = static_cast<std::int32_t>(text.field_0004);
        if (text.field_0004 != 0)
            return Mem_FreeDebug(reinterpret_cast<void*>(text.field_0004));
    }
    return result;
}

HOOK(0x005348B0, MenuMpPause_AppendSpaceSeparatedPageNames, "auto");
