#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, CMenuScreen_ConstructEmbeddedBase,
         0x712CC0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, MenuScenarioSelect_InitializeStorage,
         0x440150, (void* self, void* edx, unsigned int count));
CSF_FUNC(int, __fastcall, CMenuScenarioSelect_ResetSelectionState,
         0x52DD20, (int* self, int edx));

CSF_GLOBAL(uint8_t, g_VtblCMenuScreenBase, 0x83042C);
CSF_GLOBAL(uint8_t, g_VtblCMenuScenarioSelectMain, 0x841688);
CSF_GLOBAL(uint8_t, g_VtblCMenuScenarioSelectSecondary, 0x841638);

static CMenuScenarioSelect* __fastcall CMenuScenarioSelect_Construct(
    CMenuScenarioSelect* self, void* /*edx*/, int context)
{
    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(0x83042C);
    CMenuScreen_ConstructEmbeddedBase(
        reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    self->vtbl = reinterpret_cast<decltype(self->vtbl)>(0x841688);
    self->field_0004 = 0x841638;

    self->field_0030 = 0;
    self->field_0034 = 15;
    *reinterpret_cast<uint8_t*>(&self->field_0020) = 0;

    CSF_AT(uint32_t, self, 0x48) = 0;
    CSF_AT(uint32_t, self, 0x4C) = 0;
    CSF_AT(uint32_t, self, 0x50) = 0;

    self->field_0090 = 15;
    self->field_008c = 0;
    *reinterpret_cast<uint8_t*>(&self->field_007c) = 0;
    self->field_0070 = static_cast<uint32_t>(context);
    self->field_0018 = 0;

    MenuScenarioSelect_InitializeStorage(
        reinterpret_cast<uint8_t*>(self) + 0x44, nullptr, 0x14u);
    CMenuScenarioSelect_ResetSelectionState(reinterpret_cast<int*>(self), 0);
    return self;
}

HOOK(0x0052E3B0, CMenuScenarioSelect_Construct, "auto");
