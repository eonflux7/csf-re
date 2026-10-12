#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element helper used by the range loop: destination first, then source (both stdcall).
// The original also loads the copy context (4th argument) into ecx before the call;
// this binding follows the packet's bind line, so ecx is not passed.
CSF_FUNC(std::uint32_t*, __stdcall, UpdateArrayElement,
         0x00458500, (std::uint32_t* a1, std::uint32_t* a2));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information referenced by the frame's unwind/handler code.
CSF_GLOBAL(std::uint8_t, g_ArrayRangeExceptionInformation, 0x00888264);

// Updates each 20-byte array element in [source, sourceEnd) against the matching
// destination element, invoking the element helper for every element. The fourth
// argument is a context value that the original loads into ecx for the helper call.
// The fifth argument is never read by the original.
// Returns the end of the written destination range.
static std::uint32_t* __cdecl UpdateArrayRangeToDestination(
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
            UpdateArrayElement(
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<std::uint32_t*>(source));
            destination += 0x14u;
            source += 0x14u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return reinterpret_cast<std::uint32_t*>(destination);
}

HOOK(0x004585A0, UpdateArrayRangeToDestination, "auto");
