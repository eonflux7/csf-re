#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __stdcall, ParseSoundAreaRecord_770F80, 0x00770F80, (char* a1, int a2));
CSF_FUNC(int, __fastcall, TextDatabase_ResetBuffers, 0x004EFD10, (std::uint8_t* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_SoundAreaRangeExceptionInfo, 0x00893E34);

static int __cdecl CopySoundAreaRange_770FA0(
    int first,
    int last,
    int destination,
    int context,
    int unused)
{
    (void)&g_SoundAreaRangeExceptionInfo;
    (void)context;
    (void)unused;

    const int destinationBegin = destination;
    int source = first;

    try
    {
        while (source != last)
        {
            ParseSoundAreaRecord_770F80(reinterpret_cast<char*>(static_cast<std::uintptr_t>(destination)),
                                        source);

            destination += 0x68;
            source += 0x68;
        }
    }
    catch (...)
    {
        int completed = destinationBegin;
        while (completed != destination)
        {
            TextDatabase_ResetBuffers(
                reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(completed)),
                nullptr);
            completed += 0x68;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x00770FA0, CopySoundAreaRange_770FA0, "auto");
