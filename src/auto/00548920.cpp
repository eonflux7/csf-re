// CMenuSoundOptionsPC constructor: runs the CMenuSoundOptions base constructor,
// zeroes the PC-specific fields, and installs this class's vtables.
#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Base constructor of CMenuSoundOptions (__thiscall, one stack argument, ret 4).
CSF_FUNC(void*, __fastcall, CMenuSoundOptions_BaseConstruct, 0x0053CFC0, (void* self, void* edx, int a2));

static constexpr uint32_t kVtblCMenuSoundOptionsPC = 0x00843DB8;
static constexpr uint32_t kVtblSecondaryBase = 0x00843D68;

static CMenuSoundOptionsPC* __fastcall CMenuSoundOptionsPC_Construct(CMenuSoundOptionsPC* self, void* /*edx*/, int a2)
{
    CMenuSoundOptions_BaseConstruct(self, nullptr, a2);

    CSF_AT(uint32_t, self, 0x5C) = 0;
    CSF_AT(uint32_t, self, 0x60) = 0;
    CSF_AT(uint32_t, self, 0x64) = 0;
    CSF_AT(uint8_t, self, 0x68) = 0;

    CSF_AT(uint32_t, self, 0x0) = kVtblCMenuSoundOptionsPC;
    CSF_AT(uint32_t, self, 0x4) = kVtblSecondaryBase;

    return self;
}

HOOK(0x548920, CMenuSoundOptionsPC_Construct, "auto");
