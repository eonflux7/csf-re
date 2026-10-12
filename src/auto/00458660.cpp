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

// Per-entry worker: applies one status-icon entry (40 bytes each) using the given context.
CSF_FUNC(void, __stdcall, StatusIcon_ApplyToEntry, 0x004584E0,
         (void* a1, void* a2));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Frame data referenced by the original's exception frame stub.
CSF_GLOBAL(std::uint8_t, g_StatusIconsFrameData, 0x008882B4);

// Walks `count` consecutive 40-byte status-icon entries starting at `entries`,
// applying each one with `context`, inside the original's C++ try block.
// The catch-all rethrows the current exception through 0x7AF666 with two zero
// arguments, matching the original's `push 0; push 0; call 0x7af666` stub.
// The original passes its fourth argument in ecx to the worker; the worker's
// binding is __stdcall, so that register value is not consumed and not forwarded.
static void __cdecl HudStatusIcons_ForEachEntry(
    char* entries,
    int count,
    const void* context,
    void* /*unusedThisArg*/,
    void* /*unusedArg*/)
{
    std::uint32_t remaining = static_cast<std::uint32_t>(count);

    try
    {
        while (remaining != 0)
        {
            StatusIcon_ApplyToEntry(entries, const_cast<void*>(context));
            --remaining;
            entries += 40;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }
}

HOOK(0x00458660, HudStatusIcons_ForEachEntry, "auto");
