#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies one dword from source to *dest; __stdcall, two stack args.
CSF_FUNC(std::uint32_t*, __stdcall, CopyDwordIfNotNull, 0x00730830,
         (std::uint32_t* dest, const std::uint32_t* src));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_DwordArrayCopyExceptionMetadata, 0x00893AFC);

// Copies `count` dwords into consecutive destination slots, each via
// CopyDwordIfNotNull(destination, source). `source` stays fixed across
// iterations. The fifth argument is forwarded to the helper in ecx, which
// the __stdcall helper ignores, so it has no observable effect here.
static std::uint32_t* __cdecl CopyDwordArrayWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    const std::uint32_t* source,
    [[maybe_unused]] int unusedArg4,
    [[maybe_unused]] void* context)
{
    // Zero-count calls return null here; the original's EAX is not meaningful.
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = CopyDwordIfNotNull(destination, source);

            --count;
            ++destination;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00768FA0, CopyDwordArrayWithContext, "auto");
