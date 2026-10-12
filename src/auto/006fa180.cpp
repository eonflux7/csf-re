#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __stdcall, CopyThirtyTwoByteElement, 0x006FA030,
         (void* dest, void* source));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_ElementCopyExceptionMetadata, 0x00891494);

static void __cdecl GfxScreenEffects_FillThirtyTwoByteElements(
    char* destination,
    std::uint32_t count,
    void* source,
    void* /*unusedContext*/)
{
    try
    {
        while (count != 0)
        {
            CopyThirtyTwoByteElement(destination, source);
            --count;
            destination += 32;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }
}

HOOK(0x006FA180, GfxScreenEffects_FillThirtyTwoByteElements, "auto");
