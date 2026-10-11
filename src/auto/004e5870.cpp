#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

// Returns the address of element `index` in the 12-byte-stride table whose
// base pointer is stored at +0x0C of this object.
static int __fastcall C_00832eec_GetCandidateHandle(C_00832eec* self, void* /*edx*/, int index) {
    return static_cast<int>(self->field_000c + static_cast<uint32_t>(index) * 12u);
}

HOOK(0x004E5870, C_00832eec_GetCandidateHandle, "auto");

