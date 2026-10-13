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

// Global object accessor (cdecl, no arguments).
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());

// Receives a record and a flag; stack args are popped by the callee (ret 8).
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));

// Refreshes the spawn-state record attached to this object: builds a 0x34-byte
// record from two stored values, dispatches it to the global object, then calls
// the function pointer stored at child+0x48 (child = this[1], called with this = &this[1]),
// this object's slot 9, and finally slot 35 (returning its result).
static int __fastcall RefreshAttachedSpawnState(void* self, void* /*edx*/) {
    // Values read from this object (this[8] and this[9]).
    int spawnPrimary = CSF_AT(int, self, 0x20);
    int spawnSecondary = CSF_AT(int, self, 0x24);

    // Stack-resident record: 0x34 bytes, laid out as the original frame does.
    alignas(8) uint8_t recordBytes[0x34];
    void* record = recordBytes;
    memset(recordBytes, 0, sizeof(recordBytes));

    CSF_AT(void*, record, 0x00) = CSF_VTBL(0x0082D478);
    CSF_AT(int, record, 0x04) = 7;
    CSF_AT(void*, record, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(int, record, 0x0C) = spawnPrimary;
    CSF_AT(int, record, 0x10) = spawnSecondary;
    CSF_AT(void*, record, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(int, record, 0x18) = -1;
    CSF_AT(int, record, 0x1C) = -1;
    memset(static_cast<uint8_t*>(record) + 0x20, 0, 16);
    CSF_AT(uint8_t, record, 0x30) = 0;

    // Fetch the global object, then dispatch the record to it.
    void* globalObject = reinterpret_cast<void*>(GetGlobalObjectField184());
    DispatchRecordToGlobalObject(globalObject, nullptr, record, 0);

    // Child object stored at this[1]: its function pointer at +0x48 is called
    // directly (not through a vtable), with this = &this[1] and one out buffer.
    uint32_t outBuffer[3];
    memset(outBuffer, 0, sizeof(outBuffer));
    void* child = CSF_AT(void*, self, 0x04);
    void* childSelf = static_cast<uint8_t*>(self) + 0x04;
    ((void (__fastcall*)(void*, void*, void*))CSF_AT(void*, child, 0x48))(childSelf, nullptr, outBuffer);

    // This object's slot 9 (no arguments).
    ((void (__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[36 / 4])(self, nullptr);

    // This object's slot 35 with (0, 0, -1, 0); its result is returned.
    return ((int (__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, self, 0)[140 / 4])(self, nullptr, 0, 0, -1, 0);
}

HOOK(0x00493270, RefreshAttachedSpawnState, "auto");
