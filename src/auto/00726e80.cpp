#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t*, __fastcall, CopyDwordIfNotNull, 0x00730830,
         (void* context, void* edx, std::uint32_t* destination,
          std::uint32_t* source));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// This address is an exception-handler stub, not a recognized function start.
CSF_GLOBAL(std::uint8_t, g_ElementCopyExceptionHandlerStub, 0x00828020);
CSF_GLOBAL(std::uint8_t, g_ElementCopyExceptionMetadata, 0x00892334);

static int __cdecl CopyFourByteElementsWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    // The original leaves an incidental EAX value when count is zero.
    int result;

    try
    {
        while (count != 0)
        {
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
                CopyDwordIfNotNull(context, nullptr, destination, source)));

            --count;
            destination = reinterpret_cast<std::uint32_t*>(
                reinterpret_cast<std::uintptr_t>(destination) + 4u);
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00726E80, CopyFourByteElementsWithContext, "auto");
