#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4D0860, 0x004D0860, (void* self, void* edx, void* a2));

typedef int (__fastcall *VCallWithPtr)(void* obj, void* edx, void* arg, void* ctx);
typedef bool (__fastcall *VCallCheck)(void* obj, void* edx, int zero, void* ctx);
typedef int (__fastcall *VCallCtx)(void* obj, void* edx, void* ctx);

static int __fastcall AiAutonomous_EvaluateConditions(S_0041f0a0* self, void* /*edx*/, void* ctx)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    bool initial = false;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        VCallCheck fn = (VCallCheck)CSF_AT(void**, obj, 0)[0x80 / 4];
        initial = !fn(obj, nullptr, 0, ctx);
    }
    int savedValue = CSF_AT(int, ctx, 0x1c);
    CSF_AT(uint8_t, ctx, 0) = initial ? 1 : 0;

    sub_4D0860(self, nullptr, ctx);

    // 0x6c, slot 0x58
    {
        bool failed = CSF_AT(uint8_t, ctx, 0) != 0;
        CSF_AT(int, ctx, 0x1c) = 0;
        if (!failed) {
            void* obj = CSF_AT(void*, ctx, 4);
            CSF_AT(uint8_t, ctx, 0x15) = 0;
            CSF_AT(uint8_t, ctx, 0x16) = 0;
            VCallWithPtr fn = (VCallWithPtr)CSF_AT(void**, obj, 0)[0x58 / 4];
            CSF_AT(uint8_t, ctx, 0) = (fn(obj, nullptr, base + 0x6c, ctx) == -1) ? 1 : 0;
        }
    }
    // 0x70, slot 0x58
    {
        bool failed = CSF_AT(uint8_t, ctx, 0) != 0;
        CSF_AT(int, ctx, 0x1c) = 0;
        if (!failed) {
            void* obj = CSF_AT(void*, ctx, 4);
            CSF_AT(uint8_t, ctx, 0x15) = 0;
            CSF_AT(uint8_t, ctx, 0x16) = 0;
            VCallWithPtr fn = (VCallWithPtr)CSF_AT(void**, obj, 0)[0x58 / 4];
            CSF_AT(uint8_t, ctx, 0) = (fn(obj, nullptr, base + 0x70, ctx) == -1) ? 1 : 0;
        }
    }
    // 0x80, slot 0x38
    {
        bool failed = CSF_AT(uint8_t, ctx, 0) != 0;
        CSF_AT(int, ctx, 0x1c) = 0;
        if (!failed) {
            void* obj = CSF_AT(void*, ctx, 4);
            CSF_AT(uint8_t, ctx, 0x15) = 0;
            CSF_AT(uint8_t, ctx, 0x16) = 0;
            VCallWithPtr fn = (VCallWithPtr)CSF_AT(void**, obj, 0)[0x38 / 4];
            CSF_AT(uint8_t, ctx, 0) = (fn(obj, nullptr, base + 0x80, ctx) == -1) ? 1 : 0;
        }
    }
    // 0x74, slot 0x38
    {
        bool failed = CSF_AT(uint8_t, ctx, 0) != 0;
        CSF_AT(int, ctx, 0x1c) = 0;
        if (!failed) {
            void* obj = CSF_AT(void*, ctx, 4);
            CSF_AT(uint8_t, ctx, 0x15) = 0;
            CSF_AT(uint8_t, ctx, 0x16) = 0;
            VCallWithPtr fn = (VCallWithPtr)CSF_AT(void**, obj, 0)[0x38 / 4];
            CSF_AT(uint8_t, ctx, 0) = (fn(obj, nullptr, base + 0x74, ctx) == -1) ? 1 : 0;
        }
    }
    // 0x8c, slot 0x44
    {
        bool failed = CSF_AT(uint8_t, ctx, 0) != 0;
        CSF_AT(int, ctx, 0x1c) = 0;
        if (!failed) {
            void* obj = CSF_AT(void*, ctx, 4);
            CSF_AT(uint8_t, ctx, 0x15) = 0;
            CSF_AT(uint8_t, ctx, 0x16) = 0;
            VCallWithPtr fn = (VCallWithPtr)CSF_AT(void**, obj, 0)[0x44 / 4];
            CSF_AT(uint8_t, ctx, 0) = (fn(obj, nullptr, base + 0x8c, ctx) == -1) ? 1 : 0;
        }
    }
    // 0x8d, slot 0x44
    {
        bool failed = CSF_AT(uint8_t, ctx, 0) != 0;
        CSF_AT(int, ctx, 0x1c) = 0;
        if (!failed) {
            void* obj = CSF_AT(void*, ctx, 4);
            CSF_AT(uint8_t, ctx, 0x15) = 0;
            CSF_AT(uint8_t, ctx, 0x16) = 0;
            VCallWithPtr fn = (VCallWithPtr)CSF_AT(void**, obj, 0)[0x44 / 4];
            CSF_AT(uint8_t, ctx, 0) = (fn(obj, nullptr, base + 0x8d, ctx) == -1) ? 1 : 0;
        }
    }
    // 0x8e, slot 0x44
    {
        bool failed = CSF_AT(uint8_t, ctx, 0) != 0;
        CSF_AT(int, ctx, 0x1c) = 0;
        if (!failed) {
            void* obj = CSF_AT(void*, ctx, 4);
            CSF_AT(uint8_t, ctx, 0x15) = 0;
            CSF_AT(uint8_t, ctx, 0x16) = 0;
            VCallWithPtr fn = (VCallWithPtr)CSF_AT(void**, obj, 0)[0x44 / 4];
            CSF_AT(uint8_t, ctx, 0) = (fn(obj, nullptr, base + 0x8e, ctx) == -1) ? 1 : 0;
        }
    }

    CSF_AT(int, ctx, 0x1c) = savedValue;

    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        VCallCtx fn = (VCallCtx)CSF_AT(void**, obj, 0)[0x7c / 4];
        int result = fn(obj, nullptr, ctx);
        if ((uint8_t)result != 0) {
            CSF_AT(uint8_t, ctx, 0) = 0;
            return result;
        }
    }
    CSF_AT(uint8_t, ctx, 0) = 1;
    return 1;
}

HOOK(0x004DAD60, AiAutonomous_EvaluateConditions, "auto");
