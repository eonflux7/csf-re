#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());

static int __fastcall C_00833908_DispatchRecordOnce(
    C_00833908* self, void* /*edx*/, int value)
{
    uint32_t result = self->field_0170;
    if ((result & 0x1000) == 0)
    {
        result = static_cast<uint32_t>(GetGlobalState_50D190());
        if (CSF_AT(uint8_t, reinterpret_cast<void*>(result), 0xD0))
        {
            uint32_t record[0x34 / sizeof(uint32_t)];
            record[2] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
            record[5] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
            record[6] = static_cast<uint32_t>(-1);
            record[7] = static_cast<uint32_t>(-1);
            record[3] = self->field_0020;
            record[8] = 0;
            record[9] = 0;
            record[10] = 0;
            record[12] = 0;
            record[0] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D478));
            record[1] = 9;
            record[4] = self->field_0024;
            record[11] = static_cast<uint32_t>(value);

            void* dispatcher = reinterpret_cast<void*>(GetGlobalObjectField184());
            result = DispatchRecordToGlobalObject(dispatcher, nullptr, record, 1);
        }
        self->field_0170 |= 0x1000u;
    }
    return static_cast<int>(result);
}

HOOK(0x0048FD70, C_00833908_DispatchRecordOnce, "auto");
