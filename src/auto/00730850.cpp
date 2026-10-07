#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copy-constructs a single 4-byte element in place (the container's element copier).
CSF_FUNC(std::uint32_t*, __stdcall, CopyConstructPointerElement,
         0x00730830, (std::uint32_t* destination, std::uint32_t* source));

// MSVC C++ runtime rethrow (compiles to __CxxThrowException(0, 0) / `throw;`).
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the compiler-generated exception frame of this function.
CSF_FUNC(int, __cdecl, HandleCppExceptionFrame, 0x007AF226, (...));
CSF_GLOBAL(std::uint8_t, g_PointerRangeCopyExceptionInformation, 0x00892848);

// Equivalent of MSVC's _Uninitialized_copy over a range of 4-byte elements:
// copy-constructs [source, sourceEnd) into destination, returning the advanced destination cursor.
static std::uint32_t* __cdecl CopyConstructPointerRange(
    std::uint32_t* source,
    std::uint32_t* sourceEnd,
    std::uint32_t* destination,
    std::uint32_t* /*constructionContext*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyConstructPointerElement(destination, source);
            ++destination;
            ++source;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x00730850, CopyConstructPointerRange, "auto");
