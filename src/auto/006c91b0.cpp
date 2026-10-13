#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, sub_6C9080, 0x006C9080, (void* a1, int a2));
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

static int __cdecl MissionEntries_ForwardArgToEach(
    std::uint8_t* first,
    std::uint32_t count,
    int forwardArg,
    int /*unusedArg4*/,
    int /*contextArg*/)
{
    std::uint8_t* currentEntry = first;
    int result = -1;

    try
    {
        while (count != 0)
        {
            result = sub_6C9080(currentEntry, forwardArg);
            --count;
            currentEntry += 0x24;
        }
    }
    catch (...)
    {
        std::uint8_t* entry = first;
        while (entry != currentEntry)
        {
            MissionScript_ReleaseRecordBuffer(reinterpret_cast<int*>(entry));
            entry += 0x24;
        }

        RethrowCppException(nullptr, nullptr);
        __builtin_unreachable();
    }

    return result;
}

HOOK(0x006C91B0, MissionEntries_ForwardArgToEach, "auto");
