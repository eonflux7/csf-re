#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, sub_43EE20, 0x0043EE20, (void* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, DestroyRecord_43E300, 0x0043E300, (std::uint32_t* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInfo));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_CampaignRecordRangeExceptionInfo, 0x0088704C);

static int __cdecl CopyCampaignRecordRange_43EE60(
    int first,
    int last,
    int destination,
    int context,
    int unused)
{
    (void)&g_CampaignRecordRangeExceptionInfo;
    (void)context;
    (void)unused;

    const int destinationBegin = destination;
    int source = first;

    try
    {
        while (source != last)
        {
            sub_43EE20(reinterpret_cast<void*>(static_cast<std::uintptr_t>(destination)),
                       source);

            destination += 0x38;
            source += 0x38;
        }
    }
    catch (...)
    {
        int completed = destinationBegin;
        while (completed != destination)
        {
            DestroyRecord_43E300(
                reinterpret_cast<std::uint32_t*>(static_cast<std::uintptr_t>(completed)),
                nullptr);
            completed += 0x38;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x0043EE60, CopyCampaignRecordRange_43EE60, "auto");
