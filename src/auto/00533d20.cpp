#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// 0x00730830 - context-based element copier: writes the element at `source`
// into `destination` (unless it is null), using `context` as the implicit
// this pointer (passed in ecx). Pops its two stack arguments.
CSF_FUNC(std::uint32_t*, __fastcall, CopyDwordIfNotNull, 0x00730830,
         (void* context, void* edx, std::uint32_t* destination,
          std::uint32_t* source));

// Called from the catch-all arm to rethrow the in-flight C++ exception.
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Frame handler targeted by the original's SEH stub.
CSF_FUNC(int, __cdecl, HandleCppExceptionFrame, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception metadata reached through the original's SEH handler stub
// (the pushed handler stub itself is compiler-generated and not reproduced).
CSF_GLOBAL(std::uint8_t, g_FourByteCopyExceptionMetadata, 0x0088B55C);

// Copies `count` 4-byte elements from `source` to `destination`, delegating
// each individual element copy to the context-based copier. The whole copy
// loop runs inside a C++ try block whose catch-all rethrows the current
// exception. Returns the last element-copy result in EAX.
static std::uint32_t* __cdecl CopyFourByteElements(
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
            result = CopyDwordIfNotNull(context, nullptr, destination, source);

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

HOOK(0x00533D20, CopyFourByteElements, "auto");
