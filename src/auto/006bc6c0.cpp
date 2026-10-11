#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element helper used by the range loop: destination first, then source (both stdcall).
// The original also loads the copy context (4th argument) into ecx before the call;
// this binding follows the packet's bind line, so ecx is not passed.
CSF_FUNC(std::uint32_t*, __stdcall, InputBindings_CopyMenuElementIntoSlot,
         0x007685C0, (std::uint32_t* dst, std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information referenced by the frame's unwind/handler code.
CSF_GLOBAL(std::uint8_t, g_MenuElementRangeExceptionInformation, 0x00890284);

// Copies the range of 24-byte menu elements [source, sourceEnd) to `destination`,
// invoking the element helper for every element. The fourth argument is a copy
// context that the original passes to the helper in ecx.
// Returns the end of the written destination range.
static int __cdecl CopyMenuElementRange(
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
            InputBindings_CopyMenuElementIntoSlot(
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<std::uint32_t*>(source));
            destination += 24u;
            source += 24u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x006BC6C0, CopyMenuElementRange, "auto");
