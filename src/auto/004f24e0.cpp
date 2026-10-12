#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __stdcall, HudStringShow_CopyElement, 0x004F24C0,
         (void* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, HudStringShow_DestroyElement, 0x004F2240,
         (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_HudStringShowRangeExceptionInfo, 0x0088A64C);

static int __cdecl HudStringShow_CopyElementRange(
    int first, int last, int destination, int context, int unused)
{
    (void)&g_HudStringShowRangeExceptionInfo;
    (void)context;
    (void)unused;

    const int destinationBegin = destination;
    int source = first;

    try
    {
        while (source != last)
        {
            HudStringShow_CopyElement(
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(destination)),
                source);
            destination += 124;
            source += 124;
        }
    }
    catch (...)
    {
        int completed = destinationBegin;
        while (completed != destination)
        {
            HudStringShow_DestroyElement(
                reinterpret_cast<void*>(static_cast<std::uintptr_t>(completed)),
                nullptr);
            completed += 124;
        }
        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x004F24E0, HudStringShow_CopyElementRange, "auto");
