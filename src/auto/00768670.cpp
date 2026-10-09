#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Helper copies one 0x18-byte input-binding element. Packet bind line is authoritative.
CSF_FUNC(std::uint32_t*, __fastcall, CopyMenuElementIntoSlot, 0x007685C0,
         (void* context, void* edx, std::uint32_t* destination, const std::uint32_t* source));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler data referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_InputBindingCopyExceptionMetadata, 0x008938E8);

// Copies `count` 0x18-byte elements starting at `elements`, each via
// CopyMenuElementIntoSlot(context, destination, source). The source pointer
// is passed unchanged on every iteration. The loop runs inside the original's
// C++ try block; the catch-all rethrows the current exception.
static std::uint32_t* __cdecl CopyInputBindingElementsIntoSlots(
    std::uint32_t* elements,
    std::uint32_t count,
    const std::uint32_t* source,
    void* context,
    void* /*unusedArgument*/)
{
    // The original leaves an incidental EAX value when count is zero.
    std::uint32_t* result = nullptr;

    try
    {
        while (count != 0)
        {
            result = CopyMenuElementIntoSlot(context, nullptr, elements, source);

            --count;
            elements += 6;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00768670, CopyInputBindingElementsIntoSlots, "auto");
