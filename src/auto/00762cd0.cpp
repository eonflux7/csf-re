#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __stdcall, CharacterDefs_CopyConstructRecord, 0x00762CB0,
         (char* a1, int a2));
CSF_FUNC(int, __fastcall, CharacterDefs_DestroyRecord, 0x00511040,
         (void* self, void* edx));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static std::uint32_t* __cdecl CharacterDefs_CopyConstructRange(
    int sourceBegin, int sourceEnd, std::uint32_t* destinationBegin,
    int /*constructionContext*/, int /*unused*/)
{
    std::uint32_t* destination = destinationBegin;
    std::uint32_t source = static_cast<std::uint32_t>(sourceBegin);

    try
    {
        while (source != static_cast<std::uint32_t>(sourceEnd))
        {
            CharacterDefs_CopyConstructRecord(
                reinterpret_cast<char*>(destination),
                static_cast<int>(source));
            destination += 139;
            source += 556;
        }
    }
    catch (...)
    {
        for (std::uint32_t* record = destinationBegin;
             record != destination; record += 139)
        {
            CharacterDefs_DestroyRecord(record, nullptr);
        }
        RethrowCppException(nullptr, nullptr);
    }

    return destination;
}

HOOK(0x00762CD0, CharacterDefs_CopyConstructRange, "auto");
