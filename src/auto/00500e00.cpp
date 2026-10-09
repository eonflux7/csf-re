#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, sub_500DE0, 0x00500DE0, (void* a1, int a2));
CSF_FUNC(void, __fastcall, DestroyRecord_4FF490, 0x004FF490, (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_RecordRangeExceptionInfo, 0x0088A8B0);

static int __cdecl CopyRecordRangeWithHandler_500E00(
    std::uint8_t* first,
    std::uint8_t* last,
    std::uint8_t* destination,
    void* context)
{
    (void)&g_RecordRangeExceptionInfo;

    const std::uint8_t* destinationBegin = destination;
    std::uint8_t* source = first;

    try
    {
        while (source != last)
        {
            sub_500DE0(destination, reinterpret_cast<int>(source));

            destination += 0x58;
            source += 0x58;
        }
    }
    catch (...)
    {
        std::uint8_t* completed = const_cast<std::uint8_t*>(destinationBegin);
        while (completed != destination)
        {
            DestroyRecord_4FF490(completed, nullptr);
            completed += 0x58;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(destination));
}

HOOK(0x00500E00, CopyRecordRangeWithHandler_500E00, "auto");
