#include <cstdint>
#include "hook.h"

// Three-way style comparator for two dword values. The original compares with
// cmp/sbb (CF-based), so the comparison is unsigned. Returns 1 when *a2 < *a1,
// otherwise -1 (equal values also yield -1).
static int __cdecl CompareDwordPtrsDescending(const uint32_t* a1, const uint32_t* a2) {
    return *a2 < *a1 ? 1 : -1;
}

HOOK(0x006AFDF0, CompareDwordPtrsDescending, "auto");
