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

// Vtable addresses stored into the object by the constructor.
static const uintptr_t kCMenuScreenBaseVtableAddr = CSF_ADDR_VT(0x0083042C);
static const uintptr_t kCMenuGameOptionsVtableAddr = CSF_ADDR_VT(0x00842788);
static const uintptr_t kEmbeddedSubobjectVtableAddr = CSF_ADDR_VT(0x00842738);

CSF_FUNC(void*, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(void*, __fastcall, C_00830438_Construct, 0x00712CC0, (void* self, void* edx));

// Constructor of CMenuGameOptions: installs the base vtable, constructs the
// embedded subobject at +4, installs the derived vtable, initializes option
// flags and state, reads a value from the global options object and finally
// invokes the virtual slot 16 (vtable +0x40).
// Original: __thiscall with one stack argument (ret 4).
static char* __fastcall CMenuGameOptions_Construct(CMenuGameOptions* self, void* /*edx*/, int a2)
{
    CSF_AT(uintptr_t, self, 0) = kCMenuScreenBaseVtableAddr;
    C_00830438_Construct(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 4), nullptr);

    CSF_AT(uintptr_t, self, 0) = kCMenuGameOptionsVtableAddr;
    CSF_AT(uintptr_t, self, 4) = kEmbeddedSubobjectVtableAddr;

    self->field_0014 = 1;
    self->field_0015 = 1;
    self->field_0016 = 0;
    self->field_002c = static_cast<uint32_t>(a2);
    self->field_001c = 0;
    self->field_0028 = 0;
    self->field_0054 = 1;

    void* globalObject = GetGlobalValue_8B056C();
    uint32_t globalSubValue = CSF_AT(uint32_t, globalObject, 0x18);
    self->field_0020 = globalSubValue;

    uint32_t firstChildValue = 0;
    if (globalSubValue != 0) {
        firstChildValue = CSF_AT(uint32_t, reinterpret_cast<void*>(globalSubValue), 0x10);
    }
    self->field_0024 = firstChildValue;

    self->field_0018 = 0;
    self->field_0050 = 0;
    self->field_003c = 0;
    self->field_0040 = 0;
    self->field_0044 = 0;
    self->field_0030 = 0;
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_0048 = 0;
    self->field_004c = 0;

    // Virtual call through the current vtable, slot 0x40 / 4 = 16, __thiscall with no arguments.
    reinterpret_cast<void(__fastcall*)(void*, void*)>(CSF_AT(void**, self, 0)[16])(self, nullptr);

    return reinterpret_cast<char*>(self);
}

HOOK(0x0053B6F0, CMenuGameOptions_Construct, "auto");
