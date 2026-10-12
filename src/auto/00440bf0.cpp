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

// SceneLoader_SerializeRecordBase: __stdcall, 2 stack args (ret 8).
CSF_FUNC(void*, __stdcall, SceneLoader_SerializeRecordBase, 0x0043FAF0, (void* a1, int a2));

// Stores the scene record's 32-bit value and, when the record's first flag byte is
// clear, saves two byte parameters into the record and serializes it.
static int __cdecl Campaign_SetBriefingRecordAndSerialize(int a1, C_0082d410* a2, int a3, char a4, char a5)
{
    void* record = reinterpret_cast<void*>(a1);
    int result = a1;

    CSF_AT(int, record, 0x1C) = a3;

    if (CSF_AT(uint8_t, record, 0) == 0)
    {
        CSF_AT(char, record, 0x15) = a4;
        CSF_AT(char, record, 0x16) = a5;
        return reinterpret_cast<int>(SceneLoader_SerializeRecordBase(a2, a1));
    }

    return result;
}

HOOK(0x440BF0, Campaign_SetBriefingRecordAndSerialize, "auto");
