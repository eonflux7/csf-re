#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_006dcf90_ReleaseResource, 0x006DCF90, (S_006dcf90* self, void*));

CSF_GLOBAL(float, g_AxisY, 0x008A7D78);
CSF_GLOBAL(float, g_AxisY_Y, 0x008A7D7C);
CSF_GLOBAL(float, g_AxisY_Z, 0x008A7D80);
CSF_GLOBAL(float, g_AxisZ, 0x008A7D84);
CSF_GLOBAL(float, g_AxisZ_Y, 0x008A7D88);
CSF_GLOBAL(float, g_AxisZ_Z, 0x008A7D8C);
CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);
CSF_GLOBAL(float, g_ZeroVector2X, 0x008BB448);
CSF_GLOBAL(float, g_ZeroVector2Y, 0x008BB44C);

// The packet also labels the release-function address as a vtable reference.
[[maybe_unused]] static const auto g_ReleaseResourceVtableReference =
    CSF_VTBL(0x006DCF90);

static void StoreFloat(std::uint32_t& field, float value)
{
    std::memcpy(&field, &value, sizeof(value));
}

static int __fastcall S_006dcf90_ResetDefaults(
    S_006dcf90* self, void* /*edx*/)
{
    self->field_0000 = 0;
    S_006dcf90_ReleaseResource(self, nullptr);

    self->field_0008 = 10;
    self->field_000c = 1;
    StoreFloat(self->field_0010, 0.1f);
    self->field_0014 = 0;
    StoreFloat(self->field_0018, 1.0f);
    self->field_001c = 0;
    self->field_0020 = 0;
    self->field_0024 = 0;
    StoreFloat(self->field_0028, g_AxisY);
    CSF_AT(float, self, 0x2C) = g_AxisY_Y;
    CSF_AT(float, self, 0x30) = g_AxisY_Z;
    StoreFloat(self->field_0034, 100.0f);
    StoreFloat(self->field_0038, 100.0f);
    StoreFloat(self->field_003c, g_ZeroVector2X);
    StoreFloat(self->field_0040, g_ZeroVector2Y);
    StoreFloat(self->field_0044, g_ColorBlack);
    CSF_AT(float, self, 0x48) = g_ColorBlack_G;
    CSF_AT(float, self, 0x4C) = g_ColorBlack_B;
    CSF_AT(float, self, 0x50) = g_ColorBlack_A;
    StoreFloat(self->field_0054, g_ColorBlack);
    CSF_AT(float, self, 0x58) = g_ColorBlack_G;
    CSF_AT(float, self, 0x5C) = g_ColorBlack_B;
    CSF_AT(float, self, 0x60) = g_ColorBlack_A;
    StoreFloat(self->field_0064, g_ZeroVectorX);
    CSF_AT(float, self, 0x68) = g_ZeroVectorY;
    CSF_AT(float, self, 0x6C) = g_ZeroVectorZ;
    self->field_0070 = 0;
    StoreFloat(self->field_0074, g_ZeroVector2X);
    StoreFloat(self->field_0078, g_ZeroVector2Y);
    StoreFloat(self->field_007c, 1.0f);
    StoreFloat(self->field_0080, 1.0f);
    self->field_0084 = 0;
    self->field_0088 = 0;
    self->field_008c = 0;
    self->field_0090 = 0;
    self->field_0094 = 0;
    StoreFloat(self->field_0098, g_ZeroVectorX);
    CSF_AT(float, self, 0x9C) = g_ZeroVectorY;
    CSF_AT(float, self, 0xA0) = g_ZeroVectorZ;
    StoreFloat(self->field_00a4, g_ZeroVector2X);
    StoreFloat(self->field_00a8, g_ZeroVector2Y);
    StoreFloat(self->field_00ac, g_ZeroVector2X);
    StoreFloat(self->field_00b0, g_ZeroVector2Y);
    StoreFloat(self->field_00b4, g_ZeroVector2X);
    StoreFloat(self->field_00b8, g_ZeroVector2Y);
    self->field_00bc = 0;
    self->field_00c0 = 0;
    self->field_00c4 = 0;
    self->field_00c8 = 0;
    self->field_00cc = 0;
    self->field_00d0 = 0;
    self->field_00d4 = 0;
    StoreFloat(self->field_00d8, g_ZeroVectorX);
    CSF_AT(float, self, 0xDC) = g_ZeroVectorY;
    CSF_AT(float, self, 0xE0) = g_ZeroVectorZ;
    StoreFloat(self->field_00e4, 100.0f);
    self->field_00e8 = 0;
    self->field_00ec = 0;
    StoreFloat(self->field_00f0, 6.2831855f);
    StoreFloat(self->field_00f4, g_ZeroVectorX);
    CSF_AT(float, self, 0xF8) = g_ZeroVectorY;
    CSF_AT(float, self, 0xFC) = g_ZeroVectorZ;
    self->field_0100 = 0;
    self->field_0104 = 0;
    self->field_0108 = 0;
    StoreFloat(self->field_010c, g_AxisZ);
    CSF_AT(float, self, 0x110) = g_AxisZ_Y;
    CSF_AT(float, self, 0x114) = g_AxisZ_Z;
    StoreFloat(self->field_0118, g_AxisY);
    CSF_AT(float, self, 0x11C) = g_AxisY_Y;
    CSF_AT(float, self, 0x120) = g_AxisY_Z;
    StoreFloat(self->field_0124, g_ZeroVectorX);
    CSF_AT(float, self, 0x128) = g_ZeroVectorY;
    CSF_AT(float, self, 0x12C) = g_ZeroVectorZ;
    StoreFloat(self->field_0130, g_ZeroVectorX);
    CSF_AT(float, self, 0x134) = g_ZeroVectorY;
    CSF_AT(float, self, 0x138) = g_ZeroVectorZ;
    StoreFloat(self->field_013c, g_ZeroVectorX);
    CSF_AT(float, self, 0x140) = g_ZeroVectorY;
    CSF_AT(float, self, 0x144) = g_ZeroVectorZ;
    CSF_AT(float, self, 0x148) = g_ZeroVectorX;
    CSF_AT(float, self, 0x14C) = g_ZeroVectorY;

    int result;
    std::memcpy(&result, &g_ZeroVectorZ, sizeof(result));
    CSF_AT(float, self, 0x150) = g_ZeroVectorZ;
    return result;
}

HOOK(0x006DD040, S_006dcf90_ResetDefaults, "auto");
