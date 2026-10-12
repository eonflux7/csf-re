#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

// Serializes the seven members of a small record through an archive/stream object.
//
// Archive context layout (as accessed here):
//   +0x00  bool   failed
//   +0x04  void*  stream (polymorphic)
//   +0x15  bool   per-field flag A
//   +0x16  bool   per-field flag B
//   +0x1C  int    nesting / field-name cursor (saved, zeroed while inside the record, restored)
//
// Record layout (self):
//   +0x00, +0x04, +0x08, +0x10, +0x18, +0x1C, +0x20
static int __fastcall Record_SerializeFields(void* self, void* /*edx*/, void* archive)
{
    const int savedCursor = CSF_AT(int, archive, 0x1C);

    // Begin record: fails if already failed or the stream refuses to open the block.
    bool failed;
    if (CSF_AT(uint8_t, archive, 0x0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        const bool opened =
            ((bool(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, archive);
        failed = !opened ? true : false;
    }
    else
    {
        failed = false;
    }

    CSF_AT(uint8_t, archive, 0x0) = failed ? 1 : 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int rc =
            ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x58 / 4])(stream, nullptr, self, archive);
        CSF_AT(uint8_t, archive, 0x0) = (rc == -1) ? 1 : 0;
    }

    const bool ok1 = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (ok1)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int rc =
            ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x54 / 4])(
                stream, nullptr, (uint8_t*)self + 0x4, archive);
        CSF_AT(uint8_t, archive, 0x0) = (rc == -1) ? 1 : 0;
    }

    const bool ok2 = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (ok2)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int rc =
            ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x34 / 4])(
                stream, nullptr, (uint8_t*)self + 0x8, archive);
        CSF_AT(uint8_t, archive, 0x0) = (rc == -1) ? 1 : 0;
    }

    const bool ok3 = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (ok3)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int rc =
            ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x34 / 4])(
                stream, nullptr, (uint8_t*)self + 0x10, archive);
        CSF_AT(uint8_t, archive, 0x0) = (rc == -1) ? 1 : 0;
    }

    const bool ok4 = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (ok4)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int rc =
            ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x54 / 4])(
                stream, nullptr, (uint8_t*)self + 0x18, archive);
        CSF_AT(uint8_t, archive, 0x0) = (rc == -1) ? 1 : 0;
    }

    const bool ok5 = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (ok5)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int rc =
            ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x54 / 4])(
                stream, nullptr, (uint8_t*)self + 0x1C, archive);
        CSF_AT(uint8_t, archive, 0x0) = (rc == -1) ? 1 : 0;
    }

    const bool ok6 = CSF_AT(uint8_t, archive, 0x0) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (ok6)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int rc =
            ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x44 / 4])(
                stream, nullptr, (uint8_t*)self + 0x20, archive);
        CSF_AT(uint8_t, archive, 0x0) = (rc == -1) ? 1 : 0;
    }

    CSF_AT(int, archive, 0x1C) = savedCursor;

    // End record: closing the block must succeed too.
    if (CSF_AT(uint8_t, archive, 0x0) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x4);
        const int closeResult =
            ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, archive);
        if ((uint8_t)closeResult != 0)
        {
            CSF_AT(uint8_t, archive, 0x0) = 0;
            return closeResult;
        }
    }

    CSF_AT(uint8_t, archive, 0x0) = 1;
    return 1;
}

HOOK(0x426EC0, Record_SerializeFields, "auto");
