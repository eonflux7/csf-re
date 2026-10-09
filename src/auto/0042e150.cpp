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

// Prepares the source object (a2) and caches two 2-dword pairs from it into the HUD.
CSF_FUNC(int, __fastcall, CHud_PrepareSource, 0x0042E020, (void* self, void* edx, int* a2));

static uint32_t* __fastcall CHud_CopyBoundsFromSource(CHud* self, void* /*edx*/, int* a2)
{
    CHud_PrepareSource(self, nullptr, a2);

    // First pair: vtable slot 0x2C / 4 = 11
    uint32_t* firstPair = ((uint32_t*(__fastcall*)(void*, void*))CSF_AT(void**, a2, 0)[11])(a2, nullptr);
    CSF_AT(uint32_t, self, 0x314) = firstPair[0];
    CSF_AT(uint32_t, self, 0x318) = firstPair[1];

    // Second pair: vtable slot 0x3C / 4 = 15 (vtable reloaded from a2)
    uint32_t* secondPair = ((uint32_t*(__fastcall*)(void*, void*))CSF_AT(void**, a2, 0)[15])(a2, nullptr);
    CSF_AT(uint32_t, self, 0x31C) = secondPair[0];
    CSF_AT(uint32_t, self, 0x320) = secondPair[1];

    return secondPair;
}

HOOK(0x0042E150, CHud_CopyBoundsFromSource, "auto");
