#include <cstdint>
#include "globals.h"
#include <cstring>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall C_008445f4_ResetMultiplayerState(
    C_008445f4* self, void* /*edx*/)
{
    std::memset(&CSF_AT(uint8_t, self, 0x7E4), 0, 0x7D0u);

    self->field_0fbc = 0;
    self->field_0fd0 = 0;
    self->field_0fc0 = 0;
    self->field_0fd4 = 0;
    self->field_0fc4 = 0;
    self->field_0fd8 = 0;
    self->field_0fc8 = 0;
    self->field_0fdc = 0;
    self->field_0fcc = 0;
    self->field_0fe0 = 0;

    for (int index = 0; index < 11; ++index)
    {
        CSF_AT(uint32_t, self, 0xFE4 + index * 4) = 0;
        CSF_AT(uint32_t, self, 0x1010 + index * 4) = 0;
    }

    self->field_103c = 0;
    self->field_1064 = 0;
    self->field_1078 = 0;
    self->field_1050 = 0;
    self->field_1040 = 0;
    self->field_1068 = 0;
    self->field_107c = 0;
    self->field_1054 = 0;
    self->field_1044 = 0;
    self->field_106c = 0;
    self->field_1080 = 0;
    self->field_1058 = 0;
    self->field_1048 = 0;
    self->field_1070 = 0;
    self->field_1084 = 0;
    self->field_105c = 0;
    self->field_104c = 0;
    self->field_1074 = 0;
    self->field_1088 = 0;
    self->field_1060 = 0;

    std::memset(&CSF_AT(uint8_t, self, 0x10), 0, 0x7D0u);

    self->field_0fb4 = 0;
    self->field_0fb8 = 0;
    self->field_000c = 0;
    self->field_000d = 0;
    self->field_000e = 0;
    self->field_1094 = 0;
    self->field_108c = 0;
    self->field_1090 = 0;
    self->field_1098 = 0;
    self->field_1099 = 0;
    self->field_109a = 0;
    self->field_109b = 0;
    self->field_109c = 0;

    return 0;
}

HOOK(0x0054EE80, C_008445f4_ResetMultiplayerState, "auto");
