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

CSF_FUNC(int, __fastcall, sub_4DC8F0, 0x004DC8F0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(uint32_t*, __fastcall, StaticAi_InitializeWaypointAssessment, 0x004ADFC0, (void* self, void* edx));

// Vtable addresses referenced by the original's calls (no runtime effect here).
[[maybe_unused]] static const void* const kSearchVtblRef = CSF_VTBL(0x004DC8F0);
[[maybe_unused]] static const void* const kAssessmentVtblRef = CSF_VTBL(0x004ADFC0);

// Looks up the 16-byte slot for a key; if its payload pointer (at +0xC) is empty,
// allocates an 80-byte assessment block, initializes it, and stores it there.
static int __fastcall S_004dc8f0_GetOrCreateWaypointAssessment(S_004dc8f0* self, void* /*edx*/, uint32_t* a2)
{
    int slotIndex = sub_4DC8F0(self, nullptr, a2);
    if (slotIndex == -1)
        return 0;

    int slotOffset = slotIndex * 16;

    if (CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0068), slotOffset + 0xC) == 0)
    {
        void* block = Mem_Alloc(0x50);
        uint32_t* assessment = nullptr;
        if (block)
            assessment = StaticAi_InitializeWaypointAssessment(block, nullptr);

        CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0068), slotOffset + 0xC) =
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(assessment));
    }

    return static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0068), slotOffset + 0xC));
}

HOOK(0x004DCE10, S_004dc8f0_GetOrCreateWaypointAssessment, "auto");
