#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, SceneLoader_SerializeRecordMember, 0x00445620, (void* a1, int a2));
CSF_FUNC(unsigned int, __stdcall, CoverTask_RunStageA, 0x004A3730, (void* a1, char* a2));
CSF_FUNC(unsigned int, __stdcall, CoverTask_RunStageB, 0x004BDB20, (void* a1, char* a2));

// Runs the sub-stages of a task record, tracking completion in record[0].
static int __fastcall CoverTask_ProcessRecord(S_0041e5f0* self, void* /*edx*/, char* record)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    int savedField = CSF_AT(int, record, 0x1C);

    bool done = false;
    if (*record == 0) {
        void* owner = CSF_AT(void*, record, 4);
        typedef int(__fastcall * VFn80)(void*, void*, int, char*);
        int r = ((VFn80)CSF_AT(void**, owner, 0)[0x80 / 4])(owner, nullptr, 0, record);
        if ((r & 0xFF) == 0) done = true;
    }
    *record = done ? 1 : 0;
    CSF_AT(int, record, 0x1C) = 0;
    if (!done) {
        record[0x15] = 0;
        record[0x16] = 0;
        CoverTask_RunStageB(self, record);
    }

    bool flag = (*record == 0);
    CSF_AT(int, record, 0x1C) = 0;
    if (flag) {
        record[0x15] = 0;
        record[0x16] = 0;
        CoverTask_RunStageA(base + 0x10, record);
    }

    flag = (*record == 0);
    CSF_AT(int, record, 0x1C) = 0;
    if (flag) {
        record[0x15] = 0;
        record[0x16] = 0;
        SceneLoader_SerializeRecordMember(base + 0x20, (int)record);
    }

    flag = (*record == 0);
    CSF_AT(int, record, 0x1C) = savedField;
    if (flag) {
        void* owner = CSF_AT(void*, record, 4);
        typedef int(__fastcall * VFn7C)(void*, void*, char*);
        int r = ((VFn7C)CSF_AT(void**, owner, 0)[0x7C / 4])(owner, nullptr, record);
        if ((r & 0xFF) != 0) {
            *record = 0;
            return r;
        }
    }
    *record = 1;
    return 1;
}

HOOK(0x004C9B10, CoverTask_ProcessRecord, "auto");
