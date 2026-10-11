#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, TaskState_Sub41EF70, 0x0041EF70, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, TaskState_Sub421CA0, 0x00421CA0, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static void CopyVec3(void* dst, const void* src)
{
    CSF_AT(uint32_t, dst, 0) = CSF_AT(uint32_t, src, 0);
    CSF_AT(uint32_t, dst, 4) = CSF_AT(uint32_t, src, 4);
    CSF_AT(uint32_t, dst, 8) = CSF_AT(uint32_t, src, 8);
}

static uint32_t* __fastcall C_00834fa8_ResetTaskState(C_00834fa8* self, void* /*edx*/)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 8));

    if (!CSF_AT(uint8_t, owner, 0x230)) {
        void* obj14 = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
        void* sub48 = reinterpret_cast<void*>(CSF_AT(uint32_t, obj14, 0x48));
        bool r = ((bool(__fastcall*)(void*, void*, int))CSF_AT(void**, sub48, 0)[0x24 / 4])(sub48, nullptr, 1);
        if (r) {
            owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 8));
            void* o14 = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
            ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, o14, 0)[0x8C / 4])(o14, nullptr, 0, 0, -1, 0);
        }
    }

    owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 8));
    uint8_t* obj14 = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));

    uint32_t args[7];
    args[0] = 1;
    args[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    args[2] = CSF_AT(uint32_t, obj14, 0x20);
    args[3] = CSF_AT(uint32_t, obj14, 0x24);
    args[4] = CSF_AT(uint32_t, owner, 0x224);
    args[5] = CSF_AT(uint32_t, owner, 0x228);
    args[6] = CSF_AT(uint32_t, owner, 0x22C);

    int g = GetValue8B0554();
    DispatchWithArgBlock(reinterpret_cast<void*>(CSF_AT(uint32_t, g, 0x1FD8) + 0x34), nullptr, args);

    owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 8));
    if (CSF_AT(uint32_t, owner, 0x1C) == 0) {
        CopyVec3(owner + 0x278, owner + 0x224);
        owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 8));
        CopyVec3(owner + 0x284, owner + 0x224);
        owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 8));
        CopyVec3(owner + 0x290, owner + 0x224);
        owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 8));
        CSF_AT(uint8_t, owner, 0x274) = 0;
    }

    owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 8));
    TaskState_Sub41EF70(owner + 0x29C, nullptr);

    uint8_t* obj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 8));
    uint32_t pos[3];
    pos[0] = CSF_AT(uint32_t, obj, 0x218);
    pos[1] = CSF_AT(uint32_t, obj, 0x21C);
    pos[2] = CSF_AT(uint32_t, obj, 0x220);
    void* sub4c = reinterpret_cast<void*>(CSF_AT(uint32_t, CSF_AT(uint32_t, obj, 0x14), 0x4C));
    ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, sub4c, 0)[0x50 / 4])(sub4c, nullptr, pos);

    return TaskState_Sub421CA0(obj, nullptr);
}

HOOK(0x00423190, C_00834fa8_ResetTaskState, "auto");
