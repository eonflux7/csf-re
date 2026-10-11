#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(uint32_t*, __fastcall, InitS00402350Object, 0x00402350, (void* self, void* edx));

// Snapshot of the 0x3C-byte record that S_00402350 describes. Built from this+0x08..+0x1C
// and copied into the caller's output buffer.
static void* __fastcall C_0086379c_CaptureSnapshotTo(C_0086379c* self, void* /*edx*/, void* outBuffer)
{
    // The original references S_00402350's vtable through the constructor call below.
    (void)CSF_VTBL(0x00402350);

    alignas(8) uint8_t snapshotBytes[0x3C];
    S_00402350& snapshot = *reinterpret_cast<S_00402350*>(snapshotBytes);

    InitS00402350Object(&snapshot, nullptr);

    // Fields read from this+0x08, +0x0C, +0x10.
    uint32_t firstWord = self->field_0008;
    uint32_t secondWord = CSF_AT(uint32_t, self, 0x0C);
    uint32_t thirdWord = CSF_AT(uint32_t, self, 0x10);

    // Sub-object at this+0x14: its vtable word, the dword at +0x18, and the dword at +0x1C.
    uint32_t subVtableWord = CSF_AT(uint32_t, self, 0x14);
    uint32_t subPayloadWord = CSF_AT(uint32_t, self, 0x18);
    uint32_t subTrailingWord = CSF_AT(uint32_t, self, 0x1C);

    snapshot.field_0000 = firstWord;
    snapshot.field_0004 = secondWord;
    snapshot.field_0008 = thirdWord;
    snapshot.field_000c = subVtableWord;
    snapshot.field_0010 = subPayloadWord;
    snapshot.field_0014 = subTrailingWord;

    std::memcpy(outBuffer, snapshotBytes, 0x3C);
    return outBuffer;
}

HOOK(0x0070C2A0, C_0086379c_CaptureSnapshotTo, "auto");
