#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* edx, int additionalCapacity));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

CSF_GLOBAL(const char*, g_psz_PFX_IMPACTO_BALA, 0x008A8BD0);
CSF_GLOBAL(char*, g_psz_PFX_IMPACTO_BALA_HUMANO_1, 0x008A8BD4);
CSF_GLOBAL(const char*, g_psz_PFX_ROTURA_CRISTAL, 0x008A8C84);
CSF_GLOBAL(const char*, g_psz_PFX_HUMO, 0x008A8C18);

// Calls vtable slot 3 (byte offset 0xC) of the effect registry: look up a particle effect id by name.
static int LookupParticleEffect(int registry, const char* name)
{
    void** vtbl = CSF_AT(void**, registry, 0);
    return ((int(__fastcall*)(void*, void*, const char*))vtbl[3])(
        reinterpret_cast<void*>(registry), nullptr, name);
}

static char** __fastcall C_00840ee0_InitializeParticleStorage(C_00840ee0* self, void* /*edx*/)
{
    int registry = *reinterpret_cast<int*>(GetValue8B0554() + 8140);

    self->field_0008 = LookupParticleEffect(registry, g_psz_PFX_IMPACTO_BALA);
    self->field_001c = LookupParticleEffect(registry, g_psz_PFX_ROTURA_CRISTAL);
    self->field_0020 = LookupParticleEffect(registry, g_psz_PFX_HUMO);
    self->field_0014 = 0;

    char** cursor = &g_psz_PFX_IMPACTO_BALA_HUMANO_1;
    do {
        int effectId = LookupParticleEffect(registry, *cursor);
        if (effectId != -1) {
            uint32_t count = self->field_0014;
            uint32_t capacity = self->field_0010;
            S_0041e5f0* array = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0xC);
            if (count >= capacity) {
                S_0041e5f0_GrowArrayCapacity(array, nullptr, (int)array->field_000c);
            }
            uint32_t index = array->field_0008;
            int* slot = reinterpret_cast<int*>(array->field_0000 + 4 * index);
            array->field_0008 = index + 1;
            *slot = effectId;
        }
        ++cursor;
    } while ((int)cursor < (int)CSF_ADDR_DATA(0x008A8BDC));

    return cursor;
}

HOOK(0x0051D0C0, C_00840ee0_InitializeParticleStorage, "auto");
