#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __cdecl, sub_6D1670, 0x006D1670, (uint32_t* a1, int a2));

// Dispatches a light-related object (a2) through its virtual interface and
// registers it into one of this object's light slots, based on the object's
// reported kind. Always registers into the trailing slot at +0x74.
static int __fastcall LightWorld_AddLightBySlotKind(C_00860e68* self, void* /*edx*/, int a2)
{
    void* obj = reinterpret_cast<void*>(a2);

    // Slot +0x18 (index 6): kind query, result minus one selects the branch.
    int kindMinusOne = ((int(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[6])(obj, nullptr) - 1;

    if (kindMinusOne == 0)
    {
        sub_6D1670(&self->field_0034, a2);
    }
    else if (kindMinusOne == 1)
    {
        sub_6D1670(&self->field_0044, a2);
        // Slot +0x1C (index 7): secondary kind query; value 3 adds a second slot.
        if (((int(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[7])(obj, nullptr) == 3)
        {
            sub_6D1670(&self->field_0054, a2);
        }
    }

    sub_6D1670(reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 0x74), a2);

    // Slot +0x4 (index 1) with argument 1.
    return ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[1])(obj, nullptr, 1);
}

HOOK(0x6D1F30, LightWorld_AddLightBySlotKind, "auto");
