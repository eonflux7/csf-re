#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, AiAttackTasks_ResetSlot, 0x0047BCB0, (int owner, S_00691860* slot));
CSF_FUNC(int, __fastcall, Archive_SerializeBaseFields, 0x004DB4F0, (void* self, void* edx, int a2));

static int __fastcall S_0041f0a0_SerializeFields(S_0041f0a0* self, void* /*edx*/, S_0041f0a0* archive)
{
    uint8_t* selfBytes = reinterpret_cast<uint8_t*>(self);
    uint32_t savedField = CSF_AT(uint32_t, archive, 0x1C);

    // First flag: 1 only if the archive flag is clear and the stream's slot 32 returns false
    uint8_t flag = 0;
    if (CSF_AT(uint8_t, archive, 0) == 0) {
        void* stream = CSF_AT(void*, archive, 4);
        typedef int(__fastcall * Fn80)(void*, void*, int, void*);
        int r = ((Fn80)CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, archive);
        flag = ((uint8_t)r == 0) ? 1 : 0;
    }
    CSF_AT(uint8_t, archive, 0) = flag;

    Archive_SerializeBaseFields(self, nullptr, reinterpret_cast<int>(archive));

    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (CSF_AT(uint8_t, archive, 0) == 0) {
        void* stream = CSF_AT(void*, archive, 4);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        typedef int(__fastcall * Fn38)(void*, void*, void*, void*);
        int r = ((Fn38)CSF_AT(void**, stream, 0)[0x38 / 4])(stream, nullptr, selfBytes + 0x38, archive);
        CSF_AT(uint8_t, archive, 0) = (r == -1) ? 1 : 0;
    }

    bool failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed) {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(selfBytes + 0x24), reinterpret_cast<S_00691860*>(archive));
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed) {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        AiAttackTasks_ResetSlot(reinterpret_cast<int>(selfBytes + 0x10), reinterpret_cast<S_00691860*>(archive));
    }

    failed = CSF_AT(uint8_t, archive, 0) != 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedField;
    if (!failed) {
        void* stream = CSF_AT(void*, archive, 4);
        typedef int(__fastcall * Fn7C)(void*, void*, void*);
        int result = ((Fn7C)CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, archive);
        if ((uint8_t)result != 0) {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }
    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x004DA700, S_0041f0a0_SerializeFields, "auto");
