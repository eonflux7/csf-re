#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Processes one 44-byte (0x2C) graphics screen-effect entry if its slot is non-null.
CSF_FUNC(std::uint32_t*, __stdcall, GfxScreenEffects_ProcessEntryIfNonNull, 0x006F86D0,
         (std::uint32_t* entry, std::uint32_t* argument));

// Destroys one already-constructed screen-effect record.
CSF_FUNC(int, __stdcall, DestroyRecord, 0x0069B820,
         (std::uint32_t* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 44-byte (0x2C) screen-effect entry.
static std::uint32_t* GfxScreenEffects_NextEntry(std::uint32_t* entry)
{
    return reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint8_t*>(entry) + 0x2C);
}

// Forwards `argument` to each of `count` consecutive entries starting at `first`.
// If a forward call throws, the entries already processed are destroyed again
// and the exception is rethrown.
static std::uint32_t* __cdecl GfxScreenEffects_ForwardToEntries(
    std::uint32_t* first,
    std::uint32_t count,
    std::uint32_t* argument,
    int /*context*/)
{
    std::uint32_t* currentEntry = first;
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = GfxScreenEffects_ProcessEntryIfNonNull(currentEntry, argument);
            --count;
            currentEntry = GfxScreenEffects_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        std::uint32_t* entry = first;
        while (entry != currentEntry)
        {
            DestroyRecord(entry);
            entry = GfxScreenEffects_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x006F8780, GfxScreenEffects_ForwardToEntries, "auto");
