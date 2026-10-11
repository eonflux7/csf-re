#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, SceneLoader_SerializeRecordMember, 0x00445620, (void* a1, int a2));
CSF_FUNC(int, __fastcall, CoverAttack_ProcessRecord, 0x004C2380, (void* self, void* edx, int a2));

// Serializes/validates a record through the stream object stored at a2+4.
static int __fastcall CoverAttack_SerializeRecord(void* self, void* /*edx*/, int streamArg)
{
    uint8_t* stream = reinterpret_cast<uint8_t*>(streamArg);
    uint8_t* thisBytes = reinterpret_cast<uint8_t*>(self);

    int savedField1C = CSF_AT(int, stream, 0x1C);

    bool failed;
    if (CSF_AT(uint8_t, stream, 0) != 0) {
        failed = false;
    } else {
        void* obj = CSF_AT(void*, stream, 4);
        uint8_t r = ((uint8_t(__fastcall*)(void*, void*, int, int))CSF_AT(void**, obj, 0)[0x80 / 4])(
            obj, nullptr, 0, streamArg);
        failed = (r == 0);
    }
    CSF_AT(uint8_t, stream, 0) = failed ? 1 : 0;

    CoverAttack_ProcessRecord(self, nullptr, streamArg);

    bool errorSet = CSF_AT(uint8_t, stream, 0) != 0;
    CSF_AT(int, stream, 0x1C) = 0;
    if (!errorSet) {
        void* obj = CSF_AT(void*, stream, 4);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        int r = ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, obj, 0)[0x38 / 4])(
            obj, nullptr, thisBytes + 0x14, streamArg);
        CSF_AT(uint8_t, stream, 0) = (r == -1) ? 1 : 0;
    }

    errorSet = CSF_AT(uint8_t, stream, 0) != 0;
    CSF_AT(int, stream, 0x1C) = 0;
    if (!errorSet) {
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        SceneLoader_SerializeRecordMember(thisBytes + 0x20, streamArg);
    }

    CSF_AT(int, stream, 0x1C) = savedField1C;

    if (CSF_AT(uint8_t, stream, 0) == 0) {
        void* obj = CSF_AT(void*, stream, 4);
        int r = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0x7C / 4])(
            obj, nullptr, streamArg);
        if ((uint8_t)r != 0) {
            CSF_AT(uint8_t, stream, 0) = 0;
            return r;
        }
    }
    CSF_AT(uint8_t, stream, 0) = 1;
    return 1;
}

HOOK(0x004C9A60, CoverAttack_SerializeRecord, "auto");
