#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element copy helper used by the range loop. The original loads the copy
// context (its 4th argument) into ecx before each call; this bind follows the
// packet's bind line, so that context is not forwarded.
CSF_FUNC(std::uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull,
         0x0071A2E0, (std::uint32_t* dest, const std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH funclet emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception info block that the funclet loads into eax before jumping to the frame handler.
CSF_GLOBAL(std::uint8_t, g_MissionScriptExceptionInformation, 0x008886C0);

// Copies the range of 8-byte elements [source, sourceEnd) to `destination`,
// invoking the element helper for every element. The fourth argument is a
// copy context. The fifth argument is pushed by the caller but never read.
// Returns the end of the written destination range.
static int __cdecl MissionScript_CopyDwordPairRange(
    std::uint32_t source,
    std::uint32_t sourceEnd,
    std::uint32_t destination,
    std::uint32_t /*copyContext*/,
    std::uint32_t /*unused*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyTwoDwordsIfDestNonNull(
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<const std::uint32_t*>(source));
            destination += 8u;
            source += 8u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x004685D0, MissionScript_CopyDwordPairRange, "auto");
