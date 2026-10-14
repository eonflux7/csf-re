#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Base initializer of the menu-controls object (C_00843720). Callee is __thiscall with two stack args (ret 8).
CSF_FUNC(int, __fastcall, Sub_537BF0_BaseInit, 0x00537BF0, (void* self, void* edx, int a2, int a3));

// Vtable addresses installed by the constructor, bound by their literal addresses.
static const uint32_t kMenuControlsPrimaryVtbl   = CSF_ADDR_VT(0x00843720);
static const uint32_t kMenuControlsSecondaryVtbl = CSF_ADDR_VT(0x008436D0);
static const uint32_t kMenuControlsTertiaryVtbl  = CSF_ADDR_VT(0x008436CC);

static void* __fastcall C_00843720_Construct(void* self, void* /*edx*/, int a2, int a3)
{
    Sub_537BF0_BaseInit(self, nullptr, a2, a3);

    CSF_AT(uint32_t, self, 0x00) = kMenuControlsPrimaryVtbl;
    CSF_AT(uint32_t, self, 0x04) = kMenuControlsSecondaryVtbl;
    CSF_AT(uint32_t, self, 0x10) = kMenuControlsTertiaryVtbl;

    CSF_AT(uint8_t, self, 0x27C) = 0;
    CSF_AT(uint32_t, self, 0x278) = 1;

    return self;
}

HOOK(0x00543810, C_00843720_Construct, "auto");
