#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));

// Serializes a string field guarded by a per-object state byte (pad_0000[0]).
// The object's first virtual check (slot 32) decides whether the string is
// serialized this pass; slot 31 decides whether the state is reported as done.
static int __fastcall SerializeGatedString(void* self, void* /*edx*/, S_00691860* a2)
{
    uint32_t savedNext = a2[1].field_0004;
    void* sub = reinterpret_cast<void*>(a2->field_0004);

    bool v4 = a2->pad_0000[0] == 0
        && ((uint8_t(__fastcall*)(void*, void*, int, S_00691860*))CSF_AT(void**, sub, 0)[32])(sub, nullptr, 0, a2) == 0;

    a2->pad_0000[0] = v4;
    a2[1].field_0004 = 0;

    if (!v4) {
        CSF_AT(uint8_t, a2, 0x15) = 0;
        CSF_AT(uint8_t, a2, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(self), a2);
    }

    a2[1].field_0004 = savedNext;

    sub = reinterpret_cast<void*>(a2->field_0004);
    if (a2->pad_0000[0] != 0
        || ((uint8_t(__fastcall*)(void*, void*, S_00691860*))CSF_AT(void**, sub, 0)[31])(sub, nullptr, a2) == 0) {
        a2->pad_0000[0] = 1;
        return 1;
    }

    a2->pad_0000[0] = 0;
    return 0;
}

HOOK(0x00499130, SerializeGatedString, "auto");
