#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, IsRecordSlotActive, 0x00407F40, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __cdecl, AllocateSubobjectHandle, 0x0040A430, (int a1));
CSF_FUNC(char, __fastcall, VehicleBase_Finish, 0x00418B40, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));

// Checks one record slot on the subobject; notifies the target when it is unavailable.
static char CheckRecordSlot(C_0082e350* self, int slot)
{
    if (IsRecordSlotActive(reinterpret_cast<void*>(self->field_0030), nullptr, slot)) {
        return 1;
    }
    HandleTargetUnavailable(reinterpret_cast<void*>(self->field_0004), nullptr);
    return 0;
}

static char __fastcall C_0082e350_InitRecords(C_0082e350* self, void* /*edx*/, int a2, int a3)
{
    self->field_0004 = a2;
    uint32_t* handle = AllocateSubobjectHandle(13);
    self->field_0030 = reinterpret_cast<uint32_t>(handle);

    char ok = ((char(__fastcall*)(void*, void*, int))CSF_AT(void**, handle, 0)[2])(handle, nullptr, a2) & 1;

    ok &= CheckRecordSlot(self, 0);
    ok &= CheckRecordSlot(self, 1);
    ok &= CheckRecordSlot(self, 7);

    int slot = 8;
    int remaining = 5;
    do {
        ok &= CheckRecordSlot(self, slot - 6);
        ok &= CheckRecordSlot(self, slot);
        ++slot;
        --remaining;
    } while (remaining);

    char result = ok;
    if (result) {
        int offset = 0;
        uint32_t index = 0;
        if (self->field_004c) {
            do {
                int entryA = GetTableEntryDwordByIndex(reinterpret_cast<void*>(self->field_0030), nullptr,
                                                       CSF_AT(int, reinterpret_cast<void*>(offset + self->field_0044), 0));
                int entryB = GetTableEntryDwordByIndex(reinterpret_cast<void*>(self->field_0030), nullptr,
                                                       CSF_AT(int, reinterpret_cast<void*>(self->field_0034 + offset), 0));
                float* dst1 = reinterpret_cast<float*>(offset + self->field_0044 + 4);
                void* objA = reinterpret_cast<void*>(entryA);
                uint8_t* r1 = ((uint8_t*(__fastcall*)(void*, void*))CSF_AT(void**, objA, 0)[6])(objA, nullptr);
                *dst1 = CSF_AT(float, r1, 12) + *dst1;

                void* objB = reinterpret_cast<void*>(entryB);
                float* dst2 = reinterpret_cast<float*>(self->field_0034 + offset + 4);
                uint8_t* r2 = ((uint8_t*(__fastcall*)(void*, void*))CSF_AT(void**, objB, 0)[6])(objB, nullptr);
                *dst2 = CSF_AT(float, r2, 12) + *dst2;

                offset += 32;
                ++index;
            } while (index < self->field_004c);
        }
    }

    char base = VehicleBase_Finish(self, nullptr, a2, a3);
    return base & result;
}

HOOK(0x0041C0A0, C_0082e350_InitRecords, "auto");
