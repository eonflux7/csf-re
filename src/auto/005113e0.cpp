#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original calls sub_510860(this, a2, a3, String1) through the __thiscall convention.
CSF_FUNC(uint32_t*, __fastcall, CMenuManager_InitBase, 0x00510860,
         (void* self, void* edx, int a2, int a3, char* String1));

// GetDoubleClickTime is reached through an import-address-table slot at 0x82D2E8 (stdcall).
using GetDoubleClickTimeFn = uint32_t(__stdcall*)();
CSF_GLOBAL(GetDoubleClickTimeFn, g_GetDoubleClickTimeImport, 0x0082D2E8);

// Constructor for C_008407a8 (menu manager with double-click timing).
// Original: ret 12 (three stack arguments a2, a3, String1).
static uint32_t* __fastcall C_008407a8_Construct(C_008407a8* self, void* /*edx*/,
                                                 int a2, int a3, char* String1)
{
    CMenuManager_InitBase(self, nullptr, a2, a3, String1);

    self->vtbl = CSF_VTBL(0x008407A8);

    // Store the system double-click interval at a2 + 0xCA0.
    const uint32_t doubleClickTime = g_GetDoubleClickTimeImport();
    CSF_AT(uint32_t, reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), 0xCA0) = doubleClickTime;

    self->field_013c = -1.0f;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x005113E0, C_008407a8_Construct, "auto");
