#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element helper used by the range loop: destination first, then source (both stdcall).
// The original also loads the copy context (4th argument) into ecx before the call;
// this binding follows the packet's bind line, so ecx is not passed.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull,
         0x00730830, (std::uint32_t* dest, const std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information (FuncInfo) referenced by this function's frame handler stub.
CSF_GLOBAL(std::uint8_t, g_CoreVariantDwordRangeExceptionInfo, 0x0088F03C);

// Copies the range of dword elements [source, sourceEnd) to `destination`,
// invoking the element helper for every element. The fourth argument is a copy
// context that the original loads into ecx for the helper call.
// The fifth argument is never read by the original.
// Returns the end of the written destination range.
static int __cdecl CoreVariant_CopyDwordRange(
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

HOOK(0x00695850, CoreVariant_CopyDwordRange, "auto");
