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

CSF_FUNC(char, __fastcall, sub_462C00, 0x00462C00, (void* self, void* edx, int a2, char a3, char a4, void* a5));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Vtable of S_00462c00, referenced by the original's call-site region.
[[maybe_unused]] static const auto s_sceneVtableRef = CSF_VTBL(0x00462C00);

// Reads a 32-bit value from a raw address held in a uint32_t.
static uint32_t ReadDwordAt(uint32_t address)
{
    return *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(address));
}

static char __fastcall S_00462c00_DispatchEntryWithSceneOverride(S_00462c00* self, void* /*edx*/, int a2, int a3, uint32_t* a4)
{
    (void)a2;
    (void)a3;

    uint32_t controlWord = ReadDwordAt(a4[1] + 8);

    uint32_t ownerAddress = ReadDwordAt(a4[0] + 4);
    uint32_t targetAddress = ReadDwordAt(ownerAddress + 4);

    uint32_t tableOwner = self->field_0020;
    if ((controlWord & 0x800000u) != 0)
    {
        uint32_t sceneBlock = *CSF_AT(uint32_t*, g_pCurrentScene, 0xBC);
        tableOwner = ReadDwordAt(sceneBlock + 0x38);
        controlWord &= ~0x800000u;
    }

    uint32_t tableAddress = ReadDwordAt(tableOwner + 8);
    uint32_t entryAddress = ReadDwordAt(tableAddress + controlWord * 12u + 4u);

    return sub_462C00(self, nullptr, static_cast<int>(targetAddress), 1, 0,
                      reinterpret_cast<void*>(static_cast<uintptr_t>(entryAddress)));
}

HOOK(0x00464B90, S_00462c00_DispatchEntryWithSceneOverride, "auto");
