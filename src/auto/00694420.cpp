#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee: sub_694180 (__thiscall, ret 16). Bound by address with the packet's signature.
CSF_FUNC(void, __fastcall, sub_694180, 0x00694180, (void* self, void* edx, void* a2, void* a3, uint32_t* a4, void* a5));

// Data: 7-byte header tag "CSFFBS\0" split into the widths the original copies.
CSF_GLOBAL(uint32_t, g_HeaderTagDword, 0x0085E670);
CSF_GLOBAL(uint16_t, g_HeaderTagWord, 0x0085E674);
CSF_GLOBAL(uint8_t, g_HeaderTagByte, 0x0085E676);
CSF_GLOBAL(uint32_t, g_DefaultRecordValue, 0x008A66B0);

static void __fastcall CorePaths_WriteCsfHeaderRecord(S_00694180* self, void* /*edx*/, int a2, uint32_t* a3)
{
    uint8_t* out = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));

    CSF_AT(uint32_t, out, 0x0) = g_HeaderTagDword;
    CSF_AT(uint16_t, out, 0x4) = g_HeaderTagWord;
    CSF_AT(uint8_t, out, 0x6) = g_HeaderTagByte;
    CSF_AT(uint32_t, out, 0x8) = g_DefaultRecordValue;
    CSF_AT(uint32_t, out, 0xC) = 0;
    CSF_AT(uint32_t, out, 0x10) = 0;
    CSF_AT(uint32_t, out, 0x14) = 0;

    if (self->field_0014 != 0)
    {
        if (self->field_0000 != 0)
        {
            sub_694180(self, nullptr,
                       reinterpret_cast<void*>(out),
                       reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0000)),
                       nullptr,
                       reinterpret_cast<void*>(a3));
            CSF_AT(uint32_t, out, 0x10) = self->field_0024;
            CSF_AT(uint32_t, out, 0x14) = self->field_0034;
            return;
        }
    }
    else
    {
        CSF_AT(uint32_t, out, 0xC) = CSF_AT(uint32_t, self, 0x18);
    }

    CSF_AT(uint32_t, out, 0x10) = self->field_0024;
    CSF_AT(uint32_t, out, 0x14) = self->field_0034;
}

HOOK(0x00694420, CorePaths_WriteCsfHeaderRecord, "auto");
