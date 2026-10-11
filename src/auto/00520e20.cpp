#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(std::uint32_t*, __fastcall, S_00520ae0_AppendParticle, 0x00520D80, (S_00520ae0* self, void* , std::uint32_t* source, int growth));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));

static int __fastcall FxParticleManager_ProcessEntry(
    char* self, void* /*edx*/, void* source, int entryId, int particleId)
{
    if (!source)
        return 0;

    const int sourceIndex =
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, source, 0)[0x98 / 4])(source, nullptr, entryId);

    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* definitions = CSF_AT(void*, manager, 0x1FCC);
    const int definitionIndex =
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, definitions, 0)[0x10 / 4])(
                definitions, nullptr, particleId);

    if (sourceIndex == -1 || definitionIndex == -1)
        return 0;

    manager = reinterpret_cast<void*>(GetValue8B0554());
    definitions = CSF_AT(void*, manager, 0x1FCC);
    void* definition =
        ((void*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, definitions, 0)[0x08 / 4])(
                definitions, nullptr, definitionIndex);

    void* sourceEntry =
        ((void*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, source, 0)[0x8C / 4])(
                source, nullptr, sourceIndex);

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(sceneLoader, nullptr));

    void* particle =
        ((void*(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, registry, 0)[0x14 / 4])(
                registry, nullptr,
                reinterpret_cast<std::uint8_t*>(definition) + 4);

    if (!particle)
        return 0;

    // The original captures this vtable before querying the source entry.
    void** particleVtable = CSF_AT(void**, particle, 0);
    const int sourceValue =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, sourceEntry, 0)[0x18 / 4])(
                sourceEntry, nullptr);

    ((void(__fastcall*)(void*, void*, int))
        particleVtable[0x44 / 4])(particle, nullptr, sourceValue);

    if (((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, particle, 0)[0x60 / 4])(particle, nullptr))
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, particle, 0)[0x28 / 4])(particle, nullptr, 0);
    }

    alignas(8) std::uint8_t entryBytes[0x28]{};
    CSF_AT(void*, entryBytes, 0x00) = source;
    CSF_AT(void**, entryBytes, 0x04) = CSF_VTBL(0x0082D448);
    CSF_AT(int, entryBytes, 0x08) = -1;
    CSF_AT(int, entryBytes, 0x0C) = -1;
    CSF_AT(int, entryBytes, 0x10) = entryId;
    CSF_AT(void*, entryBytes, 0x14) = particle;
    CSF_AT(int, entryBytes, 0x18) = 0;
    CSF_AT(int, entryBytes, 0x1C) = definitionIndex;
    CSF_AT(int, entryBytes, 0x20) = -1;

    S_00520ae0_AppendParticle(
        reinterpret_cast<S_00520ae0*>(self + 0x5C),
        nullptr, reinterpret_cast<std::uint32_t*>(entryBytes), 0);

    return static_cast<int>(reinterpret_cast<std::uintptr_t>(particle));
}

HOOK(0x00520E20, FxParticleManager_ProcessEntry, "auto");
