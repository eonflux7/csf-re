#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __stdcall, CopyDwordIfNotNull, 0x00730830, (uint32_t* dest, const uint32_t* src));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

CSF_GLOBAL(std::uint8_t, g_DwordFillExceptionMetadata, 0x008898E8);

static int __cdecl FillDwordsWithContext(
    uint32_t* destination,
    uint32_t count,
    const uint32_t* source,
    void* /*unusedContext*/,
    void* /*unusedArg*/)
{
    uint32_t* result = nullptr;
    try
    {
        while (count != 0)
        {
            result = CopyDwordIfNotNull(destination, source);
            --count;
            destination = reinterpret_cast<uint32_t*>(
                reinterpret_cast<std::uintptr_t>(destination) + 4);
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(result));
}

HOOK(0x00471C90, FillDwordsWithContext, "auto");
