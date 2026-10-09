#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Widget forwarder: called once per element with (destination slot, source entry).
CSF_FUNC(int, __stdcall, Hud_ForwardToWidgetEntryIfNonNull, 0x00433EF0, (int a1, void* a2));

// Element destructor used by the cleanup path (stride 0x30 records).
CSF_FUNC(int, __stdcall, DestroyRecord_469270, 0x00469270, (void* record));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH data/code referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_ForwardRangeExceptionHandlerCode, 0x00826430);
CSF_GLOBAL(std::uint8_t, g_ForwardRangeFuncInfo, 0x00886D30);

// Walks the half-open range [source, sourceEnd) of 0x30-byte entries and forwards
// each one, together with the matching destination slot (also 0x30-byte stride
// in the destination array), to the HUD widget forwarder. If a forward throws,
// the destination slots already handled are destroyed before rethrowing.
// Returns the past-the-end destination pointer.
static int __cdecl ForwardRangeToHudWidgets(
    void* source,
    void* sourceEnd,
    void* destination,
    void* /*contextEcx*/,
    int /*unusedArg5*/)
{
    void* const destinationBegin = destination;

    try
    {
        while (source != sourceEnd)
        {
            Hud_ForwardToWidgetEntryIfNonNull(
                static_cast<int>(reinterpret_cast<std::intptr_t>(destination)),
                source);
            destination = static_cast<std::uint8_t*>(destination) + 0x30;
            source = static_cast<std::uint8_t*>(source) + 0x30;
        }
    }
    catch (...)
    {
        // Destroy every destination slot already processed in [begin, current).
        void* handled = destinationBegin;

        while (handled != destination)
        {
            DestroyRecord_469270(handled);
            handled = static_cast<std::uint8_t*>(handled) + 0x30;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x00433F10, ForwardRangeToHudWidgets, "auto");
