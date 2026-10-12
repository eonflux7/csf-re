#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void**, __stdcall, sub_775520, 0x00775520, (void* a1, int a2));
CSF_FUNC(char, __fastcall, sub_774DC0, 0x00774DC0, (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static int __cdecl ConstructRecordRange(void* sourceBegin, void* sourceEnd, void* destBegin,
                                        int /*unused a4*/, int /*unused a5*/)
{
    const std::uintptr_t begin = reinterpret_cast<std::uintptr_t>(destBegin);
    std::uintptr_t dest = begin;
    std::uintptr_t source = reinterpret_cast<std::uintptr_t>(sourceBegin);
    const std::uintptr_t sourceLast = reinterpret_cast<std::uintptr_t>(sourceEnd);

    try
    {
        while (source != sourceLast)
        {
            sub_775520(reinterpret_cast<void*>(dest), static_cast<int>(source));
            dest += 168;
            source += 168;
        }
    }
    catch (...)
    {
        for (std::uintptr_t record = begin; record != dest; record += 168)
        {
            sub_774DC0(reinterpret_cast<void*>(record), nullptr);
        }
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(dest);
}

HOOK(0x00775560, ConstructRecordRange, "auto");
