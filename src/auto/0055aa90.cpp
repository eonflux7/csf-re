#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element helper used by the range loop: destination first, then source (both stdcall).
// The original also loads the fourth argument (copy context) into ecx before the call;
// this binding follows the packet's bind line, so ecx is not passed.
CSF_FUNC(std::uint32_t*, __stdcall, CopySixteenByteElement,
         0x006D9E80, (std::uint32_t* dest, std::uint32_t* source));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function (reached through the frame stub at 0x8271B0).
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information referenced by the frame's unwind/handler code.
CSF_GLOBAL(std::uint8_t, g_MpRulesRangeExceptionInformation, 0x0088C788);

// Copies the range of 16-byte elements [source, sourceEnd) to `destination`,
// invoking the element helper for every element. The fourth argument is a copy
// context that the original passes to the helper in ecx.
// Returns the end of the written destination range.
static int __cdecl MpRules_CopySixteenByteRange(
    std::uint32_t source,
    std::uint32_t sourceEnd,
    std::uint32_t destination,
    std::uint32_t /*copyContext*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopySixteenByteElement(
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<std::uint32_t*>(source));
            destination += 16u;
            source += 16u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x0055AA90, MpRules_CopySixteenByteRange, "auto");
