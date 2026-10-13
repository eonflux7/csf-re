#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, MissionScript_CopyRecordEntry, 0x00782910, (void* a1, void* a2));
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

// Copies a range of 348-byte records from [first, last) into dest, one entry at a time.
// On exception, releases the records already written and rethrows.
// Returns the pointer one past the last written element.
static int __cdecl MissionScript_CopyRecordRange(int first, int last, int dest, int unused4, int unused5)
{
    int start = dest;
    int out = dest;
    try
    {
        while (first != last)
        {
            MissionScript_CopyRecordEntry(reinterpret_cast<void*>(out), reinterpret_cast<void*>(first));
            out += 0x15c;
            first += 0x15c;
        }
    }
    catch (...)
    {
        for (int p = start; p != out; p += 0x15c)
        {
            MissionScript_ReleaseRecordBuffer(reinterpret_cast<int*>(p));
        }
        RethrowCppException(nullptr, nullptr);
    }
    return out;
}

HOOK(0x00782930, MissionScript_CopyRecordRange, "auto");
