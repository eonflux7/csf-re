#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// sub_547090: __thiscall(context), pops 8 bytes (destination, source)
CSF_FUNC(void*, __fastcall, MenuExtras_InitStringFromPair, 0x00547090,
         (void* context, void* edx, std::uint32_t* destination,
          std::uint32_t* source));
// sub_5466C0: __thiscall(context), pops 4 bytes (element)
CSF_FUNC(int, __fastcall, ResetStringStorage_5466C0, 0x005466C0,
         (void* context, void* edx, std::uint32_t* element));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_ArrayConstructionExceptionInfo, 0x00892774);

// Copy-constructs `count` 32-byte elements from `source` into `destination`,
// using the construction context at +0 (the same source element for every
// destination element). On failure the already-constructed elements are
// destroyed in order and the pending exception is rethrown.
static void* __cdecl CopyConstructElementArray32(
    std::uint32_t* destination,
    std::uint32_t count,
    std::uint32_t* source,
    void* context)
{
    std::uint32_t* currentElement = destination;
    void* result;

    try
    {
        while (count != 0)
        {
            result = MenuExtras_InitStringFromPair(
                context, nullptr, currentElement, source);
            --count;
            currentElement += 8;
        }
    }
    catch (...)
    {
        std::uint32_t* element = destination;
        while (element != currentElement)
        {
            ResetStringStorage_5466C0(context, nullptr, element);
            element += 8;
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x0072F090, CopyConstructElementArray32, "auto");
