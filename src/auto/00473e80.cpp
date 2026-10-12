#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, SerializeRecordBase, 0x00473BE0, (void* a1, int a2));

static int __cdecl SetRecordValueAndSerialize(
    int a1, int a2, int a3, char a4, char a5,
    int /*a6*/, int /*a7*/, int /*a8*/, int /*a9*/, int /*a10*/)
{
    void* record = reinterpret_cast<void*>(a1);
    CSF_AT(int, record, 0x1C) = a3;

    if (CSF_AT(uint8_t, record, 0) == 0)
    {
        CSF_AT(char, record, 0x15) = a4;
        CSF_AT(char, record, 0x16) = a5;
        return SerializeRecordBase(reinterpret_cast<void*>(a2), a1);
    }

    return a1;
}

HOOK(0x00473E80, SetRecordValueAndSerialize, "auto");
