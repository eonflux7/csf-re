#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(C_00840e68*, __fastcall, C_00840e68_Construct, 0x0051C0C0, (C_00840e68* self, void*));
CSF_FUNC(int, __fastcall, C_00840f6c_InitializeEffects, 0x0051ED10, (void* self, void* edx));

CSF_GLOBAL(const char*, g_psz_PFX_VAHO_SOLDADO, 0x008A8C8C);
CSF_GLOBAL(const char*, g_psz_PFX_VAHO_PLAYER, 0x008A8C90);

static int __fastcall C_00840f6c_Construct(C_00840f6c* self, void* /*edx*/)
{
    C_00840e68_Construct(self, nullptr);
    *(void***)self = CSF_VTBL(0x00840F6C);
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 10;
    CSF_AT(void*, self, 0x24) = CSF_VTBL(0x0082D448);
    self->field_0028 = -1;
    self->field_002c = -1;
    self->field_0020 = 0;
    self->field_0030 = -1;
    self->field_0034 = -1082130432;
    self->field_0038 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0048 = 10;

    void* registry = CSF_AT(void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);
    self->field_0008 =
        reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, registry, 0)[3])(
                registry, nullptr, g_psz_PFX_VAHO_SOLDADO);
    self->field_000c =
        reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, registry, 0)[3])(
                registry, nullptr, g_psz_PFX_VAHO_PLAYER);
    C_00840f6c_InitializeEffects(self, nullptr);
    return reinterpret_cast<int>(self);
}

HOOK(0x0051F0D0, C_00840f6c_Construct, "auto");
