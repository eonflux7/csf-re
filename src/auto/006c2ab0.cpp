#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Placement operator new: ignores the size and returns the caller-supplied buffer.
static void* __cdecl AnimCompress_PlacementNew(unsigned int size, void* place) {
    (void)size;
    return place;
}

HOOK(0x6C2AB0, AnimCompress_PlacementNew, "auto");
