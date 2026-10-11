#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static CSF_FUNC(uint32_t*, __fastcall, GrowRecordStorage, 0x00446340,
               (void* self, void* edx, int a2));

static uint32_t* __fastcall S_00446340_AppendRecord(
    S_00446340* self, void* /*edx*/, uint32_t* source, int growth)
{
    // The packet also identifies this address as a vtable reference.
    (void)CSF_VTBL(0x00446340);

    if (self->field_0008 >= self->field_0004)
    {
        GrowRecordStorage(self, nullptr,
                          growth ? growth : static_cast<int>(self->field_000c));
    }

    const uint32_t count = self->field_0008;
    uint32_t* result =
        reinterpret_cast<uint32_t*>(self->field_0000 + 36 * count);
    self->field_0008 = count + 1;

    result[0] = source[0];
    result[2] = source[2];
    result[3] = source[3];
    result[4] = source[4];
    result[5] = source[5];
    result[6] = source[6];
    result[7] = source[7];
    result[8] = source[8];
    return result;
}

HOOK(0x00446C10, S_00446340_AppendRecord, "auto");
