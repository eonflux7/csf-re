#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x00668B70: __cdecl, no args, returns a pointer (tested for null)
CSF_FUNC(char*, __cdecl, sub_668B70, 0x00668B70, ());
// 0x00680D30: __thiscall (ecx = this), two stack args, ret 8
CSF_FUNC(int, __fastcall, sub_680D30, 0x00680D30, (void* self, void* edx, const char* settingName, const uint8_t* defaultValue));

static char* GetActiveCharacterRecord()
{
    return sub_668B70();
}

static int CharacterRecord_ApplyNamedSetting(void* record, const char* settingName, const uint8_t* defaultValue)
{
    return sub_680D30(record, nullptr, settingName, defaultValue);
}

static int MpSpawnPoints_ApplyCharacterSelection()
{
    char* firstRecord = GetActiveCharacterRecord();
    if (firstRecord)
    {
        void* record = GetActiveCharacterRecord();
        return CharacterRecord_ApplyNamedSetting(
            record,
            CSF_STR(0x00867624, "MaxJugs"),
            reinterpret_cast<const uint8_t*>(CSF_ADDR_DATA(0x008445D8)));
    }
    return static_cast<int>(reinterpret_cast<intptr_t>(firstRecord));
}

HOOK(0x0073C900, MpSpawnPoints_ApplyCharacterSelection, "auto");
