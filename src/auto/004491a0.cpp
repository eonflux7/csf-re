#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __stdcall, CopyConstructRecord_449170, 0x00449170,
         (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __stdcall, DestroyRecord_448D50, 0x00448D50, (int a1));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));
CSF_FUNC(int, __cdecl, ___CxxFrameHandler, 0x007AF226,
         (void* exceptionRecord, void* frame, void* context, void* dispatcher));

CSF_GLOBAL(std::uint8_t, g_CopyRecordRangeFunctionInfo, 0x008876F8);

static uint32_t* __cdecl CopyConstructRecordRange(
    uint32_t* sourceBegin,
    uint32_t* sourceEnd,
    uint32_t* destination,
    int /*contextEcx*/,
    int /*unusedArg5*/)
{
    uint32_t* const destinationBegin = destination;

    try
    {
        while (sourceBegin != sourceEnd)
        {
            CopyConstructRecord_449170(destination, sourceBegin);
            destination += 5;
            sourceBegin += 5;
        }
    }
    catch (...)
    {
        uint32_t* record = destinationBegin;
        while (record != destination)
        {
            DestroyRecord_448D50(
                static_cast<int>(reinterpret_cast<std::intptr_t>(record)));
            record += 5;
        }

        RethrowCppException(nullptr, nullptr);
        __assume(0);
    }

    return destination;
}

HOOK(0x004491A0, CopyConstructRecordRange, "auto");
