#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, Vehicle_IsRecordSlotActive, 0x00407F40, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Vehicle_GetTableEntryByIndex, 0x004084C0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __cdecl, Vehicle_AllocateSubobjectHandle, 0x0040A430, (int a1));
CSF_FUNC(char, __fastcall, Vehicle_BaseInit, 0x00418B40, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, Vehicle_SetupSlotA, 0x00419A00, (void* self, void* edx, int a2, int a3, float a4));
CSF_FUNC(int, __fastcall, Vehicle_SetupSlotB, 0x00419A80, (void* self, void* edx, int a2, int a3, float a4));
CSF_FUNC(int, __fastcall, HandleTargetUnavailable, 0x0048FE90, (void* self, void* edx));

static char __fastcall C_0082e280_Initialize(C_0082e280* self, void* /*edx*/, int a2, int a3)
{
    self->field_0004 = a2;

    uint32_t* handle = Vehicle_AllocateSubobjectHandle(6);
    self->field_0030 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(handle));

    char ok = static_cast<char>(
        ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, handle, 0)[2])(handle, nullptr, a2) & 1);

    void* sub = reinterpret_cast<void*>(self->field_0030);
    if (Vehicle_IsRecordSlotActive(sub, nullptr, 2)) {
        sub = reinterpret_cast<void*>(self->field_0030);
        if (Vehicle_IsRecordSlotActive(sub, nullptr, 5)) {
            Vehicle_SetupSlotA(self, nullptr, 2, 0, 20.0f);
            Vehicle_SetupSlotB(self, nullptr, 5, 0, 20.0f);
        }
    }

    static const int kSlots[4] = { 0, 3, 1, 4 };
    for (int i = 0; i < 4; ++i) {
        char active;
        if (Vehicle_IsRecordSlotActive(reinterpret_cast<void*>(self->field_0030), nullptr, kSlots[i])) {
            active = 1;
        } else {
            HandleTargetUnavailable(reinterpret_cast<void*>(self->field_0004), nullptr);
            active = 0;
        }
        ok = static_cast<char>(ok & active);
    }

    if (ok) {
        uint32_t index = 0;
        if (self->field_004c != 0) {
            uint32_t off = 0;
            do {
                int entryA = Vehicle_GetTableEntryByIndex(
                    reinterpret_cast<void*>(self->field_0030), nullptr,
                    static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0044), off)));
                int entryB = Vehicle_GetTableEntryByIndex(
                    reinterpret_cast<void*>(self->field_0030), nullptr,
                    static_cast<int>(CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0034), off)));

                void* objA = reinterpret_cast<void*>(entryA);
                uint8_t* resA = ((uint8_t*(__fastcall*)(void*, void*))CSF_AT(void**, objA, 0)[6])(objA, nullptr);
                CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0044), off + 4) =
                    CSF_AT(uint32_t, resA, 0xC);

                void* objB = reinterpret_cast<void*>(entryB);
                uint8_t* resB = ((uint8_t*(__fastcall*)(void*, void*))CSF_AT(void**, objB, 0)[6])(objB, nullptr);
                CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0034), off + 4) =
                    CSF_AT(uint32_t, resB, 0xC);

                off += 32;
                ++index;
            } while (index < self->field_004c);
        }
    }

    char base = Vehicle_BaseInit(self, nullptr, a2, a3);
    return static_cast<char>(base & ok);
}

HOOK(0x0041B710, C_0082e280_Initialize, "auto");
