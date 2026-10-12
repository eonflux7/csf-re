#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee bind line from the packet (stdcall, two stack args, ret 8).
CSF_FUNC(void, __stdcall, sub_6FA030, 0x006FA030,
         (void* a1, void* a2));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_ElementCallExceptionMetadata, 0x00886F28);

// Invokes the helper once per 32-byte element of the array, passing the
// element pointer and the caller's third argument. The loop runs inside the
// original's C++ try block; the catch-all rethrows the current exception.
static int __cdecl InvokeHelperPerElement32(
    std::uint8_t* elements,
    std::uint32_t count,
    void* argument,
    void* unusedArgument,
    void* context)
{
    // The original returns whatever EAX held from the last helper call; with
    // no elements it returns an incidental value, so start from zero.
    int result = 0;

    try
    {
        while (count != 0)
        {
            sub_6FA030(elements, argument);

            --count;
            elements += 32;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0043D330, InvokeHelperPerElement32, "auto");
