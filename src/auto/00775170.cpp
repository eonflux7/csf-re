#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Processes one 60-byte (0x3C) entry with a forwarded argument; result returned in eax.
CSF_FUNC(char*, __stdcall, Entry60_Process, 0x00774EF0, (char* a1, int a2));

// Destroys one already-constructed 60-byte (0x3C) entry.
CSF_FUNC(char, __stdcall, Entry60_Destroy, 0x00774F10, (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Advances a pointer by one 60-byte (0x3C) entry.
static std::uint8_t* Entries60_NextEntry(std::uint8_t* entry)
{
    return entry + 0x3C;
}

// Forwards `forwardArg` to each of `count` consecutive 60-byte entries starting at
// `first`, returning the last result. If a forward call throws, the entries already
// processed are destroyed again and the exception is rethrown.
static uint32_t* __cdecl Entries60_ForwardToRange(
    std::uint8_t* first,
    std::uint32_t count,
    uint32_t* forwardArg,
    void* contextObject,
    int /*unusedArg5*/)
{
    std::uint8_t* currentEntry = first;
    uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = reinterpret_cast<uint32_t*>(
                Entry60_Process(reinterpret_cast<char*>(currentEntry),
                                reinterpret_cast<int>(forwardArg)));
            --count;
            currentEntry = Entries60_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            Entry60_Destroy(static_cast<int>(reinterpret_cast<std::uintptr_t>(entry)));
            entry = Entries60_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00775170, Entries60_ForwardToRange, "auto");
