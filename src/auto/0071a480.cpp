#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copy-constructs a single 4-byte element (destination = source) using the
// context/allocator carried in the caller's 4th argument.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull,
         0x00730830, (std::uint32_t* destination, std::uint32_t* source));

// Catch-all handler: rethrows the active C++ exception.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// MSVC C++ exception dispatch routine referenced through the frame funclet.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226, (...));

// Per-instantiation exception information block used by the frame funclet.
CSF_GLOBAL(std::uint8_t, g_ConstructRangeExceptionInformation, 0x00891D14);

// Copy-constructs every 4-byte element of [source, sourceEnd) into the
// destination range and returns the one-past-the-end destination pointer.
static int __cdecl CopyConstructPointerRangeWithContext(
    std::uint32_t source,
    std::uint32_t sourceEnd,
    std::uint32_t destination,
    std::uint32_t /*constructionContext*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyDwordIfNotNull(
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

HOOK(0x0071A480, CopyConstructPointerRangeWithContext, "auto");
