#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00865aa8_InitializeFromData, 0x007284A0,
         (C_00865aa8* self, void* edx, int data, int parameter));

CSF_GLOBAL(float, g_ValueScale, 0x0082D4EC);

static int __fastcall C_00865aa8_InitializeScaledValues(
    C_00865aa8* self, void* /*edx*/, int data, int parameter)
{
    const int result =
        C_00865aa8_InitializeFromData(self, nullptr, data, parameter);

    const void* source = reinterpret_cast<const void*>(data);
    self->field_005c = CSF_AT(float, source, 0x0C) * g_ValueScale;

    const uint32_t firstCopy = CSF_AT(uint32_t, source, 0x0C);
    std::memcpy(&self->field_0060, &firstCopy, sizeof(firstCopy));

    const uint32_t secondCopy = CSF_AT(uint32_t, source, 0x0C);
    std::memcpy(&self->field_0058, &secondCopy, sizeof(secondCopy));

    return result;
}

HOOK(0x00728820, C_00865aa8_InitializeScaledValues, "auto");
