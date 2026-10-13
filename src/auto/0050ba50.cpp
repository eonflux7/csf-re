#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, OptionsFile_ConstructControlSettingsBase, 0x0050A480, (void* self, void* edx));
CSF_FUNC(int, __fastcall, OptionsFile_InitializeSettingsBlock, 0x0050AA20, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, OptionsFile_ConstructMainSettings, 0x0050B220, (void* self, void* edx));
CSF_FUNC(char, __fastcall, OptionsFile_FinalizeConstruction, 0x0050B710, (void* self, void* edx));
CSF_FUNC(int, __fastcall, OptionsFile_InitializeDefaults, 0x0050B9A0, (void* self, void* edx));
CSF_FUNC(uint8_t*, __fastcall, OptionsFile_ConstructBase, 0x0050BB50, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static int __fastcall COptionsFile_Construct(COptionsFile* self, void* /*edx*/)
{
    OptionsFile_ConstructBase(self, nullptr);
    self->vtbl = CSF_VTBL(0x0083FFA8);

    self->field_0030 = 7;
    self->field_002c = 0;
    *reinterpret_cast<std::uint16_t*>(&self->field_001c) = 0;
    self->field_004c = 7;
    self->field_0048 = 0;
    *reinterpret_cast<std::uint16_t*>(&self->field_0038) = 0;
    OptionsFile_InitializeDefaults(self, nullptr);

    void* mainSettingsStorage = Mem_Alloc(644);
    uint32_t* mainSettings = mainSettingsStorage
        ? OptionsFile_ConstructMainSettings(mainSettingsStorage, nullptr)
        : nullptr;
    self->field_0004 = reinterpret_cast<std::uint32_t>(mainSettings);

    void* volumeSettings = Mem_Alloc(20);
    if (volumeSettings)
    {
        CSF_AT(void**, volumeSettings, 0x00) = CSF_VTBL(0x0083FED8);
        CSF_AT(float, volumeSettings, 0x04) = 1.0f;
        CSF_AT(float, volumeSettings, 0x08) = 1.0f;
        CSF_AT(std::uint32_t, volumeSettings, 0x0C) = 1;
        CSF_AT(std::uint8_t, volumeSettings, 0x10) = 0;
        CSF_AT(std::uint8_t, volumeSettings, 0x11) = 0;
    }
    self->field_0008 = reinterpret_cast<std::uint32_t>(volumeSettings);

    auto* settingsBlock = static_cast<S_0050aa20*>(Mem_Alloc(52));
    if (settingsBlock)
    {
        settingsBlock->field_0004 = 0;
        settingsBlock->field_0008 = 0;
        CSF_AT(void**, settingsBlock, 0x00) = CSF_VTBL(0x0083FF18);
        OptionsFile_InitializeSettingsBlock(settingsBlock, nullptr);
    }
    self->field_000c = reinterpret_cast<std::uint32_t>(settingsBlock);

    void* controlSettings = Mem_Alloc(28);
    if (controlSettings)
    {
        OptionsFile_ConstructControlSettingsBase(controlSettings, nullptr);
        CSF_AT(void**, controlSettings, 0x00) = CSF_VTBL(0x0083FE30);
        CSF_AT(float, controlSettings, 0x0C) = 0.35f;
        CSF_AT(float, controlSettings, 0x08) = 0.35f;
    }
    self->field_0010 = reinterpret_cast<std::uint32_t>(controlSettings);

    OptionsFile_FinalizeConstruction(self, nullptr);
    return static_cast<int>(reinterpret_cast<std::uintptr_t>(self));
}

HOOK(0x0050BA50, COptionsFile_Construct, "auto");
