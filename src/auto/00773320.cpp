#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(char*, __stdcall, sub_773300, 0x00773300, (char* a1, int a2));
CSF_FUNC(int, __fastcall, Tables_ClearRecordBuffers, 0x0051ABD0, (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));

CSF_GLOBAL(std::uint8_t, g_AnimationDefsRangeExceptionInfo, 0x00894454);

static int* __cdecl AnimationDefs_CopyRange_773320(
    int first,
    int last,
    int* destination,
    int context,
    int unused)
{
    (void)&g_AnimationDefsRangeExceptionInfo;
    (void)unused;

    const std::uintptr_t destinationBegin = reinterpret_cast<std::uintptr_t>(destination);
    std::uintptr_t dest = destinationBegin;
    int source = first;

    try
    {
        while (source != last)
        {
            sub_773300(reinterpret_cast<char*>(dest), source);

            dest += 0x60;
            source += 0x60;
        }
    }
    catch (...)
    {
        std::uintptr_t completed = destinationBegin;
        while (completed != dest)
        {
            Tables_ClearRecordBuffers(reinterpret_cast<void*>(completed), nullptr);
            completed += 0x60;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    (void)context;
    return reinterpret_cast<int*>(dest);
}

HOOK(0x00773320, AnimationDefs_CopyRange_773320, "auto");
