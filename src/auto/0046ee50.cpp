#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element copy helper called once per dword: destination first, then source (stdcall, ret 8).
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull,
         0x00730830, (std::uint32_t* dest, const std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH funclet emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception-funclet data referenced by the prologue handler (mov eax, 0x88948c).
CSF_GLOBAL(std::uint8_t, g_MissionScriptCopyRangeHandlerData, 0x0088948C);

// Copies the range of dword elements [source, sourceEnd) to `destination`,
// invoking the "copy dword if dest non-null" helper for every element.
// The fourth argument is the copy context, which the original loads into ecx
// before each helper call; the bound helper signature does not take it, so it
// is accepted here and not forwarded.
// Returns the end of the written destination range.
static int __cdecl MissionScript_CopyDwordRangeIfNotNull(
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

HOOK(0x0046EE50, MissionScript_CopyDwordRangeIfNotNull, "auto");
