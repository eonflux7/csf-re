#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, WeaponFirearms_SetEntry, 0x00476170, (void* self, void* , void* entry));

static void __fastcall C_00833708_SetLastFirearmEntry(
    C_00833708* self, void* /*edx*/, int index, void* entry)
{
    self->vSlotA8(index, 0);

    const uint32_t lastRecordAddress =
        self->field_000c + 96u * self->field_0014 - 96u;
    WeaponFirearms_SetEntry(
        CSF_AT(void*, reinterpret_cast<void*>(lastRecordAddress), 0),
        nullptr, entry);
}

HOOK(0x00489C70, C_00833708_SetLastFirearmEntry, "auto");
