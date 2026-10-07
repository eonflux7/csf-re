#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t*, __fastcall, CallHandlerIfNonNull, 0x00716060,
         (void* context, void* edx, std::uint32_t* element, std::uint32_t* source));
CSF_FUNC(int, __fastcall, DestroyElementThroughContext, 0x006B5220,
         (void* context, void* edx, std::uint32_t* element));
CSF_FUNC(void, __stdcall, RethrowGameException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, HandleGameCppException, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_ArrayConstructionExceptionInfo, 0x00891A7C);

static std::uint32_t* __cdecl ConstructElementArray(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    std::uint32_t* currentElement = destination;
    std::uint32_t* result;

    try
    {
        while (count != 0)
        {
            result = CallHandlerIfNonNull(context, nullptr, currentElement, source);
            --count;
            currentElement += 4;
        }
    }
    catch (...)
    {
        std::uint32_t* element = destination;
        while (element != currentElement)
        {
            DestroyElementThroughContext(context, nullptr, element);
            element += 4;
        }

        RethrowGameException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00716630, ConstructElementArray, "auto");
