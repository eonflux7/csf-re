#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Copies one 0x10-byte entry into the destination slot (callee pops 8).
CSF_FUNC(uint32_t*, __stdcall, CopyEntryIntoSlot, 0x004685B0, (uint32_t* a1, uint32_t* a2));

// Destroys one already-copied destination slot during the cleanup path (callee pops 4).
CSF_FUNC(int, __stdcall, DestroyEntrySlot, 0x00468940, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH data/code referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_CopyRangeExceptionHandlerCode, 0x00826800);
CSF_GLOBAL(std::uint8_t, g_CopyRangeFuncInfo, 0x00888760);

// Walks the half-open range [source, sourceEnd) of 0x10-byte entries and copies each
// one into the matching destination slot (also 0x10-byte stride). If a copy throws,
// the destination slots already filled are destroyed before rethrowing.
// Returns the past-the-end destination pointer.
static int __cdecl CopyRangeIntoSlotsWithCleanup(
    void* source,
    void* sourceEnd,
    void* destination,
    void* /*contextEcx*/,
    int /*unusedArg5*/)
{
    void* const destinationBegin = destination;

    try
    {
        while (source != sourceEnd)
        {
            CopyEntryIntoSlot(
                reinterpret_cast<uint32_t*>(destination),
                reinterpret_cast<uint32_t*>(source));
            destination = static_cast<std::uint8_t*>(destination) + 0x10;
            source = static_cast<std::uint8_t*>(source) + 0x10;
        }
    }
    catch (...)
    {
        // Destroy every destination slot already filled in [begin, current).
        void* filled = destinationBegin;

        while (filled != destination)
        {
            DestroyEntrySlot(reinterpret_cast<int*>(filled));
            filled = static_cast<std::uint8_t*>(filled) + 0x10;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x00468970, CopyRangeIntoSlotsWithCleanup, "auto");
