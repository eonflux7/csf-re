#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(int, __fastcall, ApplyEffectPass, 0x00708DE0,
               (void* self, void* edx, int a2, char a3));

static int __fastcall C_00862b58_RenderFourPasses(C_00862b58* self, void* /*edx*/)
{
    self->field_004c = 0;
    self->field_0048 = 4.0f;
    self->field_0044 = 4.0f;
    self->field_0030 = 0;

    auto* pass = reinterpret_cast<uint8_t*>(self) + 0x90;
    int remaining = 4;
    do
    {
        self->field_0044 = CSF_AT(float, pass, -8);
        self->field_0048 = CSF_AT(float, pass, -4);
        self->field_003c = CSF_AT(float, pass, 0);
        const uint32_t passBits = CSF_AT(uint32_t, pass, 0);
        const uint32_t context = self->field_0060;
        self->field_0040 = passBits;
        const char mode = CSF_AT(char,
            reinterpret_cast<void*>(self->field_0054), 0x28);
        ApplyEffectPass(self, nullptr, static_cast<int>(context), mode);
        pass += 0x10;
        --remaining;
    } while (remaining);

    self->field_0040 = 0;
    self->field_003c = 0.0f;
    self->field_0048 = 1.0f;
    self->field_0044 = 1.0f;
    self->field_004c = 1;
    return 1065353216;
}

HOOK(0x006F9A80, C_00862b58_RenderFourPasses, "auto");
