#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, TaskOwner_Reset, 0x004D7340, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, SceneEntity_Activate, 0x004E51B0, (void* self, void* edx, char a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static int __fastcall AutonomousTask_StartMovementAndNotify(C_0082f9f8* self, void* /*edx*/)
{
    // owner = this->field_0008, re-read at each use as the original does
    uint8_t* owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));

    uint8_t* inner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));
    uint8_t* sub = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, inner, 0x30));
    CSF_AT(uint32_t, sub, 0x1C) = 1;

    owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
    CSF_AT(uint32_t, owner, 0x18) &= ~2u;

    owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
    void* subObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14)) + 4;
    char flag = 1;
    ((void(__fastcall*)(void*, void*, char*))CSF_AT(void**, subObj, 0)[0x50 / 4])(subObj, nullptr, &flag);

    owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
    uint8_t* entity = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));
    uint32_t* pos = GetObjectPositionVector(entity, nullptr);

    alignas(8) uint8_t argBytes[0x20];
    uint32_t* args = reinterpret_cast<uint32_t*>(argBytes);
    args[0] = 1;
    args[1] = reinterpret_cast<uint32_t>(CSF_ADDR_VT(0x0082D448));
    args[2] = CSF_AT(uint32_t, entity, 0x20);
    args[3] = CSF_AT(uint32_t, entity, 0x24);
    args[4] = pos[0];
    args[5] = pos[1];
    args[6] = pos[2];

    int global = GetValue8B0554();
    void* dispatcher = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, global, 0x1FD8)) + 0x54;
    DispatchWithArgBlock(dispatcher, nullptr, args);

    owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
    TaskOwner_Reset(owner + 0x224, nullptr);

    owner = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x8));
    if (CSF_AT(int, owner, 0x2F8) == 2) {
        uint8_t* ent = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x14));
        void* target = reinterpret_cast<void*>(CSF_AT(uint32_t, ent, 0x3C));
        SceneEntity_Activate(target, nullptr, 1);
    }

    return CharTask_StartMovement(self, nullptr, reinterpret_cast<int>(&self->field_0014));
}

HOOK(0x004D8F90, AutonomousTask_StartMovementAndNotify, "auto");
