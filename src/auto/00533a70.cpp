#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element copy helper used by the range loop. The original pushes the
// destination element first and the source element second, and forwards the
// loop context (4th argument of the target) in ECX. Modeled as the codebase's
// __thiscall form (__fastcall with an unused edx) so ECX is set exactly as the
// original does; the callee still pops 8 stack bytes.
CSF_FUNC(std::uint32_t*, __fastcall, CopyDwordIfNotNull, 0x00730830,
         (void* context, void* edx, std::uint32_t* destination, std::uint32_t* source));

// __CxxThrowException@8 - used to rethrow from the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Referenced by the SEH funclet emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226, (...));
CSF_GLOBAL(std::uint8_t, g_DwordRangeCopyExceptionInformation, 0x0088B50C);

// Copies the 4-byte elements of [source, sourceEnd) into `destination`,
// invoking the element helper at 0x730830 for each element, and returns the end
// of the written destination range.
static int __cdecl CopyDwordRange(
    std::uint32_t* source,
    std::uint32_t* sourceEnd,
    std::uint32_t* destination,
    void* context)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyDwordIfNotNull(context, nullptr, destination, source);
            ++destination;
            ++source;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return reinterpret_cast<int>(destination);
}

HOOK(0x00533A70, CopyDwordRange, "auto");
