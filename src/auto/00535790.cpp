#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Vtable addresses stored as immediate values, matching the original's
// `mov dword ptr [reg], imm32` stores.
static const uint32_t kCMenuScreenPrimaryVtable = CSF_ADDR_VT(0x0083042C);   // off_83042C
static const uint32_t kCMenuScreenSubobjectVtable = CSF_ADDR_VT(0x00841E90); // off_841E90
static const uint32_t kCMenuIntroVideoVtable = CSF_ADDR_VT(0x00841EE0);      // off_841EE0

// Callees, bound by address.
CSF_FUNC(uint32_t*, __fastcall, C_00830438_Construct, 0x00712CC0, (uint32_t* self, void* edx));
CSF_FUNC(void*, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void*, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

// __thiscall constructor (ret 4): initializes the embedded CMenuScreen base at +4,
// sets the intro-video vtables, and fills in the video-playback state fields.
static CMenuIntroVideo* __fastcall CMenuIntroVideo_Construct(CMenuIntroVideo* self, void* /*edx*/, int a2)
{
    // Primary vtable is written before the embedded base is constructed.
    CSF_AT(uint32_t, self, 0) = kCMenuScreenPrimaryVtable;

    // Construct the embedded base sub-object at this + 4.
    C_00830438_Construct(
        reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(self) + 4), nullptr);

    // Install the final vtables.
    CSF_AT(uint32_t, self, 0) = kCMenuIntroVideoVtable;
    self->field_0004 = kCMenuScreenSubobjectVtable;

    self->field_001c = static_cast<uint32_t>(a2);
    self->field_0018 = 0;
    self->field_0024 = 0;
    self->field_0014 = 0;

    // Flag derived from a global object's signature word at +0x10.
    void* globalValue = GetGlobalValue_8B056C();
    self->field_0030 = static_cast<uint8_t>(
        CSF_AT(uint32_t, globalValue, 0x10) == 0x88EE5500u ? 1 : 0);

    self->field_0020 = 0;

    // Query a virtual method (vtable slot 0x48 / 4 = 18) on the global state object.
    void* globalState = GetGlobalState_50D190();
    uint8_t stateFlag = reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(
        CSF_AT(void**, globalState, 0)[18])(globalState, nullptr);
    self->field_0028 = stateFlag;

    self->field_0029 = 0;
    self->field_002a = 0;
    self->field_0014 = 0;
    self->field_002c = 0.0f;

    return self;
}

HOOK(0x00535790, CMenuIntroVideo_Construct, "auto");
