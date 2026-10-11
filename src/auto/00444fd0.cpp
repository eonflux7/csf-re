#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

static int __fastcall S_00444fd0_ResetAndRelease(S_00444fd0* self, void* /*edx*/)
{
    self->field_0008 = 0;
    self->field_000c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_0044 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0050 = 0;

    auto* object = reinterpret_cast<uint8_t*>(self->field_0054);
    self->field_0058 = 0;
    self->field_005c = 0;

    int result = 0;
    if (object)
    {
        if (CSF_AT(uint32_t, object, -4))
        {
            result = reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, object, 0)[0])(object, nullptr, 3);
        }
        else
        {
            result = Mem_Free(object - 4);
        }
        self->field_0054 = 0;
    }
    return result;
}

HOOK(0x00444FD0, S_00444fd0_ResetAndRelease, "auto");
