#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void, __stdcall, TextureLoader_CopyElement, 0x006DD020,
         (void* a1, void* a2));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_TextureLoaderCopyExceptionMetadata, 0x00890C10);

static void __cdecl TextureLoader_CopyElementArray(
    char* destination,
    std::uint32_t count,
    const void* source,
    void* /*unusedContext*/)
{
    try
    {
        while (count != 0)
        {
            TextureLoader_CopyElement(destination, const_cast<void*>(source));
            --count;
            destination += 28;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }
}

HOOK(0x006DE130, TextureLoader_CopyElementArray, "auto");
