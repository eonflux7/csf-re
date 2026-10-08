#include <cstdint>

#include "globals.h"
#include "hook.h"

// __thiscall on the original: ecx = context, two stack arguments.
CSF_FUNC(int, __fastcall, ForwardToHandlerIfNonNull, 0x007310D0,
         (void* context, void* edx, std::uint8_t* destination,
          std::uint8_t* source));
// __thiscall on the original: ecx = context, one stack argument.
CSF_FUNC(void, __fastcall, DestroyElementThroughContext, 0x006B5220,
         (void* context, void* edx, std::uint8_t* element));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_ArrayConstructionExceptionInfo, 0x00892A5C);

// Copy-constructs `count` 16-byte elements into `destination`, each time
// invoking the context handler with the running destination and the same
// source. If any construction throws, all elements constructed so far are
// destroyed (from the first up to, but not including, the failing one) and
// the current exception is rethrown.
static int __cdecl CopyConstructElementArray(
    std::uint8_t* destination,
    std::uint32_t count,
    std::uint8_t* source,
    void* context)
{
    std::uint8_t* currentElement = destination;
    std::uint8_t* const firstElement = destination;
    int result;

    try
    {
        while (count != 0)
        {
            result = ForwardToHandlerIfNonNull(
                context, nullptr, currentElement, source);
            --count;
            currentElement += 16;
        }
    }
    catch (...)
    {
        std::uint8_t* element = firstElement;
        while (element != currentElement)
        {
            DestroyElementThroughContext(context, nullptr, element);
            element += 16;
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00731210, CopyConstructElementArray, "auto");
