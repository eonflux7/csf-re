#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiAttackTasks_FindSceneEntryIndex, 0x00770610, (void* self, void* edx, unsigned int a2));
CSF_FUNC(char*, __fastcall, GrowTargetActionBuffer, 0x004EF9E0, (void* self, void* edx, int a2));

static int __fastcall S_004ef780_DispatchTargetAction(S_004ef780* self, void* /*edx*/, int a2, S_004ef780* a3, int a4, int a5)
{
    int result = AiAttackTasks_FindSceneEntryIndex(self, nullptr, static_cast<unsigned int>(reinterpret_cast<uintptr_t>(a3)));
    if (result != -1)
    {
        if (self->field_0024 >= self->field_0020)
        {
            GrowTargetActionBuffer(&self->field_001c, nullptr, CSF_AT(int, self, 0x28));
        }

        uint32_t index = self->field_0024;
        uint8_t* entry = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(self->field_001c)) + 32u * index;
        self->field_0024 = index + 1;

        CSF_AT(int, entry, 0x00) = a2;
        CSF_AT(S_004ef780*, entry, 0x04) = a3;
        CSF_AT(int, entry, 0x0C) = -1;
        CSF_AT(int, entry, 0x10) = -1;
        CSF_AT(int, entry, 0x14) = a4;
        CSF_AT(uint8_t, entry, 0x18) = 0;
        CSF_AT(int, entry, 0x1C) = a5;

        result = static_cast<int>(reinterpret_cast<uintptr_t>(entry));
    }
    return result;
}

HOOK(0x004EFCA0, S_004ef780_DispatchTargetAction, "auto");
