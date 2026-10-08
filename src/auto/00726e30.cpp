#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __cdecl, SceneNamesMatch, 0x00694AA0,
         (char* lookupString, uint8_t* entryString));

static int __fastcall C_008655e8_FindEntryByName(
    C_008655e8* self, void* /*edx*/, char* name)
{
    const uint32_t entryCount = self->field_003c;

    for (uint32_t index = 0; index < entryCount; ++index)
    {
        void* entry = reinterpret_cast<void*>(
            reinterpret_cast<uint32_t*>(self->field_0034)[index]);

        uint8_t* entryName =
            reinterpret_cast<uint8_t* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, entry, 0)[3])(entry, nullptr);

        if (SceneNamesMatch(name, entryName))
        {
            return static_cast<int>(
                reinterpret_cast<uint32_t*>(self->field_0034)[index]);
        }
    }

    return 0;
}

HOOK(0x00726E30, C_008655e8_FindEntryByName, "auto");
