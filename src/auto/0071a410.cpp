#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Per-element worker. Called with ecx = context, two stack dwords (destination,
// source) and pops its stack arguments (ret 8): a __thiscall-style call.
CSF_FUNC(std::uint32_t*, __fastcall, CopyTwoDwordsIfDestNonNull, 0x0071A2E0,
         (void* context, void* edx, std::uint32_t* destination,
          std::uint32_t* source));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception-handler stub pushed by the unwind frame (not a normal function start),
// plus the metadata block the stub points at.
CSF_GLOBAL(std::uint8_t, g_EightByteCopyExceptionHandlerStub, 0x00827F20);
CSF_GLOBAL(std::uint8_t, g_EightByteCopyExceptionMetadata, 0x00891CC4);

static std::uint32_t* __cdecl CopyEightByteElements(
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
            result = CopyTwoDwordsIfDestNonNull(context, nullptr, destination, source);

            --count;
            destination += 2; // each element is two dwords (8 bytes)
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0071A410, CopyEightByteElements, "auto");
