#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Copies one 6-dword (0x18-byte) menu element from source to destination.
// The copy context is passed in ecx; edx is unused.
CSF_FUNC(std::uint32_t*, __fastcall, CopyMenuElementIntoSlot, 0x007685C0,
         (void* context, void* edx, std::uint32_t* destination, const std::uint32_t* source));

// __CxxThrowException@8 - used to rethrow out of the catch(...) block.
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

// Copies the range of 0x18-byte menu elements [source, sourceEnd) to `destination`,
// calling CopyMenuElementIntoSlot for each element with the copy context.
// Returns the end of the written destination range.
static std::uint32_t* __cdecl InputBindings_CopyMenuElementRange(
    std::uint32_t* source,
    std::uint32_t* sourceEnd,
    std::uint32_t* destination,
    void* copyContext,
    void* /*unused*/)
{
    try
    {
        while (source != sourceEnd)
        {
            CopyMenuElementIntoSlot(copyContext, nullptr, destination, source);
            destination += 6;
            source += 6;
        }
    }
    catch (...)
    {
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x00768600, InputBindings_CopyMenuElementRange, "auto");
