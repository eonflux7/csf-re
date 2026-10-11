#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, StateChain_BaseProcess, 0x0041F0A0, (void* self, void* edx, void* a2));

static int __fastcall S_0041f0a0_BaseProcess(S_0041f0a0* self, void* /*edx*/, uint8_t* ctx)
{
    typedef int (__fastcall *PtrFn)(void*, void*, void*, void*);

    // The original takes the address of the vtable of S_0041f0a0 (CSF_VTBL) at the
    // call site of the base routine. Referenced here without any runtime effect.
    (void)CSF_VTBL(0x0041F0A0);

    // Initial flag: set if no error yet and the stream's slot 0x80 returns false
    uint8_t failed = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        bool r = ((bool(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, ctx);
        if (!r)
            failed = 1;
    }
    int savedField = CSF_AT(int, ctx, 0x1C);
    CSF_AT(uint8_t, ctx, 0) = failed;

    StateChain_BaseProcess(self, nullptr, ctx);

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void* field = reinterpret_cast<uint8_t*>(self) + 0x1C;
        int r = ((PtrFn)CSF_AT(void**, stream, 0)[0x58 / 4])(stream, nullptr, field, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void* field = reinterpret_cast<uint8_t*>(self) + 0x20;
        int r = ((PtrFn)CSF_AT(void**, stream, 0)[0x58 / 4])(stream, nullptr, field, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void* field = reinterpret_cast<uint8_t*>(self) + 0x24;
        int r = ((PtrFn)CSF_AT(void**, stream, 0)[0x58 / 4])(stream, nullptr, field, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void* field = reinterpret_cast<uint8_t*>(self) + 0x28;
        int r = ((PtrFn)CSF_AT(void**, stream, 0)[0x54 / 4])(stream, nullptr, field, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void* field = reinterpret_cast<uint8_t*>(self) + 0x2C;
        int r = ((PtrFn)CSF_AT(void**, stream, 0)[0x48 / 4])(stream, nullptr, field, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1);
    }

    CSF_AT(int, ctx, 0x1C) = savedField;

    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        int r = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, ctx);
        if ((uint8_t)r != 0) {
            CSF_AT(uint8_t, ctx, 0) = 0;
            return r;
        }
    }
    CSF_AT(uint8_t, ctx, 0) = 1;
    return 1;
}

HOOK(0x00420550, S_0041f0a0_BaseProcess, "auto");
