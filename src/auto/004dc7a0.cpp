#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Copies two dwords from src to dest when dest is non-null; returns the destination pointer.
CSF_FUNC(std::uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_WaypointCopyExceptionMetadata, 0x0088A290);

// Walks `pairCount` entries of 8 bytes starting at `a1`, copying each pair from
// `a3` via CopyTwoDwordsIfDestNonNull. The catch-all rethrows the current exception.
// The helper's `this` argument (ecx) is ignored by the __stdcall callee, so it is not forwarded.
static int __cdecl WaypointGraph_CopyIndexPairs(int a1, int a2, int a3, int a4, int a5)
{
    (void)a4;
    (void)a5;

    std::uint32_t pairCount = static_cast<std::uint32_t>(a2);
    std::uintptr_t destinationAddress = static_cast<std::uintptr_t>(static_cast<std::uint32_t>(a1));
    const std::uint32_t* source = reinterpret_cast<const std::uint32_t*>(
        static_cast<std::uintptr_t>(static_cast<std::uint32_t>(a3)));

    // The original leaves its last EAX value when the count is zero; pass 0.
    std::uint32_t* result = nullptr;

    try
    {
        while (pairCount != 0)
        {
            result = CopyTwoDwordsIfDestNonNull(
                reinterpret_cast<std::uint32_t*>(destinationAddress), source);

            --pairCount;
            destinationAddress += 8;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x004DC7A0, WaypointGraph_CopyIndexPairs, "auto");
