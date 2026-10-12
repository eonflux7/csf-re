#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Global 32-bit counter at 0x8BB45C (unnamed in the original).
CSF_GLOBAL(int, g_NextSequenceId, 0x8BB45C);

// Reads the global counter, stores its previous value into *outId,
// increments the counter, and returns outId.
static int* __cdecl NextSequenceIdWriteOut(int* outId) {
    int currentId = g_NextSequenceId++;
    *outId = currentId;
    return outId;
}

HOOK(0x6B27E0, NextSequenceIdWriteOut, "auto");
