#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element copy helper used by the range loop: destination first, then source (both stdcall, 12-byte elements).
// The original also loads the copy context (4th argument) into ecx before the call; this binding follows the
// packet's bind line exactly, so ecx is not passed.
CSF_FUNC(std::uint32_t*, __stdcall, CopyConstructTwelveByteElement,
         0x004A3200, (std::uint32_t* dst, std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information referenced by the frame's handler code (pushed as the frame's handler table).
CSF_GLOBAL(std::uint8_t, g_TypeRegistryForwardExceptionInfo, 0x0088BE64);

// Copies the range of 12-byte elements [source, sourceEnd) to `destination`, invoking the element
// helper for every element. The fourth argument is a copy context that the original loads into ecx
// for the helper call. The fifth argument is never read by the original.
// Returns the end of the written destination range.
static std::uint32_t __cdecl TypeRegistry_ForwardRangeWithContext(
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
            CopyConstructTwelveByteElement(
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<std::uint32_t*>(source));
            destination += 12u;
            source += 12u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x0054D870, TypeRegistry_ForwardRangeWithContext, "auto");
