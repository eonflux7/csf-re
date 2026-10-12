#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t, __fastcall, MissionScript_AssignEntryToSlot_697AD0, 0x00697AD0, (void* self, void* edx, C_0082d410* source, C_0082d410* destination));
CSF_FUNC(int, __stdcall, MissionScript_ReleaseRecordBuffer, 0x00469270, (int* a1));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226, (void* exceptionRecord, void* frame, void* context, void* dispatcher));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666, (void* exceptionObject, void* throwInformation));

CSF_GLOBAL(std::uint8_t, g_CoreFilesCopyRangeFuncInfo, 0x0088F160);

static int __cdecl CoreFiles_CopyConstructRecordRange(
    void* sourceBegin,
    void* sourceEnd,
    void* destination,
    void* context,
    int /*unusedArg5*/)
{
    void* const destinationBegin = destination;

    try
    {
        while (sourceBegin != sourceEnd)
        {
            // Stack argument order follows the assembly, not the binding's parameter names.
            MissionScript_AssignEntryToSlot_697AD0(
                context, nullptr,
                static_cast<C_0082d410*>(destination),
                static_cast<C_0082d410*>(sourceBegin));
            destination = static_cast<std::uint8_t*>(destination) + 32;
            sourceBegin = static_cast<std::uint8_t*>(sourceBegin) + 32;
        }
    }
    catch (...)
    {
        void* handled = destinationBegin;
        while (handled != destination)
        {
            MissionScript_ReleaseRecordBuffer(static_cast<int*>(handled));
            handled = static_cast<std::uint8_t*>(handled) + 32;
        }
        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return static_cast<int>(reinterpret_cast<std::intptr_t>(destination));
}

HOOK(0x00697B10, CoreFiles_CopyConstructRecordRange, "auto");
