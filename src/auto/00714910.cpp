#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t*, __fastcall, CopyConstructTwelveByteElement, 0x004A3200,
         (void* context, void* edx, std::uint32_t* destination,
          std::uint32_t* source));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// SEH record material referenced by the original's exception frame.
CSF_GLOBAL(std::uint8_t, g_TwelveByteCopyExceptionHandlerStub, 0x00827E70);
CSF_GLOBAL(std::uint8_t, g_TwelveByteCopyExceptionMetadata, 0x008918B8);

// Copies `count` 12-byte elements from `source` to `destination` using the
// context-based element copier; the whole loop runs inside a C++ try block
// whose catch-all rethrows the current exception.
static std::uint32_t* __cdecl CopyTwelveByteElements(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    // The original leaves an incidental EAX value when count is zero.
    std::uint32_t* result;

    try
    {
        while (count != 0)
        {
            result = CopyConstructTwelveByteElement(context, nullptr, destination, source);

            --count;
            destination = reinterpret_cast<std::uint32_t*>(
                reinterpret_cast<std::uintptr_t>(destination) + 12u);
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00714910, CopyTwelveByteElements, "auto");
