#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __stdcall, CopyThirtyTwoByteElement, 0x006FA030, (void* dest, void* source));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

CSF_GLOBAL(std::uint8_t, g_ScreenEffectsRangeExceptionInformation, 0x00891354);

static char* __cdecl GfxScreenEffects_CopyElementRange(
    char* source,
    char* sourceEnd,
    char* destination,
    int /*copyContext*/,
    int /*unused*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyThirtyTwoByteElement(destination, source);
            destination += 32;
            source += 32;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x006FA0C0, GfxScreenEffects_CopyElementRange, "auto");
