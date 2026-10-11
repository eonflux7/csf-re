#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, AudioSoundManager_ProcessArchiveContext, 0x00405450, (int a1));
CSF_FUNC(int, __stdcall, sub_4C9D70, 0x004C9D70, (uint32_t* a1, int a2));

static int __fastcall CoverAttackTask_ProcessContext(void* self, void* /*edx*/, uint8_t* ctx)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    int savedField1C = CSF_AT(int, ctx, 0x1C);

    // Step 1: query the context object (vtable slot 0x80 / 4)
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        void** vt = CSF_AT(void**, obj, 0);
        bool r = ((bool(__fastcall*)(void*, void*, int, int))vt[0x80 / 4])(obj, nullptr, 0, (int)ctx);
        CSF_AT(uint8_t, ctx, 0) = r ? 0 : 1;
    } else {
        CSF_AT(uint8_t, ctx, 0) = 0;
    }

    AudioSoundManager_ProcessArchiveContext((int)ctx);

    bool failed = CSF_AT(uint8_t, ctx, 0) == 0;
    CSF_AT(int, ctx, 0x1C) = 0;
    if (failed) {
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        sub_4C9D70(reinterpret_cast<uint32_t*>(base + 4), (int)ctx);
    }

    failed = CSF_AT(uint8_t, ctx, 0) == 0;
    CSF_AT(int, ctx, 0x1C) = 0;
    if (failed) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void** vt = CSF_AT(void**, obj, 0);
        int r = ((int(__fastcall*)(void*, void*, void*, int))vt[0x48 / 4])(obj, nullptr, base + 0x14, (int)ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    failed = CSF_AT(uint8_t, ctx, 0) == 0;
    CSF_AT(int, ctx, 0x1C) = 0;
    if (failed) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void** vt = CSF_AT(void**, obj, 0);
        int r = ((int(__fastcall*)(void*, void*, void*, int))vt[0x54 / 4])(obj, nullptr, base + 0x18, (int)ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = savedField1C;

    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        void** vt = CSF_AT(void**, obj, 0);
        int r = ((int(__fastcall*)(void*, void*, int))vt[0x7C / 4])(obj, nullptr, (int)ctx);
        if ((uint8_t)r != 0) {
            CSF_AT(uint8_t, ctx, 0) = 0;
            return r;
        }
    }

    CSF_AT(uint8_t, ctx, 0) = 1;
    return 1;
}

HOOK(0x004CB3C0, CoverAttackTask_ProcessContext, "auto");
