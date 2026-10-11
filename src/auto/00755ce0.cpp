#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(void*, __stdcall, CopyElementRecord_755CC0, 0x00755CC0, (void* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, SceneLoader_ReleaseAndResetBuffers, 0x00500210, (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_CopyRangeFuncInfo, 0x00892FC4);

static void* __cdecl CopyRangeWithCleanup_755CE0(
    std::uint8_t* first,
    std::uint8_t* last,
    std::uint8_t* destination,
    int unusedA4,
    int unusedA5)
{
    (void)&g_CopyRangeFuncInfo;
    (void)unusedA4;
    (void)unusedA5;

    std::uint8_t* destinationBegin = destination;
    std::uint8_t* source = first;

    try
    {
        while (source != last)
        {
            CopyElementRecord_755CC0(destination, reinterpret_cast<int>(source));

            destination += 0x50;
            source += 0x50;
        }
    }
    catch (...)
    {
        std::uint8_t* completed = destinationBegin;
        while (completed != destination)
        {
            SceneLoader_ReleaseAndResetBuffers(completed, nullptr);
            completed += 0x50;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x00755CE0, CopyRangeWithCleanup_755CE0, "auto");
