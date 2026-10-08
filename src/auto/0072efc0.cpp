#include <cstdint>
#include "globals.h"
#include "hook.h"

// Copy-constructs a range of 32-byte elements into uninitialized storage.
// If any construction throws, the already-constructed elements are destroyed
// and the exception is rethrown. Used by the STL / container copy machinery.

CSF_FUNC(void, __stdcall, MenuExtras_InitStringFromPair, 0x00547090,
         (std::uint32_t* destination, std::uint32_t* source));

CSF_FUNC(void, __stdcall, ResetStringStorage_5466C0, 0x005466C0, (std::uint32_t* element));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH handler stub and its scope table (bind addresses, not symbols).
CSF_GLOBAL(std::uint8_t, g_CopyConstructExceptionHandlerCode, 0x008280B0);
CSF_GLOBAL(std::uint8_t, g_CopyConstructExceptionInfo, 0x00892724);

static std::uint32_t* __cdecl CopyConstructElementRange(
    std::uint32_t* source,
    std::uint32_t* sourceEnd,
    std::uint32_t* destination,
    void* allocatorContext)
{
    // The supplied stdcall callee prototypes do not consume ECX.
    (void)allocatorContext;

    std::uint32_t* const destinationBegin = destination;

    try
    {
        while (source != sourceEnd)
        {
            MenuExtras_InitStringFromPair(destination, source);
            destination += 8; // 0x20 bytes per element
            source += 8;      // 0x20 bytes per element
        }
    }
    catch (...)
    {
        std::uint32_t* constructed = destinationBegin;
        std::uint32_t* const constructedEnd = destination;

        while (constructed != constructedEnd)
        {
            ResetStringStorage_5466C0(constructed);
            constructed += 8; // 0x20 bytes per element
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x0072EFC0, CopyConstructElementRange, "auto");
