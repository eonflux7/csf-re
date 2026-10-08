#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(std::uint32_t*, __fastcall, ForwardTo7234F0, 0x007236A0,
         (void* context, void* edx, std::uint32_t* destination,
          std::uint32_t* source));
CSF_FUNC(int, __fastcall, ResetWideStringStorage, 0x00723390,
         (void* context, void* edx, std::uint32_t* element));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_ArrayConstructionExceptionInfo, 0x0089209C);

static std::uint32_t* __cdecl CopyConstructElementArray(
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
            result = ForwardTo7234F0(
                context, nullptr, currentElement, source);
            --count;
            currentElement += 15;
        }
    }
    catch (...)
    {
        std::uint32_t* element = destination;
        while (element != currentElement)
        {
            ResetWideStringStorage(context, nullptr, element);
            element += 15;
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00723850, CopyConstructElementArray, "auto");
