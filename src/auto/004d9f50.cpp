#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AiAttackTasks_ResetSlot, 0x0047BCB0, (int owner, S_00691860* slot));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

static int __fastcall S_0041f0a0_SerializeFields(S_0041f0a0* self, void* /*edx*/, S_0041f0a0* archive)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    S_00691860* ar = reinterpret_cast<S_00691860*>(archive);

    uint32_t savedField = CSF_AT(uint32_t, archive, 0x1C);

    uint8_t isReading;
    if (CSF_AT(uint8_t, archive, 0) != 0) {
        isReading = 0;
    } else {
        void* stream = CSF_AT(void*, archive, 4);
        void** vtbl = CSF_AT(void**, stream, 0);
        bool r = ((bool(__fastcall*)(void*, void*, int, void*))vtbl[0x80 / 4])(stream, nullptr, 0, archive);
        isReading = r ? 0 : 1;
    }
    CSF_AT(uint8_t, archive, 0) = isReading;

    Archive_SerializeBaseFields(self, nullptr, reinterpret_cast<int>(archive));

    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (CSF_AT(uint8_t, archive, 0) == 0) {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(reinterpret_cast<int>(base + 0x74), ar);
    }

    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (CSF_AT(uint8_t, archive, 0) == 0) {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(base + 0x60), ar);
    }

    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (CSF_AT(uint8_t, archive, 0) == 0) {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(base + 0x10), ar);
    }

    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (CSF_AT(uint8_t, archive, 0) == 0) {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(base + 0x4C), ar);
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedField;

    if (CSF_AT(uint8_t, archive, 0) == 0) {
        void* stream = CSF_AT(void*, archive, 4);
        void** vtbl = CSF_AT(void**, stream, 0);
        int result = ((int(__fastcall*)(void*, void*, void*))vtbl[0x7C / 4])(stream, nullptr, archive);
        if ((uint8_t)result != 0) {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }
    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x004D9F50, S_0041f0a0_SerializeFields, "auto");
