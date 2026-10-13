#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Copies the struct's second and third dwords into the caller's two output slots.
// Returns the third dword. Matches the original ret 8 (__thiscall, two stack args).
static int __fastcall S_00692b10_GetFileData(S_00692b10* self, void* /*edx*/,
                                             uint32_t* outFirst, uint32_t* outSecond) {
    *outFirst = self->field_0004;
    uint32_t second = self->field_0008;
    *outSecond = second;
    return static_cast<int>(second);
}

HOOK(0x00692900, S_00692b10_GetFileData, "auto");
