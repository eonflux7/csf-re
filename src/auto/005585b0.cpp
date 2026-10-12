#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Helper copies one 8-byte element (two dwords). Packet bind line is authoritative.
CSF_FUNC(uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (uint32_t* dest, const uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Data referenced by the original's C++ frame setup (the handler thunk loads it into eax).
CSF_GLOBAL(std::uint8_t, g_MpRulesCopyExceptionMetadata, 0x0088C32C);

// Copies `count` 8-byte elements from `source` into `destination`, one
// CopyTwoDwordsIfDestNonNull call per element. The loop runs inside the
// original's C++ try block; the catch-all rethrows the current exception.
// The original passes `context` in ecx for the helper call; the helper is
// __stdcall, so ecx is not consumed and the value is not forwarded here.
static int __cdecl MpRules_CopyPairArray(
    uint32_t* destination,
    uint32_t count,
    uint32_t* source,
    void* context)
{
    (void)context;

    // The original leaves an incidental EAX value when count is zero.
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
                CopyTwoDwordsIfDestNonNull(destination, source)));

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

HOOK(0x005585B0, MpRules_CopyPairArray, "auto");
