#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element copy helper: destination pair first, then the source pair on the stack.
// The original also loads the copy context into ecx before the call (see notes);
// this binding follows the packet's bind line.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull,
         0x00730830, (std::uint32_t* dest, const std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH funclet emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));
CSF_GLOBAL(std::uint8_t, g_CharActionFsmRangeExceptionInformation, 0x00889B80);

// Copies the range of 4-byte elements [source, sourceEnd) to `destination`,
// invoking the "copy dword if dest non-null" element helper for every element.
// The fourth argument is a copy context passed through to the helper.
// Returns the end of the written destination range.
static int __cdecl CharActionFsm_CopyDwordRangeIfNotNull(
    std::uint32_t source,
    std::uint32_t sourceEnd,
    std::uint32_t destination,
    std::uint32_t /*copyContext*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyDwordIfNotNull(
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<const std::uint32_t*>(source));
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

HOOK(0x00480D00, CharActionFsm_CopyDwordRangeIfNotNull, "auto");
