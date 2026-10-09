#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(unsigned int, __cdecl, ApplyGlobalFloatValue, 0x00525250, ());

static void __fastcall S_00452280_UpdateTimedEvent(
    S_00452280* self, void* /*edx*/, float deltaTime)
{
    if (self->field_0120 == -1.0f)
        return;

    const float elapsed = deltaTime + self->field_0120;
    self->field_0120 = elapsed;

    if (elapsed >= self->field_0124)
    {
        alignas(4) uint8_t record[0x34]{};

        CSF_AT(void*, record, 0x08) = CSF_VTBL(0x0082D448);
        CSF_AT(void*, record, 0x14) = CSF_VTBL(0x0082D448);
        CSF_AT(int32_t, record, 0x0C) = -1;
        CSF_AT(int32_t, record, 0x10) = -1;
        CSF_AT(int32_t, record, 0x18) = -1;
        CSF_AT(int32_t, record, 0x1C) = -1;
        CSF_AT(void*, record, 0x00) = CSF_VTBL(0x0082D478);
        CSF_AT(int32_t, record, 0x04) = 39;
        std::memset(record + 0x20, 0, 16);
        CSF_AT(uint8_t, record, 0x30) = 0;

        void* globalObject =
            reinterpret_cast<void*>(GetGlobalObjectField184());
        DispatchRecordToGlobalObject(globalObject, nullptr, record, 0);

        self->field_0120 = -1.0f;
        Get_008b0678();
        ApplyGlobalFloatValue();
    }
}

HOOK(0x00452280, S_00452280_UpdateTimedEvent, "auto");
