#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Per-entry initializer: called once per 0x44-byte record with (destination slot, source entry).
CSF_FUNC(char*, __stdcall, MissionScript_InitEntry, 0x0046A0E0, (char* a1, int a2));

// Per-entry cleanup, used by the unwind path on destination slots already initialized.
CSF_FUNC(int, __stdcall, MissionScript_DestroyEntry, 0x00469230, (int* a1));

CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

// Compiler-generated EH data referenced by the frame setup: bind by address.
CSF_GLOBAL(std::uint8_t, g_MissionScriptEntryRangeFuncInfo, 0x00888A48);

// Walks the half-open range [sourceBegin, sourceEnd) of 0x44-byte entries and initializes
// each destination slot (same 0x44 stride) from its source entry. If an initializer throws,
// the destination slots already handled are destroyed before rethrowing.
// Returns the past-the-end destination pointer.
static char* __cdecl MissionScript_InitEntryRange(
    int sourceBegin,
    int sourceEnd,
    char* destination,
    int /*contextEcx*/,
    int /*unusedArg5*/)
{
    char* const destinationBegin = destination;
    int source = sourceBegin;

    try
    {
        while (source != sourceEnd)
        {
            MissionScript_InitEntry(destination, source);
            destination += 0x44;
            source += 0x44;
        }
    }
    catch (...)
    {
        // Destroy every destination slot already processed in [begin, current).
        char* handled = destinationBegin;

        while (handled != destination)
        {
            MissionScript_DestroyEntry(reinterpret_cast<int*>(handled));
            handled += 0x44;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x0046A100, MissionScript_InitEntryRange, "auto");
