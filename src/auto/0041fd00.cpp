#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_41F0A0, 0x0041F0A0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

typedef int  (__fastcall* ArchiveRwFn)(void* stream, void* edx, void* data, void* ar);
typedef char (__fastcall* ArchiveBoolFn)(void* stream, void* edx, void* a, void* b);
typedef int  (__fastcall* ArchiveEndFn)(void* stream, void* edx, void* ar);

// Serializes (loads/saves) the task state through the archive object.
static int __fastcall TaskState_Serialize(S_0041f0a0* self, void* /*edx*/, void* archive)
{
    uint8_t* ar = reinterpret_cast<uint8_t*>(archive);
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    uint8_t flag;
    if (CSF_AT(uint8_t, ar, 0) != 0) {
        flag = 0;
    } else {
        void* stream = CSF_AT(void*, ar, 4);
        char r = ((ArchiveBoolFn)CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, nullptr, ar);
        flag = (r == 0) ? 1 : 0;
    }
    int savedField = CSF_AT(int, ar, 0x1c);
    CSF_AT(uint8_t, ar, 0) = flag;

    sub_41F0A0(self, nullptr, ar);

    CSF_AT(int, ar, 0x1c) = 0;
    if (CSF_AT(uint8_t, ar, 0) == 0) {
        void* stream = CSF_AT(void*, ar, 4);
        CSF_AT(uint8_t, ar, 0x15) = 0;
        CSF_AT(uint8_t, ar, 0x16) = 0;
        int r = ((ArchiveRwFn)CSF_AT(void**, stream, 0)[0x38 / 4])(stream, nullptr, base + 0x20, ar);
        CSF_AT(uint8_t, ar, 0) = (r == -1);
    }

    CSF_AT(int, ar, 0x1c) = 0;
    if (CSF_AT(uint8_t, ar, 0) == 0) {
        void* stream = CSF_AT(void*, ar, 4);
        CSF_AT(uint8_t, ar, 0x15) = 0;
        CSF_AT(uint8_t, ar, 0x16) = 0;
        int r = ((ArchiveRwFn)CSF_AT(void**, stream, 0)[0x38 / 4])(stream, nullptr, base + 0x2c, ar);
        CSF_AT(uint8_t, ar, 0) = (r == -1);
    }

    CSF_AT(int, ar, 0x1c) = 0;
    if (CSF_AT(uint8_t, ar, 0) == 0) {
        void* stream = CSF_AT(void*, ar, 4);
        CSF_AT(uint8_t, ar, 0x15) = 0;
        CSF_AT(uint8_t, ar, 0x16) = 0;
        int r = ((ArchiveRwFn)CSF_AT(void**, stream, 0)[0x54 / 4])(stream, nullptr, base + 0x3c, ar);
        CSF_AT(uint8_t, ar, 0) = (r == -1);
    }

    CSF_AT(int, ar, 0x1c) = 0;
    if (CSF_AT(uint8_t, ar, 0) == 0) {
        void* stream = CSF_AT(void*, ar, 4);
        CSF_AT(uint8_t, ar, 0x15) = 0;
        CSF_AT(uint8_t, ar, 0x16) = 0;
        int r = ((ArchiveRwFn)CSF_AT(void**, stream, 0)[0x44 / 4])(stream, nullptr, &self->field_0040, ar);
        CSF_AT(uint8_t, ar, 0) = (r == -1);
    }

    CSF_AT(int, ar, 0x1c) = 0;
    if (CSF_AT(uint8_t, ar, 0) == 0) {
        void* stream = CSF_AT(void*, ar, 4);
        CSF_AT(uint8_t, ar, 0x15) = 0;
        CSF_AT(uint8_t, ar, 0x16) = 0;
        int r = ((ArchiveRwFn)CSF_AT(void**, stream, 0)[0x58 / 4])(stream, nullptr, base + 0x38, ar);
        CSF_AT(uint8_t, ar, 0) = (r == -1);
    }

    CSF_AT(int, ar, 0x1c) = 0;
    if (CSF_AT(uint8_t, ar, 0) == 0) {
        CSF_AT(uint8_t, ar, 0x15) = 1;
        CSF_AT(uint8_t, ar, 0x16) = 0;
        Archive_SerializeObjectRef(base + 0x1c, reinterpret_cast<bool*>(ar));
    }

    CSF_AT(int, ar, 0x1c) = 0;
    if (CSF_AT(uint8_t, ar, 0) == 0) {
        void* stream = CSF_AT(void*, ar, 4);
        CSF_AT(uint8_t, ar, 0x15) = 0;
        CSF_AT(uint8_t, ar, 0x16) = 0;
        int r = ((ArchiveRwFn)CSF_AT(void**, stream, 0)[0x54 / 4])(stream, nullptr, base + 0x44, ar);
        CSF_AT(uint8_t, ar, 0) = (r == -1);
    }

    CSF_AT(int, ar, 0x1c) = savedField;
    if (CSF_AT(uint8_t, ar, 0) == 0) {
        void* stream = CSF_AT(void*, ar, 4);
        int r = ((ArchiveEndFn)CSF_AT(void**, stream, 0)[0x7c / 4])(stream, nullptr, ar);
        if ((r & 0xFF) != 0) {
            CSF_AT(uint8_t, ar, 0) = 0;
            return r;
        }
    }
    CSF_AT(uint8_t, ar, 0) = 1;
    return 1;
}

HOOK(0x0041FD00, TaskState_Serialize, "auto");
