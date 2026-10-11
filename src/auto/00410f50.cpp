#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int*, __fastcall, S_004109c0_CopyHeader, 0x00410CB0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, S_0046bea0_Copy, 0x00410E70, (void* self, void* edx, int a2));

static int __fastcall S_004109c0_Copy(S_004109c0* self, void* /*edx*/, int sourceAddress)
{
    const auto* source = reinterpret_cast<const S_004109c0*>(sourceAddress);

    S_004109c0_CopyHeader(self, nullptr, sourceAddress);
    self->field_0010 = source->field_0010;
    self->field_0014 = source->field_0014;
    self->field_0018 = source->field_0018;
    self->field_001c = source->field_001c;
    self->field_0020 = source->field_0020;
    self->field_0024 = source->field_0024;
    self->field_0028 = source->field_0028;
    self->field_002c = source->field_002c;
    self->field_0030 = source->field_0030;
    self->field_0034 = source->field_0034;
    self->field_0038 = source->field_0038;
    self->field_003c = source->field_003c;
    self->field_0040 = source->field_0040;
    self->field_0044 = source->field_0044;
    self->field_0048 = source->field_0048;
    self->field_004c = source->field_004c;
    self->field_0050 = source->field_0050;
    self->field_0054 = source->field_0054;
    self->field_0058 = source->field_0058;
    self->field_005c = source->field_005c;
    self->field_0060 = source->field_0060;
    self->field_0064 = source->field_0064;
    self->field_0068 = source->field_0068;
    self->field_006c = source->field_006c;
    self->field_0070 = source->field_0070;
    self->field_0074 = source->field_0074;
    self->field_0078 = source->field_0078;
    self->field_007c = source->field_007c;
    self->field_0080 = source->field_0080;
    self->field_0084 = source->field_0084;
    self->field_0088 = source->field_0088;
    self->field_008c = source->field_008c;
    self->field_0090 = source->field_0090;

    S_0046bea0_Copy(reinterpret_cast<uint8_t*>(self) + 0x94,
                    nullptr, sourceAddress + 148);

    self->field_00a4 = source->field_00a4;
    self->field_00a8 = source->field_00a8;
    self->field_00ac = source->field_00ac;
    self->field_00b0 = source->field_00b0;
    self->field_00b4 = source->field_00b4;
    self->field_00b8 = source->field_00b8;
    self->field_00bc = source->field_00bc;
    self->field_00c0 = source->field_00c0;
    self->field_00c4 = source->field_00c4;
    self->field_00c8 = source->field_00c8;
    self->field_00cc = source->field_00cc;
    self->field_00d0 = source->field_00d0;
    self->field_00d4 = source->field_00d4;
    self->field_00d8 = source->field_00d8;
    self->field_00dc = source->field_00dc;
    self->field_00e0 = source->field_00e0;
    self->field_00e4 = source->field_00e4;
    self->field_00e8 = source->field_00e8;
    self->field_00ec = source->field_00ec;
    self->field_00f0 = source->field_00f0;
    self->field_00f4 = source->field_00f4;
    self->field_00f8 = source->field_00f8;
    self->field_00fc = source->field_00fc;
    self->field_0100 = source->field_0100;
    self->field_0104 = source->field_0104;
    self->field_0108 = source->field_0108;
    self->field_010c = source->field_010c;
    self->field_0110 = source->field_0110;
    self->field_0114 = source->field_0114;
    self->field_0115 = source->field_0115;
    self->field_0118 = source->field_0118;
    self->field_011c = source->field_011c;

    S_0046bea0_Copy(reinterpret_cast<uint8_t*>(self) + 0x120,
                    nullptr, sourceAddress + 288);

    self->field_0130 = source->field_0130;
    self->field_0131 = source->field_0131;
    self->field_0134 = source->field_0134;
    self->field_0138 = source->field_0138;
    self->field_013c = source->field_013c;
    self->field_0140 = source->field_0140;
    self->field_0144 = source->field_0144;
    self->field_0148 = source->field_0148;
    self->field_014c = source->field_014c;
    self->field_0150 = source->field_0150;
    self->field_0154 = source->field_0154;
    self->field_0158 = source->field_0158;
    self->field_015c = source->field_015c;
    self->field_0160 = source->field_0160;
    self->field_0164 = source->field_0164;
    self->field_0168 = source->field_0168;
    self->field_016c = source->field_016c;
    self->field_0170 = source->field_0170;
    self->field_0174 = source->field_0174;
    self->field_0178 = source->field_0178;
    self->field_017c = source->field_017c;
    self->field_0180 = source->field_0180;
    self->field_0184 = source->field_0184;
    self->field_0188 = source->field_0188;
    self->field_018c = source->field_018c;
    self->field_0190 = source->field_0190;
    self->field_0194 = source->field_0194;
    self->field_0198 = source->field_0198;
    self->field_019c = source->field_019c;
    self->field_01a0 = source->field_01a0;
    self->field_01a4 = source->field_01a4;
    self->field_01a8 = source->field_01a8;
    self->field_01ac = source->field_01ac;
    self->field_01b0 = source->field_01b0;
    self->field_01b4 = source->field_01b4;
    self->field_01b8 = source->field_01b8;
    self->field_01bc = source->field_01bc;
    self->field_01c0 = source->field_01c0;
    self->field_01c4 = source->field_01c4;
    self->field_01c8 = source->field_01c8;
    self->field_01cc = source->field_01cc;
    self->field_01d0 = source->field_01d0;
    self->field_01d4 = source->field_01d4;
    self->field_01d8 = source->field_01d8;
    self->field_01dc = source->field_01dc;
    self->field_01e0 = source->field_01e0;
    self->field_01e1 = source->field_01e1;
    self->field_01e4 = source->field_01e4;
    self->field_01e8 = source->field_01e8;
    self->field_01ec = source->field_01ec;
    self->field_01f0 = source->field_01f0;
    self->field_01f4 = source->field_01f4;
    self->field_01f8 = source->field_01f8;
    self->field_01fc = source->field_01fc;
    self->field_0200 = source->field_0200;
    self->field_0204 = source->field_0204;
    self->field_0208 = source->field_0208;
    self->field_020c = source->field_020c;
    self->field_0210 = source->field_0210;
    self->field_0214 = source->field_0214;
    self->field_0218 = source->field_0218;
    self->field_021c = source->field_021c;
    self->field_0220 = source->field_0220;
    self->field_0224 = source->field_0224;
    self->field_0228 = source->field_0228;

    return static_cast<int>(reinterpret_cast<uintptr_t>(self));
}

HOOK(0x00410F50, S_004109c0_Copy, "auto");
