#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copy-constructs one element from source into destination. The original sets
// ecx to the 4th argument (construction context) before every call, so it is
// bound as a __fastcall with that context in ecx (edx unused); the callee pops
// 8 bytes of stack arguments (destination, source).
CSF_FUNC(void*, __fastcall, CopyDwordIfNotNull,
         0x00730830, (void* context, void* /*edx*/,
                      std::uint32_t* destination, std::uint32_t* source));

CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226, (...));

// Scopetable referenced by the original's SEH frame (handler at 0x00828070).
CSF_GLOBAL(std::uint8_t, g_CopyConstructPointerRangeExceptionInformation, 0x0089252C);

static int __cdecl CopyConstructPointerRange(
    std::uint32_t source,
    std::uint32_t sourceEnd,
    std::uint32_t destination,
    std::uint32_t constructionContext)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyDwordIfNotNull(
                reinterpret_cast<void*>(constructionContext),
                nullptr,
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

HOOK(0x0072A660, CopyConstructPointerRange, "auto");
