#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CoverAttack_SetStance, 0x00492720, (void* self, void* edx, int newStance));

static int __fastcall TDarObjeto_BeginGiveObjectAction(TDarObjeto* self, void* /*edx*/)
{
    // owner->field_0018 &= ~2
    CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(self->field_0008), 0x18) &= ~2u;

    // subobj = owner->field_0014 + 4; subobj->vtbl[0x50 / 4](subobj, &flag) with flag = 1
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    uint8_t* subobj = CSF_AT(uint8_t*, owner, 0x14) + 4;
    char flag = 1;
    ((void(__fastcall*)(void*, void*, char*))CSF_AT(void**, subobj, 0)[20])(subobj, nullptr, &flag);

    // self->field_0078 = owner->field_0014->field_01D8
    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    uint8_t* ai = CSF_AT(uint8_t*, owner, 0x14);
    self->field_0078 = CSF_AT(uint32_t, ai, 0x1d8);

    // owner->field_0014->field_0030->field_001C = 1
    uint8_t* mover = CSF_AT(uint8_t*, ai, 0x30);
    CSF_AT(uint32_t, mover, 0x1c) = 1;

    // Cache the object position vector once per object
    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    if (CSF_AT(uint8_t, owner, 0x3d4) == 0)
    {
        uint8_t* aiForPos = CSF_AT(uint8_t*, owner, 0x14);
        uint32_t* positionVector = GetObjectPositionVector(aiForPos, nullptr);
        owner = reinterpret_cast<uint8_t*>(self->field_0008);
        CSF_AT(uint32_t, owner, 0x3d8) = positionVector[0];
        CSF_AT(uint32_t, owner, 0x3dc) = positionVector[1];
        CSF_AT(uint32_t, owner, 0x3e0) = positionVector[2];
        CSF_AT(uint8_t, owner, 0x3d4) = 1;
    }

    // return CoverAttack_SetStance(owner->field_0014, 3)
    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    ai = CSF_AT(uint8_t*, owner, 0x14);
    return CoverAttack_SetStance(ai, nullptr, 3);
}

HOOK(0x004D4ED0, TDarObjeto_BeginGiveObjectAction, "auto");
