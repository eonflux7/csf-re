#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(char, __fastcall, RefreshFrameMatrix, 0x006BFC70, (void* self, void* edx));

// Copies the two raw dwords of the source pair into the frame's
// field_0018 and field_0014, then refreshes the frame.
static char __fastcall C_00860700_SetFramePairAndRefresh(C_00860700* self, void* /*edx*/, uint32_t* a2)
{
    std::memcpy(&self->field_0018, &a2[0], sizeof(uint32_t));
    std::memcpy(&self->field_0014, &a2[1], sizeof(uint32_t));
    return RefreshFrameMatrix(self, nullptr);
}

HOOK(0x006BFF20, C_00860700_SetFramePairAndRefresh, "auto");
