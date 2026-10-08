#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callee: CMenuScreen inner base constructor (__thiscall, no stack args, returns this).
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x712CC0, (void* self, void* edx));

// Vtable addresses referenced by the original constructor, spelled out by address.
static const uintptr_t kVtblCMenuScreenBase = CSF_ADDR_VT(0x0083042C);   // CMenuScreen base vtable (written before base ctor)
static const uintptr_t kVtblCMenuLanguage = CSF_ADDR_VT(0x00841CD0);     // CMenuLanguage primary vtable
static const uintptr_t kVtblCMenuLanguageInner = CSF_ADDR_DATA(0x00841C80); // CMenuLanguage inner sub-object vtable

// Constructor of CMenuLanguage (__thiscall, one stack arg -> ret 4).
// Mirrors the original store order exactly: base vtable, base ctor, own vtables,
// zeroed state, owner argument at +0x38, flag byte at +0x58, returns this.
static CMenuLanguage* __fastcall CMenuLanguage_Ctor(CMenuLanguage* self, void* /*edx*/, int ownerArg)
{
    // Install the CMenuScreen base vtable at +0, then construct the embedded base at +4.
    CSF_AT(void*, self, 0x00) = reinterpret_cast<void*>(kVtblCMenuScreenBase);
    C_00830438_Construct(reinterpret_cast<uint8_t*>(self) + 4, nullptr);

    // Install CMenuLanguage vtables: primary at +0, inner sub-object vtable at +4.
    CSF_AT(void*, self, 0x00) = reinterpret_cast<void*>(kVtblCMenuLanguage);
    CSF_AT(void*, self, 0x04) = reinterpret_cast<void*>(kVtblCMenuLanguageInner);

    // Zero-initialize the language state.
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;
    self->field_0050 = 0;
    self->field_0054 = 15;
    CSF_AT(uint8_t, self, 0x40) = 0;   // original writes a single byte here
    self->field_0030 = 0;
    self->field_0034 = 0;

    // Store the constructor argument and set the flag byte.
    self->field_0038 = static_cast<uint32_t>(ownerArg);
    self->field_0058 = 1;

    return self;
}

HOOK(0x00534540, CMenuLanguage_Ctor, "auto");
