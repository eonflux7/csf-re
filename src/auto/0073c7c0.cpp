#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char*, __cdecl, sub_668B70, 0x00668B70, ());
CSF_FUNC(C_008310b8*, __fastcall, ParamTable_SetNamedValue, 0x00680D30,
         (C_008310b8* self, void* edx, const char* name, int value));

// Stores `mapNameValue` under the "NombreMapa" key in the parameter table
// obtained from the global table accessor. Returns the accessor's result
// (null when the table is not available).
static C_008310b8* __stdcall MpSpawnPoints_SetMapNameParameter(int mapNameValue)
{
    C_008310b8* result = reinterpret_cast<C_008310b8*>(sub_668B70());
    if (result) {
        C_008310b8* table = reinterpret_cast<C_008310b8*>(sub_668B70());
        return ParamTable_SetNamedValue(table, nullptr, CSF_STR(0x867634, "NombreMapa"), mapNameValue);
    }
    return result;
}

HOOK(0x0073C7C0, MpSpawnPoints_SetMapNameParameter, "auto");
