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

// Returns the first 16-bit value minus the other two, computed in 32-bit int
// (movzx each operand, then two subtractions in eax).
static int __cdecl ComputeSpawnPointBalance(uint16_t total, uint16_t first_deduction, uint16_t second_deduction)
{
    int remaining = static_cast<int>(total);
    remaining -= static_cast<int>(first_deduction);
    remaining -= static_cast<int>(second_deduction);
    return remaining;
}

HOOK(0x5516C0, ComputeSpawnPointBalance, "auto");
