#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __stdcall, Campaign_CopyConstructRecord, 0x0043EE40, (char* a1, int a2));
CSF_FUNC(unsigned int, __fastcall, Campaign_DestroyRecord, 0x0043E360,
         (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static void* __cdecl Campaign_CopyConstructRecordRange(int sourceBegin, int sourceEnd,
                                                       void* destBegin, int copyContext,
                                                       int /*unused*/)
{
    const std::uintptr_t begin = reinterpret_cast<std::uintptr_t>(destBegin);
    const std::uintptr_t sourceLast = static_cast<std::uintptr_t>(sourceEnd);
    std::uintptr_t dest = begin;
    std::uintptr_t source = static_cast<std::uintptr_t>(sourceBegin);

    try
    {
        while (static_cast<int>(source) != static_cast<int>(sourceLast))
        {
            Campaign_CopyConstructRecord(reinterpret_cast<char*>(dest),
                                         static_cast<int>(source));
            dest += 196;
            source += 196;
        }
    }
    catch (...)
    {
        for (std::uintptr_t record = begin; record != dest; record += 196)
        {
            Campaign_DestroyRecord(reinterpret_cast<void*>(record), nullptr);
        }
        RethrowCppException(nullptr, nullptr);
    }

    return reinterpret_cast<void*>(dest);
}

HOOK(0x0043EEF0, Campaign_CopyConstructRecordRange, "auto");

