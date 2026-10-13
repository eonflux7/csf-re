#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));

static uint32_t* __fastcall C_00863d18_Destroy(
    C_00863d18* self, void* /*edx*/, char flags)
{
    uint32_t secondObject = self->field_0008;
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00863D18);

    void* releaseContext = self;
    if (secondObject != 0) {
        void* object = reinterpret_cast<void*>(secondObject);
        void** table = CSF_AT(void**, object, 0);
        releaseContext = table;
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            table[2])(releaseContext, nullptr, object);
        self->field_0008 = 0;
    }

    uint32_t firstObject = self->field_0004;
    if (firstObject != 0) {
        void* object = reinterpret_cast<void*>(firstObject);
        void** table = CSF_AT(void**, object, 0);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            table[2])(releaseContext, table, object);
        self->field_0004 = 0;
    }

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x00863D04);
    if ((flags & 1) != 0)
        CoreMemory_Free(self);

    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x007118C0, C_00863d18_Destroy, "auto");
