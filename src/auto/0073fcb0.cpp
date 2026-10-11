#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"

// Scans the circular list headed at self+0x90 (count at self+0x94) for the first
// record whose data pointer is non-null and not yet flagged, marks it as taken
// (flag byte at data+0x10 = 1) and returns its data pointer. Returns 0 otherwise.
static int __fastcall GetNextPendingSpawnRecord(uint8_t* self, void* /*edx*/) {
    int result = 0;

    if (CSF_AT(uint32_t, self, 0x94) != 0) {
        uint8_t* listHead = CSF_AT(uint8_t*, self, 0x90);
        uint8_t* node = CSF_AT(uint8_t*, listHead, 0);

        if (node != CSF_AT(uint8_t*, self, 0x90)) {
            do {
                if (result != 0) {
                    break;
                }

                uint8_t* record = CSF_AT(uint8_t*, node, 8);
                if (record == nullptr || CSF_AT(uint8_t, record, 0x10) != 0) {
                    result = 0;
                } else {
                    CSF_AT(uint8_t, record, 0x10) = 1;
                    result = static_cast<int>(reinterpret_cast<uintptr_t>(record));
                }

                node = CSF_AT(uint8_t*, node, 0);
            } while (node != CSF_AT(uint8_t*, self, 0x90));
        }
    }

    return result;
}

HOOK(0x73FCB0, GetNextPendingSpawnRecord, "auto");
