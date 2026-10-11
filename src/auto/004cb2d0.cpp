#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoverAttack_Sub4C2380, 0x004C2380, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, CoverAttack_Sub4C9D70, 0x004C9D70, (uint32_t* a1, int a2));

typedef int (__fastcall* VCall2Fn)(void* self, void* edx, void* arg0, void* arg1);

static int __fastcall CoverAttack_RunTaskSteps(void* self, void* /*edx*/, int ctxArg)
{
    uint8_t* ctx = reinterpret_cast<uint8_t*>(ctxArg);
    uint8_t* thisBytes = reinterpret_cast<uint8_t*>(self);

    int savedField1C = CSF_AT(int, ctx, 0x1C);

    uint8_t first;
    if (CSF_AT(uint8_t, ctx, 0) != 0) {
        first = 0;
    } else {
        void* obj = CSF_AT(void*, ctx, 4);
        typedef uint8_t (__fastcall* Fn80)(void*, void*, int, void*);
        Fn80 fn = reinterpret_cast<Fn80>(CSF_AT(void**, obj, 0)[0x80 / 4]);
        uint8_t r = fn(obj, nullptr, 0, ctx);
        first = (r != 0) ? 0 : 1;
    }
    CSF_AT(uint8_t, ctx, 0) = first;

    CoverAttack_Sub4C2380(self, nullptr, ctxArg);

    bool done = CSF_AT(uint8_t, ctx, 0) != 0;
    CSF_AT(int, ctx, 0x1C) = 0;
    if (!done) {
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        CoverAttack_Sub4C9D70(reinterpret_cast<uint32_t*>(thisBytes + 0x14), ctxArg);
    }

    done = CSF_AT(uint8_t, ctx, 0) != 0;
    CSF_AT(int, ctx, 0x1C) = 0;
    if (!done) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        VCall2Fn fn = reinterpret_cast<VCall2Fn>(CSF_AT(void**, obj, 0)[0x48 / 4]);
        int r = fn(obj, nullptr, thisBytes + 0x24, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    done = CSF_AT(uint8_t, ctx, 0) != 0;
    CSF_AT(int, ctx, 0x1C) = 0;
    if (!done) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        VCall2Fn fn = reinterpret_cast<VCall2Fn>(CSF_AT(void**, obj, 0)[0x54 / 4]);
        int r = fn(obj, nullptr, thisBytes + 0x28, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    done = CSF_AT(uint8_t, ctx, 0) != 0;
    CSF_AT(int, ctx, 0x1C) = 0;
    if (!done) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        VCall2Fn fn = reinterpret_cast<VCall2Fn>(CSF_AT(void**, obj, 0)[0x38 / 4]);
        int r = fn(obj, nullptr, thisBytes + 0x2C, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = savedField1C;

    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        typedef int (__fastcall* Fn7C)(void*, void*, void*);
        Fn7C fn = reinterpret_cast<Fn7C>(CSF_AT(void**, obj, 0)[0x7C / 4]);
        int result = fn(obj, nullptr, ctx);
        if ((result & 0xFF) != 0) {
            CSF_AT(uint8_t, ctx, 0) = 0;
            return result;
        }
    }
    CSF_AT(uint8_t, ctx, 0) = 1;
    return 1;
}

HOOK(0x004CB2D0, CoverAttack_RunTaskSteps, "auto");
