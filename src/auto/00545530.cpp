#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Base-class constructor for the embedded sub-object at +0x04 (ecx = this+4, __thiscall, no stack args).
CSF_FUNC(void*, __fastcall, MenuScreenSubObject_Construct, 0x712CC0, (void* self, void* edx));

// Constructor for the embedded sub-object at +0x5C (ecx = this+0x5C, __thiscall, no stack args).
CSF_FUNC(void*, __fastcall, SaveGameListSubObject_Construct, 0x5445F0, (void* self, void* edx));

// Returns a pointer (as int) to a global settings record; caller reads its +4 dword. __cdecl, no args.
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x506BD0, ());

// Vtable addresses referenced by the constructor (bound by address).
CSF_GLOBAL(uint8_t, g_VtblCMenuScreenBase, 0x83042C);
CSF_GLOBAL(uint8_t, g_VtblCMenuSaveGameSubObject, 0x843B70);
CSF_GLOBAL(uint8_t, g_VtblCMenuSaveGame, 0x843B30);

// Vtable pointer values stored by the constructor. The literal addresses are kept explicit
// so the stored vtable pointers are visibly the original's 0x83042C / 0x843B70 / 0x843B30.
static constexpr uint32_t kVtblCMenuScreenBaseAddr = 0x0083042Cu;
static constexpr uint32_t kVtblCMenuSaveGameSubObjectAddr = 0x00843B70u;
static constexpr uint32_t kVtblCMenuSaveGameAddr = 0x00843B30u;

// CMenuSaveGame constructor. Stack arg: owner/context value stored at +0x48. Callee pops 4 (ret 4).
static CMenuSaveGame* __fastcall CMenuSaveGame_Construct(CMenuSaveGame* self, void* /*edx*/, int ownerContext)
{
    // Install base-class vtable (0x83042C), then construct the embedded sub-object at +0x04.
    CSF_AT(uint32_t, self, 0) = kVtblCMenuScreenBaseAddr;
    MenuScreenSubObject_Construct(reinterpret_cast<uint8_t*>(self) + 0x04, nullptr);
    // Sub-object vtable (0x843B70) installed at +0x04.
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self) + 0x04, 0) = kVtblCMenuSaveGameSubObjectAddr;
    // Final CMenuSaveGame vtable (0x843B30).
    CSF_AT(uint32_t, self, 0) = kVtblCMenuSaveGameAddr;

    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;

    // Construct the embedded sub-object at +0x5C.
    SaveGameListSubObject_Construct(reinterpret_cast<uint8_t*>(self) + 0x5C, nullptr);

    self->field_0048 = static_cast<uint32_t>(ownerContext);
    self->field_0044 = 0;

    // Read the settings global; its +4 dword is stored at +0x58 after the remaining fields are set.
    void* globalRecord = reinterpret_cast<void*>(GetGlobalValue_8B056C());
    uint32_t globalValue = CSF_AT(uint32_t, globalRecord, 4);

    self->field_0040 = 0;
    self->field_004c = 0;
    self->field_001c = 0;
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_0050 = 0xFFFFFFFFu;
    self->field_0054 = 0xFFFFFFFFu;
    self->field_0058 = globalValue;

    return self;
}

HOOK(0x545530, CMenuSaveGame_Construct, "auto");
