#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __stdcall, SoundSlot_ResetFlags, 0x0041F6C0, (uint32_t* a1, char* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

typedef unsigned char (__fastcall* ArchiveBoolFn)(void* obj, void* edx, void* a, void* b);
typedef int (__fastcall* ArchiveIoFn)(void* obj, void* edx, void* data, void* stream);
typedef int (__fastcall* ArchiveFinishFn)(void* obj, void* edx, void* stream);

// Serializes a task-state block (three byte-sized members, a sound slot, an object ref and a
// trailing member) through the archive stream object `stream`.
static int __fastcall TaskState_SerializeFields(void* self, void* /*edx*/, uint8_t* stream)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    int savedValue = CSF_AT(int, stream, 0x1C);

    // Begin section: only touches the archive object when no error is pending.
    bool beginFailed = false;
    if (CSF_AT(uint8_t, stream, 0) == 0) {
        void* obj = CSF_AT(void*, stream, 4);
        unsigned char began = ((ArchiveBoolFn)CSF_AT(void**, obj, 0)[0x80 / 4])(obj, nullptr, nullptr, stream);
        if (began == 0) {
            beginFailed = true;
        }
    }
    CSF_AT(uint8_t, stream, 0) = beginFailed ? 1 : 0;

    CSF_AT(int, stream, 0x1C) = 0;
    if (!beginFailed) {
        void* obj = CSF_AT(void*, stream, 4);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        int r = ((ArchiveIoFn)CSF_AT(void**, obj, 0)[0x44 / 4])(obj, nullptr, base, stream);
        CSF_AT(uint8_t, stream, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, stream, 0x1C) = 0;
    if (CSF_AT(uint8_t, stream, 0) == 0) {
        void* obj = CSF_AT(void*, stream, 4);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        int r = ((ArchiveIoFn)CSF_AT(void**, obj, 0)[0x44 / 4])(obj, nullptr, base + 1, stream);
        CSF_AT(uint8_t, stream, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, stream, 0x1C) = 0;
    if (CSF_AT(uint8_t, stream, 0) == 0) {
        void* obj = CSF_AT(void*, stream, 4);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        int r = ((ArchiveIoFn)CSF_AT(void**, obj, 0)[0x44 / 4])(obj, nullptr, base + 2, stream);
        CSF_AT(uint8_t, stream, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, stream, 0x1C) = 0;
    if (CSF_AT(uint8_t, stream, 0) == 0) {
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        SoundSlot_ResetFlags(reinterpret_cast<uint32_t*>(base + 4), reinterpret_cast<char*>(stream));
    }

    CSF_AT(int, stream, 0x1C) = 0;
    if (CSF_AT(uint8_t, stream, 0) == 0) {
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        Archive_SerializeObjectRef(base + 0x14, reinterpret_cast<bool*>(stream));
    }

    CSF_AT(int, stream, 0x1C) = 0;
    if (CSF_AT(uint8_t, stream, 0) == 0) {
        void* obj = CSF_AT(void*, stream, 4);
        CSF_AT(uint8_t, stream, 0x15) = 0;
        CSF_AT(uint8_t, stream, 0x16) = 0;
        int r = ((ArchiveIoFn)CSF_AT(void**, obj, 0)[0x54 / 4])(obj, nullptr, base + 0x18, stream);
        CSF_AT(uint8_t, stream, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, stream, 0x1C) = savedValue;
    if (CSF_AT(uint8_t, stream, 0) == 0) {
        void* obj = CSF_AT(void*, stream, 4);
        int r = ((ArchiveFinishFn)CSF_AT(void**, obj, 0)[0x7C / 4])(obj, nullptr, stream);
        if ((r & 0xFF) != 0) {
            CSF_AT(uint8_t, stream, 0) = 0;
            return r;
        }
    }
    CSF_AT(uint8_t, stream, 0) = 1;
    return 1;
}

HOOK(0x0041F8C0, TaskState_SerializeFields, "auto");
