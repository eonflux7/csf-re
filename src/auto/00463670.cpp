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

CSF_FUNC(int, __fastcall, sub_46F390, 0x0046F390, (void* self, void* edx, int a2));

// __thiscall with three stack args (a2..a4), callee pops 12 bytes.
static int __fastcall AdvanceIndexFromOwnerTable(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    (void)a2;
    (void)a3;
    (void)a4;

    // First call: owner pointer at +0x10 goes in ecx, current index at +0x1C is pushed.
    uint32_t ownerBefore = CSF_AT(uint32_t, self, 0x10);
    uint32_t currentIndex = CSF_AT(uint32_t, self, 0x1C);
    int slot = sub_46F390((void*)(uintptr_t)ownerBefore, nullptr, (int)currentIndex);

    // Reload owner after the call; read its table at +0x60 and fetch the 8-byte entry for slot.
    uint32_t ownerAfter = CSF_AT(uint32_t, self, 0x10);
    uint32_t tableBase = CSF_AT(uint32_t, (void*)(uintptr_t)ownerAfter, 0x60);
    uint32_t entryAddr = tableBase + (uint32_t)slot * 8u;
    int result = (int)*reinterpret_cast<uint32_t*>((uintptr_t)entryAddr) - 1;

    CSF_AT(uint8_t, self, 0x81) = 1;
    CSF_AT(int, self, 0x1C) = result;
    return result;
}

HOOK(0x463670, AdvanceIndexFromOwnerTable, "auto");
