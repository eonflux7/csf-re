#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Inner sub-object initializer (thiscall, no stack args, ret 0).
CSF_FUNC(void*, __fastcall, InnerSubobject_Init, 0x0068B250, (void* self, void* edx));

// Constructor-style initializer for the net_perf_counters object.
// Layout as seen in the original:
//   +0x00 vtable (0x872D08)
//   +0x04 zeroed dword
//   +0x08 embedded sub-object, initialized by 0x68B250
//   +0x24 first constructor argument
//   +0x28 second constructor argument
static uint32_t* __fastcall NetPerfCounters_Construct(void* self, void* /*edx*/, uint32_t a2, uint32_t a3)
{
    CSF_AT(uint32_t, self, 0x04) = 0;
    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x872D08);

    InnerSubobject_Init(static_cast<uint8_t*>(self) + 0x08, nullptr);

    CSF_AT(uint32_t, self, 0x24) = a2;
    CSF_AT(uint32_t, self, 0x28) = a3;

    return static_cast<uint32_t*>(self);
}

HOOK(0x0078D0E0, NetPerfCounters_Construct, "auto");
