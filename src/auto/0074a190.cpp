#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Scans the spawn-point table in C_00867900 (records of 0x120 bytes starting at +0x34)
// for a slot whose player/object id equals a2. Each record holds six id fields at
// relative offsets -0x30, 0x00, +0x30, +0x60, +0x90, +0xC0. Returns the matching
// slot index, or -1 if none is found.
static int __fastcall MpSpawnPoints_FindPlayerIndex(C_00867900* self, void* /*edx*/, int a2) {
    int result = -1;
    int slotIndex = 2;          // v3: running index, advances by 6 per record
    int recordBase = 0x34;      // v4 offset of the current record inside self

    do {
        if (result != -1)
            break;

        if (CSF_AT(int, self, recordBase - 0x30) == a2) {
            result = slotIndex - 2;
            if (slotIndex != 1)
                break;
        }
        if (CSF_AT(int, self, recordBase) == a2) {
            result = slotIndex - 1;
            if (slotIndex != 0)
                break;
        }
        if (CSF_AT(int, self, recordBase + 0x30) == a2) {
            result = slotIndex;
            if (slotIndex != -1)
                break;
        }
        if (CSF_AT(int, self, recordBase + 0x60) == a2) {
            result = slotIndex + 1;
            if (slotIndex != -2)
                break;
        }
        if (CSF_AT(int, self, recordBase + 0x90) == a2) {
            result = slotIndex + 2;
            if (slotIndex != -3)
                break;
        }
        if (CSF_AT(int, self, recordBase + 0xC0) == a2) {
            result = slotIndex + 3;
        }

        slotIndex += 6;
        recordBase += 0x120;
    } while (slotIndex - 2 < 30);

    return result;
}

HOOK(0x0074A190, MpSpawnPoints_FindPlayerIndex, "auto");
