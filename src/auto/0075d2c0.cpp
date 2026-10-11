#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Virtual call signatures on the object stored at ctx+4 (thiscall -> fastcall free functions).
typedef bool (__fastcall *PreCheckFn)(void* obj, void* edx, int zero, uint8_t* ctx);
typedef int (__fastcall *VisitFn)(void* obj, void* edx, void* node, uint8_t* ctx);
typedef int (__fastcall *PostCheckFn)(void* obj, void* edx, uint8_t* ctx);

static int __fastcall WaypointGraph_VisitNodePair(S_004a9e10* self, void* /*edx*/, uint8_t* ctx)
{
    bool ok = false;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        PreCheckFn fn = (PreCheckFn)CSF_AT(void**, obj, 0)[0x80 / 4];
        ok = !fn(obj, nullptr, 0, ctx);
    }
    CSF_AT(uint8_t, ctx, 0) = ok ? 1 : 0;

    int savedField = CSF_AT(int, ctx, 0x1c);
    CSF_AT(int, ctx, 0x1c) = 0;

    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        VisitFn fn = (VisitFn)CSF_AT(void**, obj, 0)[0x48 / 4];
        int r = fn(obj, nullptr, self, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1c) = 0;

    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        VisitFn fn = (VisitFn)CSF_AT(void**, obj, 0)[0x48 / 4];
        int r = fn(obj, nullptr, (uint8_t*)self + 4, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1c) = savedField;

    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* obj = CSF_AT(void*, ctx, 4);
        PostCheckFn fn = (PostCheckFn)CSF_AT(void**, obj, 0)[0x7c / 4];
        int result = fn(obj, nullptr, ctx);
        if ((result & 0xFF) != 0) {
            CSF_AT(uint8_t, ctx, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, ctx, 0) = 1;
    return 1;
}

HOOK(0x0075D2C0, WaypointGraph_VisitNodePair, "auto");
