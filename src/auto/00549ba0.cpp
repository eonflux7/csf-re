#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element copy helper reached with ecx = `context` (the element container)
// and two stack arguments: the destination element first, then the source
// element. The callee pops 8 bytes (2 stack args -> __thiscall ABI, modelled
// here as __fastcall with ecx/edx).
CSF_FUNC(std::uint32_t*, __fastcall, CopyMenuElementIntoSlot,
         0x007685C0, (void* context, void* edx,
                      std::uint32_t* destination,
                      const std::uint32_t* source));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH funclet emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226, (...));
CSF_GLOBAL(std::uint8_t, g_CopyRangeExceptionInformation, 0x0088BA3C);

// Copies the range of 24-byte elements [source, sourceEnd) into the
// destination buffer, invoking the element helper for every element.
// `copyContext` is forwarded as the helper's `this` (element container).
// Returns the end of the written destination range.
static int __cdecl MenuVideoOptions_CopyElementRange(
    std::uint32_t source,
    std::uint32_t sourceEnd,
    std::uint32_t destination,
    std::uint32_t copyContext)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyMenuElementIntoSlot(
                reinterpret_cast<void*>(copyContext), nullptr,
                reinterpret_cast<std::uint32_t*>(destination),
                reinterpret_cast<const std::uint32_t*>(source));
            destination += 0x18u;
            source += 0x18u;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(destination);
}

HOOK(0x00549BA0, MenuVideoOptions_CopyElementRange, "auto");
