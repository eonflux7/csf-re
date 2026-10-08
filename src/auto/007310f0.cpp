#include <cstdint>
#include "globals.h"
#include "hook.h"

// Forwards each element of the source range through the element handler,
// producing the destination range. If the handler throws, the already built
// destination elements are destroyed and the exception is rethrown.
CSF_FUNC(int, __stdcall, ForwardToHandlerIfNonNull, 0x007310D0, (int a1, int a2));
CSF_FUNC(int, __stdcall, DestroyElementThroughContext, 0x006B5220, (int* element));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated exception-handling stubs: bound by address, not called.
CSF_GLOBAL(std::uint8_t, g_ForwardRangeExceptionHandlerCode, 0x00828120);
CSF_GLOBAL(std::uint8_t, g_ForwardRangeExceptionInfo, 0x00892A0C);

static int __cdecl ForwardRangeToHandler(
    std::uint8_t* source,
    std::uint8_t* sourceEnd,
    std::uint8_t* destination)
{
    std::uint8_t* const destinationBegin = destination;

    try
    {
        while (source != sourceEnd)
        {
            ForwardToHandlerIfNonNull(reinterpret_cast<int>(destination),
                                     reinterpret_cast<int>(source));
            destination += 16;
            source += 16;
        }
    }
    catch (...)
    {
        std::uint8_t* constructed = destinationBegin;
        std::uint8_t* const constructedEnd = destination;

        while (constructed != constructedEnd)
        {
            DestroyElementThroughContext(reinterpret_cast<int*>(constructed));
            constructed += 16;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return reinterpret_cast<int>(destination);
}

HOOK(0x007310F0, ForwardRangeToHandler, "auto");
