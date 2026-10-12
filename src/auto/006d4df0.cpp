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

// Forwards one 0x3C-byte entry plus a forwarded argument to the per-entry worker.
CSF_FUNC(int, __stdcall, sub_6D4C70, 0x006D4C70, (void* a1, void* a2));

// Destroys one already-constructed 0x3C-byte entry.
CSF_FUNC(char, __stdcall, sub_6D49D0, 0x006D49D0, (int a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Advances a pointer by one 0x3C-byte (60-byte) entry.
static std::uint8_t* ClumpDict_NextEntry(std::uint8_t* entry)
{
    return entry + 0x3C;
}

// Forwards `forwardArg` to each of `count` consecutive 0x3C-byte entries starting at
// `first`. If a forward call throws, the entries already processed are destroyed
// again and the exception is rethrown. Caller cleans the stack (__cdecl, ret 0).
static int __cdecl ClumpDict_ForwardToEntries(
    std::uint8_t* first,
    std::uint32_t count,
    void* forwardArg,
    void* /*contextObject*/,
    int /*unusedArg*/)
{
    std::uint8_t* currentEntry = first;
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = sub_6D4C70(currentEntry, forwardArg);
            --count;
            currentEntry = ClumpDict_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            sub_6D49D0(static_cast<int>(reinterpret_cast<std::uintptr_t>(entry)));
            entry = ClumpDict_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x006D4DF0, ClumpDict_ForwardToEntries, "auto");
