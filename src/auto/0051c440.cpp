#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(const char*, g_psz_PFX_EXPLOSION_GRANADA_1, 0x008A8BDC);
CSF_GLOBAL(const char*, g_psz_PFX_ESTELA_PISTOLA, 0x008A8BF0);
CSF_GLOBAL(const char*, g_psz_PFX_HUMO, 0x008A8C18);
CSF_GLOBAL(const char*, g_psz_PFX_GAS, 0x008A8C1C);
CSF_GLOBAL(const char*, g_psz_PFX_COKTEL_MOLOTOV_1, 0x008A8C28);
CSF_GLOBAL(const char*, g_psz_PFX_EXPLOSION_VEHI_1, 0x008A8C30);
CSF_GLOBAL(const char*, g_psz_PFX_EXPLOSION_BOMBA_SABOTAGE, 0x008A8C50);
CSF_GLOBAL(const char*, g_psz_PFX_EXPLOSION_AGUA_1, 0x008A8C9C);
CSF_GLOBAL(const char*, g_psz_PFX_EXPLOSION_AGUA_UNDER_1, 0x008A8CA4);
CSF_GLOBAL(const char*, g_pszKey_VERSION_8A8CAC, 0x008A8CAC);

static int __cdecl FxParticleManager_FindEffect(void* registry, const char* name)
{
    return reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
        CSF_AT(void**, registry, 0)[0xC / 4])(registry, nullptr, name);
}

static void __cdecl FxParticleManager_AppendEffect(
    uint32_t& data, uint32_t& capacity, uint32_t& count,
    uint32_t& growth, int effect)
{
    if (count >= capacity)
    {
        const uint32_t increment = growth;
        void* replacement = Mem_Alloc(4 * (increment + capacity));
        const uint32_t oldCount = count;
        if (oldCount)
            std::memcpy(replacement, reinterpret_cast<const void*>(data),
                        4 * oldCount);

        const uint32_t oldData = data;
        capacity += increment;
        if (oldData)
        {
            Mem_Free(reinterpret_cast<void*>(oldData));
            data = 0;
        }
        data = reinterpret_cast<uint32_t>(replacement);
    }

    const uint32_t index = count;
    int* destination = reinterpret_cast<int*>(data + 4 * index);
    count = index + 1;
    *destination = effect;
}

static int __fastcall C_00840ec0_LoadParticleEffectIds(
    C_00840ec0* self, void* /*edx*/)
{
    // The packet also identifies this direct callee as a vtable reference.
    (void)CSF_VTBL(0x0041E5F0);

    void* registry = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);

    self->field_0010 = 0;
    const char** name = &g_psz_PFX_EXPLOSION_GRANADA_1;
    do
    {
        const int effect = FxParticleManager_FindEffect(registry, *name);
        if (effect != -1)
        {
            if (self->field_0010 >= self->field_000c)
            {
                S_0041e5f0_GrowArrayCapacity(
                    reinterpret_cast<S_0041e5f0*>(
                        reinterpret_cast<uint8_t*>(self) + 0x08),
                    nullptr, static_cast<int>(self->field_0014));
            }

            const uint32_t index = self->field_0010;
            int* destination =
                reinterpret_cast<int*>(self->field_0008 + 4 * index);
            self->field_0010 = index + 1;
            *destination = effect;
        }
        ++name;
    }
    while (reinterpret_cast<intptr_t>(name) <
           reinterpret_cast<intptr_t>(&g_psz_PFX_ESTELA_PISTOLA));

    self->field_00a0 = 0;
    name = &g_psz_PFX_EXPLOSION_AGUA_1;
    do
    {
        const int effect = FxParticleManager_FindEffect(registry, *name);
        if (effect != -1)
            FxParticleManager_AppendEffect(
                self->field_0098, self->field_009c,
                self->field_00a0, self->field_00a4, effect);
        ++name;
    }
    while (reinterpret_cast<intptr_t>(name) <
           reinterpret_cast<intptr_t>(&g_psz_PFX_EXPLOSION_AGUA_UNDER_1));

    self->field_00b0 = 0;
    name = &g_psz_PFX_EXPLOSION_AGUA_UNDER_1;
    do
    {
        const int effect = FxParticleManager_FindEffect(registry, *name);
        if (effect != -1)
            FxParticleManager_AppendEffect(
                self->field_00a8, self->field_00ac,
                self->field_00b0, self->field_00b4, effect);
        ++name;
    }
    while (reinterpret_cast<intptr_t>(name) <
           reinterpret_cast<intptr_t>(&g_pszKey_VERSION_8A8CAC));

    self->field_0040 = 0;
    int effect = FxParticleManager_FindEffect(registry, g_psz_PFX_GAS);
    if (effect != -1)
        FxParticleManager_AppendEffect(
            self->field_0038, self->field_003c,
            self->field_0040, self->field_0044, effect);

    self->field_0050 = 0;
    effect = FxParticleManager_FindEffect(registry, g_psz_PFX_HUMO);
    if (effect != -1)
        FxParticleManager_AppendEffect(
            self->field_0048, self->field_004c,
            self->field_0050, self->field_0054, effect);

    self->field_0060 = 0;
    name = &g_psz_PFX_COKTEL_MOLOTOV_1;
    do
    {
        effect = FxParticleManager_FindEffect(registry, *name);
        if (effect != -1)
            FxParticleManager_AppendEffect(
                self->field_0058, self->field_005c,
                self->field_0060, self->field_0064, effect);
        ++name;
    }
    while (reinterpret_cast<intptr_t>(name) <
           reinterpret_cast<intptr_t>(&g_psz_PFX_EXPLOSION_VEHI_1));

    self->field_0080 = 0;
    name = &g_psz_PFX_EXPLOSION_VEHI_1;
    do
    {
        effect = FxParticleManager_FindEffect(registry, *name);
        if (effect != -1)
            FxParticleManager_AppendEffect(
                self->field_0078, self->field_007c,
                self->field_0080, self->field_0084, effect);
        ++name;
    }
    while (reinterpret_cast<intptr_t>(name) <
           reinterpret_cast<intptr_t>(&g_psz_PFX_EXPLOSION_BOMBA_SABOTAGE));

    self->field_0090 = 0;
    effect = FxParticleManager_FindEffect(
        registry, g_psz_PFX_EXPLOSION_BOMBA_SABOTAGE);
    if (effect != -1)
        FxParticleManager_AppendEffect(
            self->field_0088, self->field_008c,
            self->field_0090, self->field_0094, effect);

    if (self->field_0010 > self->field_001c)
    {
        void* replacement = Mem_Alloc(4 * self->field_000c);
        const uint32_t oldData = self->field_0018;
        self->field_001c = self->field_000c;
        if (oldData)
        {
            Mem_Free(reinterpret_cast<void*>(oldData));
            self->field_0018 = 0;
        }
        self->field_0018 = reinterpret_cast<uint32_t>(replacement);
    }
    uint32_t count = self->field_0010;
    if (count)
        std::memcpy(reinterpret_cast<void*>(self->field_0018),
                    reinterpret_cast<const void*>(self->field_0008), 4 * count);
    self->field_0020 = self->field_0010;
    self->field_0024 = self->field_0014;

    if (self->field_0010 > self->field_002c)
    {
        void* replacement = Mem_Alloc(4 * self->field_000c);
        const uint32_t oldData = self->field_0028;
        self->field_002c = self->field_000c;
        if (oldData)
        {
            Mem_Free(reinterpret_cast<void*>(oldData));
            self->field_0028 = 0;
        }
        self->field_0028 = reinterpret_cast<uint32_t>(replacement);
    }
    count = self->field_0010;
    if (count)
        std::memcpy(reinterpret_cast<void*>(self->field_0028),
                    reinterpret_cast<const void*>(self->field_0008), 4 * count);
    self->field_0030 = self->field_0010;
    int result = static_cast<int>(self->field_0014);
    self->field_0034 = static_cast<uint32_t>(result);

    if (self->field_0010 > self->field_006c)
    {
        void* replacement = Mem_Alloc(4 * self->field_000c);
        self->field_006c = self->field_000c;
        result = static_cast<int>(self->field_0068);
        if (result)
        {
            result = Mem_Free(reinterpret_cast<void*>(self->field_0068));
            self->field_0068 = 0;
        }
        self->field_0068 = reinterpret_cast<uint32_t>(replacement);
    }
    count = self->field_0010;
    if (count)
        std::memcpy(reinterpret_cast<void*>(self->field_0068),
                    reinterpret_cast<const void*>(self->field_0008), 4 * count);
    self->field_0070 = self->field_0010;
    self->field_0074 = self->field_0014;
    return result;
}

HOOK(0x0051C440, C_00840ec0_LoadParticleEffectIds, "auto");
