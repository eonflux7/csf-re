#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies one 8-byte element. Packet bind line is authoritative.
CSF_FUNC(std::uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_MissionScriptCopyExceptionMetadata, 0x00888710);

// Copies `count` 8-byte elements starting at `destination`, each via
// CopyTwoDwordsIfDestNonNull(destination, source). The loop runs inside the
// original's C++ try block; the catch-all rethrows the current exception.
// The original also loads `context` into ecx for the call; the bound
// stdcall helper takes no implicit this, so it is not forwarded.
static std::uint32_t* __cdecl MissionScript_CopyDwordPairArray(
    std::uint32_t* destination,
    std::uint32_t count,
    const std::uint32_t* source,
    void* context,
    int unusedArg)
{
    // Original returns whatever eax held when count is zero; pass 0.
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = CopyTwoDwordsIfDestNonNull(destination, source);

            --count;
            destination += 2;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x004688D0, MissionScript_CopyDwordPairArray, "auto");
