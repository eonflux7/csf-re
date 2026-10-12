#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Element helper used by the range loop: destination first, then source (both stdcall).
// The original also loads its fourth argument into ecx before the call; this binding
// follows the packet's bind line, so ecx is not passed.
CSF_FUNC(std::uint32_t*, __stdcall, UpdateArrayElement,
         0x00458500, (std::uint32_t* a1, std::uint32_t* a2));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException,
         0x007AF666, (void* exceptionObject, void* throwInformation));

// Referenced by the SEH frame emitted for this function.
CSF_FUNC(int, __cdecl, ___CxxFrameHandler,
         0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Exception information referenced by the frame's handler code.
CSF_GLOBAL(std::uint8_t, g_ScreenEffectsRangeExceptionInformation, 0x008912B4);

// Walks the element range [first, last) and updates the matching destination
// element for each one (5-dword stride, 0x14 bytes). The fourth argument is a
// context value the original loads into ecx for the helper call; the fifth
// argument is never read by the original.
// Returns the end of the written destination range.
static std::uint32_t* __cdecl ScreenEffects_UpdateElementRange(
    std::uint32_t* first,
    std::uint32_t* last,
    std::uint32_t* dest,
    int /*updateContext*/,
    int /*unused*/)
{
    try
    {
        while (first != last)
        {
            UpdateArrayElement(dest, first);
            dest += 5;
            first += 5;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return dest;
}

HOOK(0x006FA050, ScreenEffects_UpdateElementRange, "auto");
