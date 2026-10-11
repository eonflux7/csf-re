#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ResetAttackTaskState, 0x0047B220, (void* self, void*));
CSF_FUNC(int, __fastcall, AiAttackTasks_CopyTargetHandle, 0x0048C9F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

static int __fastcall C_00834fa8_ClearAttackTarget(C_00834fa8* self, void* /*edx*/)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);

    if (CSF_AT(int, base, 0x14) != 0) {
        if (CSF_AT(int, base, 0x18) != 0) {
            alignas(8) uint8_t handleBytes[12];
            CSF_AT(uint32_t, handleBytes, 0) = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
            CSF_AT(int, handleBytes, 4) = -1;
            CSF_AT(int, handleBytes, 8) = -1;

            uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, base, 0x8));
            uint8_t* ownerObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));
            void* attackTasks = reinterpret_cast<void*>(CSF_AT(uint32_t, ownerObj, 0x204));
            AiAttackTasks_CopyTargetHandle(attackTasks, nullptr, reinterpret_cast<int>(handleBytes));

            ResetAttackTaskState(reinterpret_cast<void*>(CSF_AT(uint32_t, base, 0x18)), nullptr);
        }
        uint8_t* target = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, base, 0x14));
        CSF_AT(int, target, 0x1C) = 0;
        uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, base, 0x8));
        CSF_AT(uint8_t, owner, 0x314) = 0;
        CSF_AT(int, base, 0x14) = 0;

        int result = CSF_AT(int, base, 0x14);
        if (result != 0)
            return result;
    }

    int result = CSF_AT(int, base, 0x18);
    if (result == 0) {
        uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, base, 0x8));
        void** ownerObj = reinterpret_cast<void**>(CSF_AT(uint32_t, owner, 0x14));
        ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, ownerObj, 0)[0x8C / 4])(
            ownerObj, nullptr, 0, 0, -1, 0);
        return CharTask_LookupByOffset(self, nullptr, static_cast<int>(CSF_AT(uint32_t, base, 0xC)) + 0x54);
    }
    return result;
}

HOOK(0x004D8E60, C_00834fa8_ClearAttackTarget, "auto");
