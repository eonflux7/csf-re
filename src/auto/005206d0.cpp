#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, FxParticleManager_GrowResolvedEntries, 0x0051FB70, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(int, __fastcall, GfxMatrix_CopyTranslationAndUpdate, 0x006AD9E0, (void* self, void* , const uint32_t* src));
CSF_FUNC(int, __fastcall, S_006ada10_SetRotation, 0x006AE2F0, (S_006ada10* self, void* , float angleX, float angleY, float angleZ));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, InputBindings_FindEntryIndexByKey, 0x007686E0, (void* self, void* , int key));
CSF_FUNC(int, __fastcall, C_0085e7b8_GetSlotEntry, 0x00768740, (C_0085e7b8* self, void* , int slotIndex));

static int __fastcall S_005206d0_ResolveUnindexedEntry(
    S_005206d0* self, void* /*edx*/, int arg0)
{
    auto* entry = reinterpret_cast<std::uint8_t*>(arg0);

    auto* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* definitions = CSF_AT(void*, manager, 0x1FCC);
    const int definitionIndex =
        ((int(__fastcall*)(void*, void*, int))
            CSF_AT(void**, definitions, 0)[0x10 / 4])(
                definitions, nullptr, CSF_AT(int, entry, 0x20));

    manager = reinterpret_cast<void*>(GetValue8B0554());
    const int slotIndex = InputBindings_FindEntryIndexByKey(
        CSF_AT(void*, manager, 0x1F90), nullptr,
        CSF_AT(int, entry, 0x24));

    manager = reinterpret_cast<void*>(GetValue8B0554());
    definitions = CSF_AT(void*, manager, 0x1FCC);
    auto* definition =
        ((std::uint8_t*(__fastcall*)(void*, void*, int))
            CSF_AT(void**, definitions, 0)[0x08 / 4])(
                definitions, nullptr, definitionIndex);

    manager = reinterpret_cast<void*>(GetValue8B0554());
    auto* slot = reinterpret_cast<std::uint8_t*>(
        C_0085e7b8_GetSlotEntry(
            CSF_AT(C_0085e7b8*, manager, 0x1F90), nullptr, slotIndex));

    void* sceneLoader = reinterpret_cast<void*>(GetGlobalInt());
    void* registry = reinterpret_cast<void*>(
        SceneLoader_GetObjectRegistry(sceneLoader, nullptr));
    void* object =
        ((void*(__fastcall*)(void*, void*, const void*))
            CSF_AT(void**, registry, 0)[0x14 / 4])(
                registry, nullptr, definition + 4);

    if (object)
    {
        alignas(8) std::uint8_t matrixBytes[0x10] = {};
        auto& matrix = *reinterpret_cast<S_006ada10*>(matrixBytes);
        matrix.field_0000 =
            reinterpret_cast<decltype(matrix.field_0000)>(RwMatrixCreate());

        const float angleY = CSF_AT(float, slot, 0x14);
        const float angleX = CSF_AT(float, slot, 0x10);
        CSF_AT(std::uint8_t, matrixBytes, 0x04) = 1;
        S_006ada10_SetRotation(&matrix, nullptr, angleX, angleY, 0.0f);
        GfxMatrix_CopyTranslationAndUpdate(
            &matrix, nullptr, reinterpret_cast<const uint32_t*>(slot + 4));

        ((void(__fastcall*)(void*, void*, S_006ada10*))
            CSF_AT(void**, object, 0)[0x40 / 4])(
                object, nullptr, &matrix);

        const int parameter = CSF_AT(int, entry, 0x28);
        if (parameter != -1)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[0x50 / 4])(
                    object, nullptr, parameter);
        }

        if (CSF_AT(std::uint8_t, entry, 0x31) ||
            ((bool(__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0)[0x60 / 4])(object, nullptr))
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, object, 0)[0x28 / 4])(object, nullptr, 0);
        }

        const int entryId = CSF_AT(int, entry, 0x1C);
        const std::uint32_t entryFlag = CSF_AT(std::uint8_t, entry, 0x32);

        if (CSF_AT(std::uint32_t, self, 0x34) >=
            CSF_AT(std::uint32_t, self, 0x30))
        {
            FxParticleManager_GrowResolvedEntries(
                reinterpret_cast<std::uint8_t*>(self) + 0x2C,
                nullptr, CSF_AT(int, self, 0x38));
        }

        const std::uint32_t count = CSF_AT(std::uint32_t, self, 0x34);
        auto* resolvedEntry =
            CSF_AT(std::uint8_t*, self, 0x2C) + 20 * count;
        CSF_AT(std::uint32_t, self, 0x34) = count + 1;
        CSF_AT(int, resolvedEntry, 0x00) = entryId;
        CSF_AT(int, resolvedEntry, 0x04) = slotIndex;
        CSF_AT(void*, resolvedEntry, 0x08) = object;
        CSF_AT(int, resolvedEntry, 0x0C) = definitionIndex;
        CSF_AT(std::uint32_t, resolvedEntry, 0x10) = entryFlag;

        if (CSF_AT(std::uint8_t, matrixBytes, 0x04))
            RwMatrixDestroy(
                reinterpret_cast<std::uint32_t*>(matrix.field_0000));
    }

    return reinterpret_cast<int>(object);
}

HOOK(0x005206D0, S_005206d0_ResolveUnindexedEntry, "auto");
