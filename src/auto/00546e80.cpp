#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// element factory: constructs one 0x44-byte element (context/allocator in ecx)
CSF_FUNC(std::uint32_t*, __fastcall, MenuExtras_ForwardIfNonNull, 0x00546DD0,
         (void* context, void* edx, std::uint32_t* element, std::uint32_t* source));
// element destructor used by the unwind path (context in ecx)
CSF_FUNC(int, __fastcall, MenuExtras_ReleaseAndResetBuffers, 0x00546670,
         (void* context, void* edx, std::uint32_t* element));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_MenuExtrasExceptionTypeInfo, 0x0088B7F4);

// Builds `count` consecutive 0x44-byte elements in place, then returns the
// result of the last construction. If a construction throws, the already
// constructed elements are destroyed and the exception is rethrown.
static std::uint32_t* __cdecl MenuExtras_ConstructElementArray(
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
            result = MenuExtras_ForwardIfNonNull(context, nullptr, currentElement, source);
            --count;
            currentElement += 17; // 17 dwords == 0x44 bytes
        }
    }
    catch (...)
    {
        std::uint32_t* element = destination;
        while (element != currentElement)
        {
            MenuExtras_ReleaseAndResetBuffers(context, nullptr, element);
            element += 17;
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00546E80, MenuExtras_ConstructElementArray, "auto");
