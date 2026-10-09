#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies one dword from `source` to `destination` if the destination is non-null.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull, 0x00730830,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_IntVectorExceptionMetadata, 0x00888CAC);

// Fills `count` consecutive dwords starting at `destination`, each via
// CopyDwordIfNotNull(destination, source). The loop runs inside the original's
// C++ try block; the catch-all rethrows the current exception.
static int __cdecl IntVector_FillUninitialized(
    std::uint32_t* destination,
    std::uint32_t count,
    const std::uint32_t* source,
    void* context)
{
    // The original returns whatever EAX held last; with count == 0 that is
    // an uninitialized value, so pass 0.
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = reinterpret_cast<int>(CopyDwordIfNotNull(destination, source));

            --count;
            destination += 1;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0046BFB0, IntVector_FillUninitialized, "auto");
