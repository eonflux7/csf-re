#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

typedef bool (__fastcall *WaterVCallBoolFn)(void* obj, void* edx, int arg0, void* ctx);
typedef int (__fastcall *WaterVCallIntFn)(void* obj, void* edx, void* arg, void* ctx);
typedef int (__fastcall *WaterVCallInt1Fn)(void* obj, void* edx, void* ctx);

static int __fastcall Water_RunPassChain(void* self, void* /*edx*/, void* ctx)
{
    uint8_t* c = reinterpret_cast<uint8_t*>(ctx);

    bool ok = false;
    if (CSF_AT(uint8_t, c, 0) == 0) {
        void* obj = CSF_AT(void*, c, 4);
        void** vtbl = CSF_AT(void**, obj, 0);
        bool r = reinterpret_cast<WaterVCallBoolFn>(vtbl[0x80 / 4])(obj, nullptr, 0, ctx);
        ok = !r;
    }
    CSF_AT(uint8_t, c, 0) = ok ? 1 : 0;

    int savedValue = CSF_AT(int, c, 0x1c);
    CSF_AT(int, c, 0x1c) = 0;

    if (CSF_AT(uint8_t, c, 0) == 0) {
        void* obj = CSF_AT(void*, c, 4);
        CSF_AT(uint8_t, c, 0x15) = 0;
        CSF_AT(uint8_t, c, 0x16) = 0;
        void** vtbl = CSF_AT(void**, obj, 0);
        int r = reinterpret_cast<WaterVCallIntFn>(vtbl[0x28 / 4])(obj, nullptr, (uint8_t*)self + 4, ctx);
        CSF_AT(uint8_t, c, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, c, 0x1c) = 0;

    if (CSF_AT(uint8_t, c, 0) == 0) {
        void* obj = CSF_AT(void*, c, 4);
        CSF_AT(uint8_t, c, 0x15) = 0;
        CSF_AT(uint8_t, c, 0x16) = 0;
        void** vtbl = CSF_AT(void**, obj, 0);
        int r = reinterpret_cast<WaterVCallIntFn>(vtbl[0x14 / 4])(obj, nullptr, (uint8_t*)self + 0x1c, ctx);
        CSF_AT(uint8_t, c, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, c, 0x1c) = savedValue;

    if (CSF_AT(uint8_t, c, 0) == 0) {
        void* obj = CSF_AT(void*, c, 4);
        void** vtbl = CSF_AT(void**, obj, 0);
        int r = reinterpret_cast<WaterVCallInt1Fn>(vtbl[0x7c / 4])(obj, nullptr, ctx);
        if ((r & 0xFF) != 0) {
            CSF_AT(uint8_t, c, 0) = 0;
            return r;
        }
    }

    CSF_AT(uint8_t, c, 0) = 1;
    return 1;
}

HOOK(0x70CAB0, Water_RunPassChain, "auto");
