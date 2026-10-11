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

CSF_FUNC(int, __fastcall, VehicleWheel_BaseSerialize, 0x0041B360, (void* self, void* edx, int a2));

static int __fastcall C_0082e3b8_Process(C_0082e3b8* self, void* /*edx*/, uint8_t* ctx)
{
    typedef int (__fastcall *PtrFn)(void*, void*, void*, void*);

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

    VehicleWheel_BaseSerialize(self, nullptr, (int)(intptr_t)ctx);

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((PtrFn)CSF_AT(void**, stream, 0)[0x58 / 4])(stream, nullptr, &self->field_008c, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((PtrFn)CSF_AT(void**, stream, 0)[0x38 / 4])(stream, nullptr, &self->field_0090, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((PtrFn)CSF_AT(void**, stream, 0)[0x54 / 4])(stream, nullptr, &self->field_009c, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((PtrFn)CSF_AT(void**, stream, 0)[0x54 / 4])(stream, nullptr, &self->field_00a0, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1);
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        void* field = reinterpret_cast<uint8_t*>(self) + 0xA4;
        int r = ((PtrFn)CSF_AT(void**, stream, 0)[0x54 / 4])(stream, nullptr, field, ctx);
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

HOOK(0x0041CEC0, C_0082e3b8_Process, "auto");
