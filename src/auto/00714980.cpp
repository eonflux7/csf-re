#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t*, __stdcall, CopyConstructPointerElement,
         0x00730830, (std::uint32_t* destination, std::uint32_t* source));

CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, HandleCppExceptionFrame, 0x007AF226, (...));
CSF_GLOBAL(std::uint8_t, g_PointerRangeCopyExceptionInformation, 0x00891908);

static int __cdecl CopyConstructPointerRange(
    std::uint32_t source,
    std::uint32_t sourceEnd,
    std::uint32_t destination,
    std::uint32_t /*constructionContext*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyConstructPointerElement(
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<std::uint32_t*>(source));
            destination += 4u;
            source += 4u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x00714980, CopyConstructPointerRange, "auto");
