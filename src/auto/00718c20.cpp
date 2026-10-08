#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Wrapper around the SEH handler stub at 0x827EE0 (jmp __CxxFrameHandler).
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226, (...));
// __CxxThrowException@8 - used to rethrow from the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Copy-constructs one 4-byte element from `source` into `destination` (pops 8 bytes).
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull,
         0x00730830, (std::uint32_t* destination, std::uint32_t* source));

// Exception scope table referenced by the original's unwind entry (0x827EE0).
CSF_GLOBAL(std::uint8_t, g_PointerRangeCopyExceptionInformation, 0x00891B50);

// Copies [source, sourceEnd) into the destination range, copy-constructing each
// 4-byte element; rethrows whatever escapes the copy loop. Returns the advanced
// destination pointer.
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

HOOK(0x00718C20, CopyConstructPointerRange, "auto");
