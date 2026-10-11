#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __stdcall, SaveGame_CopyConstructRecord, 0x00512A80, (void* a1, void* a2));
CSF_FUNC(unsigned int, __fastcall, SaveGame_DestroyRecord, 0x00500A80,
         (std::uint32_t* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static int __cdecl SaveGame_CopyConstructRange(int sourceBegin, int sourceEnd, int destBegin,
                                               int /*copyContext*/, int /*unused*/)
{
    const std::uintptr_t begin = static_cast<std::uintptr_t>(destBegin);
    const std::uintptr_t sourceLast = static_cast<std::uintptr_t>(sourceEnd);
    std::uintptr_t dest = begin;
    std::uintptr_t source = static_cast<std::uintptr_t>(sourceBegin);

    try
    {
        while (source != sourceLast)
        {
            SaveGame_CopyConstructRecord(reinterpret_cast<void*>(dest),
                                         reinterpret_cast<void*>(source));
            dest += 200;
            source += 200;
        }
    }
    catch (...)
    {
        for (std::uintptr_t record = begin; record != dest; record += 200)
        {
            SaveGame_DestroyRecord(reinterpret_cast<std::uint32_t*>(record), nullptr);
        }
        RethrowCppException(nullptr, nullptr);
    }

    return static_cast<int>(dest);
}

HOOK(0x00512AA0, SaveGame_CopyConstructRange, "auto");
