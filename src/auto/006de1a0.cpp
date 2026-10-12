#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __stdcall, CopyTextureElement,
         0x006DD020, (void* a1, void* a2));

CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_TextureRangeExceptionInformation, 0x00890BC0);

static char* __cdecl GfxTextureLoader_CopyElementRange(
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
            CopyTextureElement(destination, source);
            destination += 28;
            source += 28;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x006DE1A0, GfxTextureLoader_CopyElementRange, "auto");
