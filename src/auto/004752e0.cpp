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

CSF_FUNC(int, __fastcall, C_0082d478_BaseSerialize, 0x00474BD0, (void* self, void* edx, int a2));

static int __fastcall C_00831678_SerializeFieldGroup(C_00831678* self, void* /*edx*/, uint8_t* ctx)
{
    // Initial flag: set when no error yet and the stream's slot 0x80 returns false
    bool failed = false;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        bool r = ((bool(__fastcall*)(void*, void*, int, void*))CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, ctx);
        failed = !r;
    }
    int savedField = CSF_AT(int, ctx, 0x1C);
    CSF_AT(uint8_t, ctx, 0) = failed ? 1 : 0;

    C_0082d478_BaseSerialize(self, nullptr, (int)(intptr_t)ctx);

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x38 / 4])(stream, nullptr, &self->field_0034, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x38 / 4])(stream, nullptr, &self->field_0040, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x58 / 4])(stream, nullptr, &self->field_004c, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x58 / 4])(stream, nullptr, &self->field_0050, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = 0;
    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        CSF_AT(uint8_t, ctx, 0x15) = 0;
        CSF_AT(uint8_t, ctx, 0x16) = 0;
        int r = ((int(__fastcall*)(void*, void*, void*, void*))CSF_AT(void**, stream, 0)[0x54 / 4])(stream, nullptr, &self->field_0054, ctx);
        CSF_AT(uint8_t, ctx, 0) = (r == -1) ? 1 : 0;
    }

    CSF_AT(int, ctx, 0x1C) = savedField;

    if (CSF_AT(uint8_t, ctx, 0) == 0) {
        void* stream = CSF_AT(void*, ctx, 4);
        int result = ((int(__fastcall*)(void*, void*, void*))CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, ctx);
        if ((uint8_t)result != 0) {
            CSF_AT(uint8_t, ctx, 0) = 0;
            return result;
        }
    }
    CSF_AT(uint8_t, ctx, 0) = 1;
    return 1;
}

HOOK(0x004752E0, C_00831678_SerializeFieldGroup, "auto");
