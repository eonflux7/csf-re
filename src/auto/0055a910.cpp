#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, InitSubObject_55A090, 0x0055A090, (void* self, void* edx));
CSF_FUNC(char, __fastcall, EvaluateStatus_55A800, 0x0055A800, (void* self, void* edx));

static C_00844928* __fastcall C_00844928_InitFromName(C_00844928* self, void* /*edx*/, char* a2, char* a3)
{
    self->field_0004 = 0;
    self->vtbl = CSF_VTBL(0x00844928);
    InitSubObject_55A090(reinterpret_cast<uint8_t*>(self) + 8, nullptr);

    CSF_AT(uint32_t, self, 0x1FC) = 0;
    self->field_0200 = 15;
    CSF_AT(uint8_t, self, 0x1EC) = 0;

    AssignStringRange_429B60(reinterpret_cast<uint8_t*>(self) + 0x1E8, nullptr, a2,
                             static_cast<unsigned int>(std::strlen(a2)));

    *a3 = EvaluateStatus_55A800(self, nullptr);
    return self;
}

HOOK(0x0055A910, C_00844928_InitFromName, "auto");
