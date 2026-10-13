#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, S_00518780_CalculateChecksum, 0x00518780, (S_00518780* self, void* /*edx*/, int* checksum));

// Packet-listed vtable reference (0x00518780). The original performs no store to it here.
[[maybe_unused]] static const void* const s_S_00518780_VtableRef = CSF_VTBL(0x00518780);

static int __fastcall S_00518780_IsStoredChecksumMatching(S_00518780* self, void* /*edx*/)
{
    // The original leaves its out-slot uninitialized and only the callee writes it.
    int checksum;
    S_00518780_CalculateChecksum(self, nullptr, &checksum);
    return static_cast<int>(self->field_0004) == checksum ? 1 : 0;
}

HOOK(0x00518860, S_00518780_IsStoredChecksumMatching, "auto");
