#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, SceneObjectOptions_ReleaseAndResetStorage, 0x004018E0, (void* self, void*));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(unsigned int, __fastcall, WeaponFirearms_DecodeColor, 0x00476430, (float* self, void* , unsigned int color));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void, __cdecl, SceneLoader_NoOp, 0x00506AC0, ());
CSF_FUNC(std::uint32_t*, __fastcall, FxParticleManager_InitializeEntry, 0x0051F670, (std::uint32_t* self, void*));
CSF_FUNC(std::uint32_t*, __fastcall, ParticleManager_InitializeEmptyWord, 0x0051F690, (std::uint32_t* self, void*));
CSF_FUNC(char, __fastcall, FxParticleManager_CopyFourWords, 0x0051F6A0, (void* self, void* , const void* source));
CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, CoreMemory_Free, 0x00691110, (void* block));
CSF_FUNC(void, __fastcall, ForwardToChildIfUnhandled, 0x006C8640, (void* self, void* , int a2));
CSF_FUNC(C_00860908*, __fastcall, C_00860908_InitDefaults, 0x006C8670, (C_00860908* self, void*));
CSF_FUNC(int, __fastcall, InitializeSceneObjectResources, 0x006C89E0, (void* self, void* , void* definition));

CSF_GLOBAL(std::uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(const char*, g_psz_PFX_CASQUILLOS, 0x008A8C94);

static int __fastcall C_008316d8_PrepareWeaponState(
    C_008316d8* self, void* /*edx*/)
{
    self->vSlotBC();

    auto* definition = reinterpret_cast<std::uint8_t*>(self->field_0040);
    self->field_0030 = 0;
    if (CSF_AT(std::uint32_t, definition, 0x74) == 33)
    {
        auto* allocation = reinterpret_cast<std::uint32_t*>(
            CoreMemory_AllocUntagged(16));
        auto* particleState = allocation
            ? ParticleManager_InitializeEmptyWord(allocation, nullptr)
            : nullptr;
        self->field_0030 = reinterpret_cast<std::uint32_t>(particleState);

        alignas(8) std::uint8_t entryBytes[0x10]{};
        auto* entry = reinterpret_cast<std::uint32_t*>(entryBytes);
        FxParticleManager_InitializeEntry(entry, nullptr);

        const int lookupValue = self->vResolveEntryLookupValue();
        void* relatedObject = reinterpret_cast<void*>(self->field_002c);
        CSF_AT(int, entryBytes, 0x00) = lookupValue;
        CSF_AT(int, entryBytes, 0x04) = relatedObject
            ? reinterpret_cast<int(__fastcall*)(void*, void*)>(
                  CSF_AT(void**, relatedObject, 0)[0x10 / 4])(
                  relatedObject, nullptr)
            : -1;

        definition = reinterpret_cast<std::uint8_t*>(self->field_0040);
        CSF_AT(std::uint32_t, entryBytes, 0x08) =
            CSF_AT(std::uint32_t, definition, 0xE8);
        CSF_AT(std::uint32_t, entryBytes, 0x0C) =
            CSF_AT(std::uint32_t, definition, 0xEC);

        if (!FxParticleManager_CopyFourWords(
                reinterpret_cast<void*>(self->field_0030),
                nullptr, entryBytes))
        {
            void* failedState = reinterpret_cast<void*>(self->field_0030);
            if (failedState)
            {
                SceneLoader_NoOp();
                CoreMemory_Free(failedState);
                self->field_0030 = 0;
            }
            self->field_0030 = 0;
        }
    }

    auto* resourceName =
        reinterpret_cast<std::uint8_t*>(self->field_0040) + 0xA4;
    self->field_0028 = 0;
    if (CompareStringSubstring(
            resourceName, nullptr, 0,
            CSF_AT(std::uint32_t, resourceName, 0x14),
            reinterpret_cast<const char*>(&g_byte_82E65C), 0))
    {
        auto* allocation = reinterpret_cast<C_00860908*>(
            CoreMemory_AllocUntagged(52));
        auto* sceneObject = allocation
            ? C_00860908_InitDefaults(allocation, nullptr)
            : nullptr;
        self->field_0028 = reinterpret_cast<std::uint32_t>(sceneObject);

        // Header, two embedded strings, and color/options occupy one object.
        alignas(8) std::uint8_t optionsBytes[0x5C]{};
        auto& primaryName =
            *reinterpret_cast<C_0082d410*>(optionsBytes + 0x04);
        auto& secondaryName =
            *reinterpret_cast<C_0082d410*>(optionsBytes + 0x20);

        CSF_AT(std::uint32_t, optionsBytes, 0x1C) = 15;
        CSF_AT(std::uint32_t, optionsBytes, 0x38) = 15;
        definition = reinterpret_cast<std::uint8_t*>(self->field_0040);

        CSF_AT(int, optionsBytes, 0x00) = 0;
        CSF_AT(std::uint32_t, optionsBytes, 0x18) = 0;
        *reinterpret_cast<std::uint8_t*>(&primaryName.field_0004) = 0;
        CSF_AT(std::uint32_t, optionsBytes, 0x34) = 0;
        *reinterpret_cast<std::uint8_t*>(&secondaryName.field_0004) = 0;
        CSF_AT(float, optionsBytes, 0x3C) = 0.0f;
        CSF_AT(float, optionsBytes, 0x40) = 0.0f;
        CSF_AT(float, optionsBytes, 0x44) = 0.0f;
        CSF_AT(float, optionsBytes, 0x48) = 1.0f;
        CSF_AT(std::uint32_t, optionsBytes, 0x4C) = 0;
        CSF_AT(float, optionsBytes, 0x58) = 15.0f;
        CSF_AT(float, optionsBytes, 0x54) = 15.0f;
        CSF_AT(std::uint8_t, optionsBytes, 0x50) = 0;

        CSF_AT(std::uint32_t, optionsBytes, 0x00) =
            CSF_AT(std::uint32_t, definition, 0xC4);
        AssignStringSubstring(
            &primaryName, nullptr, definition + 0x88, 0,
            static_cast<std::uint32_t>(-1));
        AssignStringSubstring(
            &secondaryName, nullptr,
            reinterpret_cast<std::uint8_t*>(self->field_0040) + 0xA4,
            0, static_cast<std::uint32_t>(-1));

        definition = reinterpret_cast<std::uint8_t*>(self->field_0040);
        CSF_AT(std::uint32_t, optionsBytes, 0x4C) =
            CSF_AT(std::uint32_t, definition, 0xC8);
        WeaponFirearms_DecodeColor(
            reinterpret_cast<float*>(optionsBytes + 0x3C), nullptr,
            CSF_AT(std::uint32_t, definition, 0xC0));
        CSF_AT(std::uint32_t, optionsBytes, 0x54) =
            CSF_AT(std::uint32_t, definition, 0xCC);
        CSF_AT(std::uint32_t, optionsBytes, 0x58) =
            CSF_AT(std::uint32_t, definition, 0xD0);

        InitializeSceneObjectResources(
            reinterpret_cast<void*>(self->field_0028),
            nullptr, optionsBytes);

        auto* owner = reinterpret_cast<C_00833908*>(self->field_0008);
        if (owner)
        {
            const int attachmentIndex =
                reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                    owner->vtbl[0x98 / 4])(owner, nullptr, 100);
            if (attachmentIndex != -1)
            {
                owner = reinterpret_cast<C_00833908*>(self->field_0008);
                const int childAddress =
                    reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                        owner->vtbl[0x8C / 4])(
                        owner, nullptr, attachmentIndex);
                void* child = reinterpret_cast<void*>(childAddress);
                const int attachmentValue =
                    reinterpret_cast<int(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, child, 0)[0x18 / 4])(
                        child, nullptr);
                ForwardToChildIfUnhandled(
                    reinterpret_cast<void*>(self->field_0028),
                    nullptr, attachmentValue);
            }
        }

        SceneObjectOptions_ReleaseAndResetStorage(optionsBytes, nullptr);
    }

    void* manager = reinterpret_cast<void*>(GetValue8B0554());
    void* effectRegistry = CSF_AT(void*, manager, 0x1FCC);
    const int effectId =
        reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, effectRegistry, 0)[0x0C / 4])(
            effectRegistry, nullptr, g_psz_PFX_CASQUILLOS);
    self->field_0034 = effectId;
    return effectId;
}

HOOK(0x00477F90, C_008316d8_PrepareWeaponState, "auto");
