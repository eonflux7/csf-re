#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element copy helper used by the range loop: destination first, then source on the stack.
// The original also loads the copy context (a5) into ecx before the call; this binding
// follows the packet's bind line, so the context is not forwarded (see notes).
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull,
         0x00730830, (std::uint32_t* dest, const std::uint32_t* src));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH funclet emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information data referenced by the funclet.
CSF_GLOBAL(std::uint8_t, g_MissionScriptCopyExceptionInformation, 0x00888B6C);

// Copies the range of 4-byte elements [source, sourceEnd) to `destination`,
// invoking the "copy dword if dest non-null" element helper for every element.
// The fourth argument is a copy context in the original; it is not forwarded here.
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
        (void)g_MissionScriptCopyExceptionInformation;
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x0046BED0, MissionScript_CopyDwordRangeIfNotNull, "auto");
