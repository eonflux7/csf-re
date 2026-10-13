#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element helper used by the range loop: destination first, then source (both stdcall).
// The original also loads the copy context (4th argument) into ecx before the call;
// this binding follows the packet's bind line, so ecx is not passed.
CSF_FUNC(uint32_t*, __stdcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0, (uint32_t* dest, const uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Data loaded by the exception funclet (mov eax, 0x88bb60) before jumping to the frame handler.
CSF_GLOBAL(std::uint8_t, g_TwoDwordRangeUnwindInfo, 0x0088BB60);

// Copies the range of 8-byte elements [source, sourceEnd) to `destination`,
// invoking the element helper for every element. The fourth argument is a copy
// context that the original loads into ecx for the helper call.
// The fifth argument is never read by the original.
// Returns the end of the written destination range.
static int __cdecl CopyTwoDwordRangeToDestination(
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
                reinterpret_cast<uint32_t*>(destination),
                reinterpret_cast<const uint32_t*>(source));
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

HOOK(0x0054AF40, CopyTwoDwordRangeToDestination, "auto");
