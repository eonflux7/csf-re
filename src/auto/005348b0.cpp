#include <cstdint>
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0,
         (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(void*, __fastcall, Profiles_AppendStringRange, 0x0043EB30, (void* self, int edx, void* text, unsigned int count));
CSF_FUNC(int, __fastcall, MenuMpPause_FindFirstCharacterNotInSet, 0x00533C90,
         (void* self, void* edx, void* characterSet, std::uint32_t position, std::size_t setLength));
CSF_FUNC(int, __fastcall, MenuMpPause_FindSubstring, 0x00533BC0,
         (void* self, void* edx, char* needle, std::uint32_t position, std::uint32_t needleLength));
CSF_FUNC(void*, __fastcall, MenuManager_AppendPageName, 0x00470FD0, (S_00470ab0* self, void*, void* name));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// The game's narrow string (0x1C bytes): +4 inline buffer or heap pointer, +0x14 length, +0x18 capacity.
struct MenuPageString
{
    std::uint32_t allocator;
    union { char text[16]; char* heap; };
    std::uint32_t length;
    std::uint32_t capacity;

    __attribute__((always_inline)) MenuPageString() : allocator(0), length(0), capacity(15) { text[0] = 0; }
    __attribute__((always_inline)) void Release()
    {
        if (capacity >= 0x10 && heap)
            Mem_FreeDebug(heap);
        capacity = 15;
        length = 0;
        text[0] = 0;
    }
};

// Splits `source` at spaces and appends every word to `pages` as a page name.
// The original's one-byte delimiter set has no terminator, so its strlen runs into the saved frame pointer
// (set length 1..4 at random); the set here is the intended single space.
static int __cdecl MenuMpPause_AppendSpaceSeparatedPageNames(void* source, S_00470ab0* pages)
{
    char space = ' ';
    MenuPageString line;
    AssignStringSubstring(&line, nullptr, source, 0, 0xFFFFFFFFu);
    Profiles_AppendStringRange(&line, 0, const_cast<char*>(CSF_STR(0x0083F284, " ")), 1);

    int start = MenuMpPause_FindFirstCharacterNotInSet(&line, nullptr, &space, 0, 1);
    int end = MenuMpPause_FindSubstring(&line, nullptr, &space, static_cast<std::uint32_t>(start), 1);
    while (end != -1)
    {
        MenuPageString word;
        AssignStringSubstring(&word, nullptr, &line, static_cast<std::uint32_t>(start),
                              static_cast<std::uint32_t>(end - start));
        MenuManager_AppendPageName(pages, nullptr, &word);
        word.Release();

        start = MenuMpPause_FindFirstCharacterNotInSet(&line, nullptr, &space, static_cast<std::uint32_t>(end), 1);
        if (start == -1)
            break;
        end = MenuMpPause_FindSubstring(&line, nullptr, &space, static_cast<std::uint32_t>(start), 1);
    }
    line.Release();
    return 0;
}

HOOK(0x005348B0, MenuMpPause_AppendSpaceSeparatedPageNames, "auto");
