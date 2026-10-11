#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082e598_ClearStateFields, 0x0041E230, (C_0082e598* self, void* edx));
CSF_FUNC(int, __fastcall, TaskState_InitSubObject, 0x0041ED30, (void* self, void* edx));

static int __fastcall C_0082f9f8_ResetState(C_0082f9f8* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    C_0082e598_ClearStateFields(reinterpret_cast<C_0082e598*>(self), nullptr);

    CSF_AT(uint32_t, base, 0x244) = 1;
    CSF_AT(uint32_t, base, 0x248) = 0;
    CSF_AT(uint32_t, base, 0x24C) = 0;
    CSF_AT(uint32_t, base, 0x250) = 0;
    CSF_AT(uint32_t, base, 0x254) = 0;
    CSF_AT(uint32_t, base, 0x258) = 0;
    CSF_AT(uint32_t, base, 0x1FC) = 0;
    CSF_AT(int, base, 0x270) = -1;
    CSF_AT(uint32_t, base, 0x200) = 0;
    CSF_AT(uint8_t, base, 0x230) = 0;
    CSF_AT(uint8_t, base, 0x274) = 0;

    TaskState_InitSubObject(base + 0x29C, nullptr);

    uint8_t* owner = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_0014));
    CSF_AT(uint32_t, base, 0x2BC) = CSF_AT(uint32_t, owner, 0x1D8);
    CSF_AT(uint32_t, base, 0x2C0) = CSF_AT(uint32_t, owner, 0x1C4);
    uint32_t result = CSF_AT(uint32_t, owner, 0x1E0);
    CSF_AT(uint32_t, base, 0x2C4) = result;
    return static_cast<int>(result);
}

HOOK(0x00420790, C_0082f9f8_ResetState, "auto");
