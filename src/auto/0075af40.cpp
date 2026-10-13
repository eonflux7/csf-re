#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __stdcall, InitSoundEntry_75AEF0, 0x0075AEF0, (int a1, int a2));
CSF_FUNC(int, __fastcall, BddTables_ResetElementBuffer, 0x0051BEB0, (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_SoundEntryRangeExceptionInfo, 0x008930E8);

static int __cdecl InitSoundEntryRange_75AF40(
    int first,
    int last,
    int destination,
    int context,
    int unused)
{
    (void)&g_SoundEntryRangeExceptionInfo;
    (void)context;
    (void)unused;

    const int destinationBegin = destination;
    int source = first;

    try
    {
        while (source != last)
        {
            InitSoundEntry_75AEF0(destination, source);

            destination += 0x44;
            source += 0x44;
        }
    }
    catch (...)
    {
        int completed = destinationBegin;
        while (completed != destination)
        {
            BddTables_ResetElementBuffer(
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(completed)),
                nullptr);
            completed += 0x44;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x0075AF40, InitSoundEntryRange_75AF40, "auto");
