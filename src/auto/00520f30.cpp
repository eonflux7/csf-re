#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(std::uint32_t*, __fastcall, S_00520ae0_AppendParticle, 0x00520D80, (S_00520ae0* self, void* , std::uint32_t* source, int growth));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall FxParticleManager_ApplyResolvedBinding(
    char* self, void* /*edx*/, int handle, int bindingKey, int particleKey)
{
    void* owner = reinterpret_cast<void*>(
        CHandleTable_Resolve(g_pCurrentScene, nullptr, handle));
    if (!owner)
        return 0;

    void* bindings =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);

    int bindingIndex =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, bindings, 0)[0x98 / 4])(
                bindings, nullptr, bindingKey);

    void* particleCatalog = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);
    int particleIndex =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, particleCatalog, 0)[0x10 / 4])(
                particleCatalog, nullptr, particleKey);

    if (bindingIndex == -1 || particleIndex == -1)
        return 0;

    particleCatalog = CSF_AT(
        void*, reinterpret_cast<void*>(GetValue8B0554()), 0x1FCC);
    void* particleDefinition =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, particleCatalog, 0)[0x08 / 4])(
                particleCatalog, nullptr, particleIndex);

    bindings =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    void* binding =
        reinterpret_cast<void* (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, bindings, 0)[0x8C / 4])(
                bindings, nullptr, bindingIndex);

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
    void* particle =
        reinterpret_cast<void* (__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, registry, 0)[0x14 / 4])(
                registry, nullptr,
                reinterpret_cast<std::uint8_t*>(particleDefinition) + 4);

    if (particle)
    {
        void** particleVtable = CSF_AT(void**, particle, 0);
        int bindingValue =
            reinterpret_cast<int (__fastcall*)(void*, void*)>(
                CSF_AT(void**, binding, 0)[0x18 / 4])(binding, nullptr);

        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            particleVtable[0x44 / 4])(particle, nullptr, bindingValue);

        bool needsReset =
            reinterpret_cast<bool (__fastcall*)(void*, void*)>(
                CSF_AT(void**, particle, 0)[0x60 / 4])(particle, nullptr);
        if (needsReset)
        {
            reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, particle, 0)[0x28 / 4])(particle, nullptr, 0);
        }

        const std::uint32_t handlePart1 =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(handle), 0x04);
        const std::uint32_t handlePart2 =
            CSF_AT(std::uint32_t, reinterpret_cast<void*>(handle), 0x08);

        alignas(8) std::uint8_t bindingBytes[0x28];
        CSF_AT(std::uint32_t, bindingBytes, 0x08) = handlePart1;
        CSF_AT(void**, bindingBytes, 0x04) = CSF_VTBL(0x0082D448);
        CSF_AT(std::uint32_t, bindingBytes, 0x00) = 0;
        CSF_AT(std::uint32_t, bindingBytes, 0x18) = 0;
        CSF_AT(int, bindingBytes, 0x20) = -1;
        CSF_AT(std::uint32_t, bindingBytes, 0x0C) = handlePart2;
        CSF_AT(void*, bindingBytes, 0x14) = particle;
        CSF_AT(int, bindingBytes, 0x1C) = particleIndex;
        CSF_AT(int, bindingBytes, 0x10) = bindingKey;

        S_00520ae0_AppendParticle(
            reinterpret_cast<S_00520ae0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x5C),
            nullptr, reinterpret_cast<std::uint32_t*>(bindingBytes), 0);
    }

    return reinterpret_cast<int>(particle);
}

HOOK(0x00520F30, FxParticleManager_ApplyResolvedBinding, "auto");
