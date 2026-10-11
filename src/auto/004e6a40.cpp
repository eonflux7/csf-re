#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CheckAndProcessField_4E6430, 0x004E6430, (void* self, void* edx, void* a2));

typedef unsigned char(__fastcall* VCallBoolFn)(void* self, void* edx, int a, void* ctx);
typedef int(__fastcall* VCallPtrFn)(void* self, void* edx, void* field, void* ctx);
typedef int(__fastcall* VCallCtxFn)(void* self, void* edx, void* ctx);

static int __fastcall C_0083124c_ProcessFields(C_0083124c* self, void* /*edx*/, int a2)
{
    uint8_t* ctx = reinterpret_cast<uint8_t*>(a2);
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    int savedValue = CSF_AT(int, ctx, 0x1C);

    bool v4 = false;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        unsigned char r = ((VCallBoolFn)CSF_AT(void**, obj, 0)[0x80 / 4])(obj, nullptr, 0, ctx);
        v4 = (r == 0);
    }
    CSF_AT(uint8_t, ctx, 0) = v4 ? 1 : 0;
    CSF_AT(int, ctx, 0x1C) = 0;
    if (!v4) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((VCallPtrFn)CSF_AT(void**, obj, 0)[0x58 / 4])(obj, nullptr, base + 4, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((VCallPtrFn)CSF_AT(void**, obj, 0)[0x58 / 4])(obj, nullptr, base + 8, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        CheckAndProcessField_4E6430(base + 0xC, nullptr, ctx);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((VCallPtrFn)CSF_AT(void**, obj, 0)[0x38 / 4])(obj, nullptr, base + 0x20, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((VCallPtrFn)CSF_AT(void**, obj, 0)[0x54 / 4])(obj, nullptr, base + 0x2C, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((VCallPtrFn)CSF_AT(void**, obj, 0)[0x54 / 4])(obj, nullptr, base + 0x30, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = savedValue;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        int result = ((VCallCtxFn)CSF_AT(void**, obj, 0)[0x7C / 4])(obj, nullptr, ctx);
        if ((uint8_t)result != 0) {
            CSF_AT(uint8_t, ctx, 0) = 0;
            return result;
        }
    }
    CSF_AT(uint8_t, ctx, 0) = 1;
    return 1;
}

HOOK(0x004E6A40, C_0083124c_ProcessFields, "auto");
