#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(uint32_t*, __fastcall, CProfileManager_InitBaseSubobject, 0x00516D00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CProfileManager_InitSlotTable, 0x00514F80, (void* self, void* edx));
CSF_FUNC(char, __fastcall, CProfileManager_ScanSaveGames, 0x00515500, (void* self, void* edx));

// CProfileManager::CProfileManager() body: base init, vtable, default field values,
// then slot table setup and save-game scan. Returns this (eax = esi).
static int __fastcall CProfileManager_InitFields(CProfileManager* self, void* /*edx*/)
{
    CProfileManager_InitBaseSubobject(self, nullptr);

    self->vtbl = CSF_VTBL(0x008409F0);
    self->field_002c = 15;
    self->field_0028 = 0;
    CSF_AT(uint8_t, self, 0x18) = 0;
    self->field_0044 = 0;
    self->field_0048 = 7;
    CSF_AT(uint16_t, self, 0x34) = 0;
    self->field_0064 = 7;
    self->field_0060 = 0;
    CSF_AT(uint16_t, self, 0x50) = 0;
    self->field_0080 = 7;
    self->field_007c = 0;
    CSF_AT(uint16_t, self, 0x6C) = 0;
    self->field_009c = 7;
    self->field_0098 = 0;
    CSF_AT(uint16_t, self, 0x88) = 0;

    CProfileManager_InitSlotTable(self, nullptr);
    CProfileManager_ScanSaveGames(self, nullptr);

    return static_cast<int>(reinterpret_cast<intptr_t>(self));
}

HOOK(0x00515670, CProfileManager_InitFields, "auto");
