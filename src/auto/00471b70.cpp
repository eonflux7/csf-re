#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __stdcall, CopyDwordIfNotNull, 0x00730830, (uint32_t* dest, const uint32_t* src));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

CSF_GLOBAL(std::uint8_t, g_DwordCopyRangeExceptionInformation, 0x00889898);

static int __cdecl CopyDwordRange(
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
                reinterpret_cast<uint32_t*>(destination),
                reinterpret_cast<const uint32_t*>(source));
            destination += 4;
            source += 4;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x00471B70, CopyDwordRange, "auto");
