#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Forwards a message to one widget entry if its slot is non-null.
CSF_FUNC(int, __stdcall, Hud_ForwardToWidgetEntryIfNonNull, 0x00433EF0,
         (int a1, void* a2));

// Destroys one already-constructed 48-byte widget entry.
CSF_FUNC(int, __stdcall, DestroyRecord_469270, 0x00469270,
         (void* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Advances a pointer by one 48-byte (0x30) widget entry.
static C_0082d410* HudWidgets_NextEntry(C_0082d410* entry)
{
    return reinterpret_cast<C_0082d410*>(reinterpret_cast<std::uint8_t*>(entry) + 0x30);
}

// Forwards `forwardArg` to each of `count` consecutive widget entries starting at
// `first`. If a forward call throws, the entries already processed are destroyed
// again and the exception is rethrown.
static int __cdecl HudWidgets_ForwardToEntries(
    C_0082d410* first,
    std::uint32_t count,
    C_0082d410* forwardArg,
    void* /*context*/)
{
    C_0082d410* currentEntry = first;
    int result;

    try
    {
        while (count != 0)
        {
            result = Hud_ForwardToWidgetEntryIfNonNull(
                static_cast<int>(reinterpret_cast<std::uintptr_t>(currentEntry)),
                static_cast<void*>(forwardArg));
            --count;
            currentEntry = HudWidgets_NextEntry(currentEntry);
        }
    }
    catch (...)
    {
        C_0082d410* entry = first;
        while (entry != currentEntry)
        {
            DestroyRecord_469270(static_cast<void*>(entry));
            entry = HudWidgets_NextEntry(entry);
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x004340C0, HudWidgets_ForwardToEntries, "auto");
