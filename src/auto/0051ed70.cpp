#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, InitializeBreathEffects, 0x0051ED10, (void* self, void* edx));

CSF_GLOBAL(const char*, g_psz_PFX_VAHO_SOLDADO, 0x008A8C8C);
CSF_GLOBAL(const char*, g_psz_PFX_VAHO_PLAYER, 0x008A8C90);

static int __fastcall C_00840f6c_ResolveBreathEffects(
    C_00840f6c* self, void* /*edx*/)
{
    void* context = reinterpret_cast<void*>(GetValue8B0554());
    void* effectRegistry = CSF_AT(void*, context, 0x1FCC);

    self->field_0008 =
        reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, effectRegistry, 0)[0xC / 4])(
                effectRegistry, nullptr, g_psz_PFX_VAHO_SOLDADO);

    self->field_000c =
        reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, effectRegistry, 0)[0xC / 4])(
                effectRegistry, nullptr, g_psz_PFX_VAHO_PLAYER);

    return InitializeBreathEffects(self, nullptr);
}

HOOK(0x0051ED70, C_00840f6c_ResolveBreathEffects, "auto");
