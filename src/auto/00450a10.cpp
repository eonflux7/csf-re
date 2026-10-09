#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Callee bound exactly as the packet specifies (__stdcall, ret 8).
CSF_FUNC(std::uint32_t*, __stdcall, UpdateArrayElement, 0x00458500,
         (std::uint32_t* a1, std::uint32_t* a2));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context,
          void* dispatcherContext));

// Exception handler metadata referenced by the original's C++ frame.
CSF_GLOBAL(std::uint8_t, g_ArrayUpdateExceptionMetadata, 0x00887D18);

// Walks `count` elements of 20 bytes starting at `elements`, calling
// UpdateArrayElement(element, argument) for each. The loop runs inside the
// original's C++ try block; the catch-all rethrows the current exception.
static int __cdecl ForEachArrayElementUpdate(
    std::uint8_t* elements,
    std::uint32_t count,
    std::uint32_t* argument,
    void* owner)
{
    // The original returns whatever eax held if count is zero; pass 0.
    int result = 0;

    try
    {
        while (count != 0)
        {
            result = static_cast<int>(reinterpret_cast<std::uintptr_t>(
                UpdateArrayElement(reinterpret_cast<std::uint32_t*>(elements), argument)));

            --count;
            elements += 20;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x00450A10, ForEachArrayElementUpdate, "auto");
