#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original calls C_0082e598_ClearStateFields (0x41E230), then falls into a tail jump to 0x425B20.
// 0x425B20 is not a registered function start, so its body is reproduced inline: it only writes
// the embedded block at this+0x24 and returns 0 in eax.
CSF_FUNC(int, __fastcall, C_0082e598_ClearStateFields, 0x0041E230, (C_0082e598* self, void*));

// Resets the task state of a C_0082faf8 object to its initial values.
static int __fastcall C_0082faf8_ResetTaskState(C_0082faf8* self, void* /*edx*/)
{
    C_0082e598_ClearStateFields(self, nullptr);

    // Read the referenced object before the field writes, as the original does.
    void* taskState = reinterpret_cast<void*>(self->field_0014);
    float initialSpeed = CSF_AT(float, taskState, 0x184);

    self->field_0020 = 0;
    self->field_0050 = 0.0f;
    self->field_0058 = 0.0f;
    self->field_005c = 0.0f;
    self->field_0074 = 0;
    self->field_0060 = 0;
    self->field_0054 = initialSpeed;
    self->field_0064 = 0xFFFFFFFFu;
    self->field_0068 = 0xFFFFFFFFu;
    self->field_006c = 0xFFFFFFFFu;

    // Embedded block at this+0x24 (passed as ecx to the inlined tail routine).
    // Its fields: +0x24 -> this+0x48, +0x28 -> this+0x4C, +0x0C -> this+0x30, +0x1C -> this+0x40.
    CSF_AT(uint32_t, self, 0x48) = 0xFFFFFFFFu;
    CSF_AT(uint8_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x30) = 0;
    self->field_0040 = 0;

    return 0;
}

HOOK(0x00424030, C_0082faf8_ResetTaskState, "auto");
