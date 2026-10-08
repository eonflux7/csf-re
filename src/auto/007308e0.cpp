#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies a single four-byte element from `source` into `destination` using a
// caller supplied context object (allocator / owning container, etc.).
CSF_FUNC(std::uint32_t*, __fastcall, CopyDwordIfNotNull, 0x00730830,
         (void* context, void* edx, std::uint32_t* destination,
          std::uint32_t* source));

// Invoked from the catch(...) handler below; the orignal rethrows.
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Frame handler referenced through the compiler generated handler stub.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// These are the compiler generated SEH thunk and its FuncInfo metadata: data
// referenced by the original, not call targets.
CSF_GLOBAL(std::uint8_t, g_ElementCopyExceptionHandlerStub, 0x008280F0);
CSF_GLOBAL(std::uint8_t, g_ElementCopyExceptionMetadata, 0x008928E8);

// Copies `count` four-byte elements from `source` into the consecutive slots at
// `destination`, forwarding each to CopyDwordIfNotNull with `context`.
static std::uint32_t* __cdecl CopyFourByteElementsWithContext(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    // Intentionally uninitialised: when count is zero the original returns the
    // caller's incidental EAX value untouched.
    std::uint32_t* result;

    try
    {
        while (count != 0)
        {
            result = CopyDwordIfNotNull(context, nullptr, destination, source);

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

HOOK(0x007308E0, CopyFourByteElementsWithContext, "auto");
