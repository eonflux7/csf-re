#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element copy helper used by the range loop: destination element first, then
// the source element on the stack. Copies the source dword into the
// destination only when the source element is non-null.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull,
         0x00730830, (std::uint32_t* destination, std::uint32_t* source));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH funclet emitted for this function.
CSF_FUNC(int, __cdecl, HandleCppExceptionFrame, 0x007AF226, (...));
CSF_GLOBAL(std::uint8_t, g_CopyDwordRangeExceptionInformation, 0x0088AB14);

// Copies the range of dword elements [source, sourceEnd) to `destination`,
// invoking the "copy if not null" element helper for every element (so null
// source elements leave the destination slot untouched). The fourth argument
// is a copy context forwarded to the element helper. Returns the end of the
// written destination range.
static int __cdecl MenuManager_CopyDwordRangeIfNotNull(
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
                reinterpret_cast<std::uint32_t*>(source));
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

HOOK(0x0050C8E0, MenuManager_CopyDwordRangeIfNotNull, "auto");
