#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element copy helper called once per dword pair: destination first, then source.
// The original also loads the copy context (a4) into ecx before each call; this
// binding follows the packet's bind line, so the context is not passed.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull,
         0x00730830, (std::uint32_t* dest, const std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH funclet emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information block used by the funclet (loaded as eax = 0x893A5C).
CSF_GLOBAL(std::uint8_t, g_DwordRangeExceptionInformation, 0x00893A5C);

// Copies the range of dwords [source, sourceEnd) to `destination`, invoking the
// "copy dword if not null" element helper for every element. The fourth argument
// is a copy context loaded into ecx for the helper call. Returns the end of the
// written destination range.
static std::uint32_t* __cdecl InputBindings_CopyDwordRangeIfNotNull(
    std::uint32_t* source,
    std::uint32_t* sourceEnd,
    std::uint32_t* destination,
    int /*copyContext*/,
    int /*unused*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyDwordIfNotNull(destination, source);
            ++destination;
            ++source;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x00768F30, InputBindings_CopyDwordRangeIfNotNull, "auto");
