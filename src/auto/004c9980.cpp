#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TaskCheck_4A9E10, 0x004A9E10, (void* self, void* edx, void* a2));
CSF_FUNC(int, __stdcall, TaskCheck_4B9120, 0x004B9120, (int* a1, void* a2));
CSF_FUNC(int, __fastcall, TaskCheck_4C2380, 0x004C2380, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, TaskCheck_4C2480, 0x004C2480, (void* self, void* edx, int a2));

static int __fastcall CoverAttack_EvaluateConditions(S_004c2380* self, void* /*edx*/, int a2)
{
    uint8_t* ctx = reinterpret_cast<uint8_t*>(a2);
    uint8_t* me = reinterpret_cast<uint8_t*>(self);
    int savedField = CSF_AT(int, ctx, 0x1C);

    uint8_t failed;
    if (CSF_AT(uint8_t, ctx, 0) != 0) {
        failed = 0;
    } else {
        void* obj = CSF_AT(void*, ctx, 4);
        uint8_t r = ((uint8_t(__fastcall*)(void*, void*, int, int))CSF_AT(void**, obj, 0)[0x80 / 4])(obj, nullptr, 0, a2);
        failed = (r == 0) ? 1 : 0;
    }
    CSF_AT(uint8_t, ctx, 0) = failed;

    TaskCheck_4C2380(self, nullptr, a2);

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        TaskCheck_4C2480(me + 0x14, nullptr, a2);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        TaskCheck_4B9120(reinterpret_cast<int*>(me + 0x28), ctx);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        TaskCheck_4A9E10(me + 0x38, nullptr, ctx);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, obj, 0)[0x44 / 4])(obj, nullptr, me + 0x24, a2);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = savedField;

    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        int r = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0x7C / 4])(obj, nullptr, a2);
        if ((uint8_t)r != 0) {
            CSF_AT(uint8_t, ctx, 0) = 0;
            return r;
        }
    }
    CSF_AT(uint8_t, ctx, 0) = 1;
    return 1;
}

HOOK(0x004C9980, CoverAttack_EvaluateConditions, "auto");
