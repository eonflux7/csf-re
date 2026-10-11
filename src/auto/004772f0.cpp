#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(float*, __fastcall, WeaponFirearms_UpdateLinkedObject, 0x0040C8A0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(void, __fastcall, WeaponFirearms_ApplyShotEffect, 0x00476820, (void* self, void* edx, float a2));
CSF_FUNC(uint32_t*, __fastcall, WeaponFirearms_BuildShotTransform, 0x00477240, (void* self, void* edx, int a2, int a3));
CSF_FUNC(unsigned int, __fastcall, WeaponFirearms_ActivateContext, 0x006C8550, (void* self, void* edx));
CSF_FUNC(int, __fastcall, WeaponFirearms_IsContextActive, 0x006C8620, (void* self, void* edx));
CSF_FUNC(int, __cdecl, IsSubObjectFieldFlagSet, 0x00760FF0, (int a1));
CSF_FUNC(int, __cdecl, WeaponFirearms_IsAlternateWeaponType, 0x007610B0, (int a1));

static char __fastcall C_008316d8_Fire(
    C_008316d8* self, void* /*edx*/, int action)
{
    void* actionData = reinterpret_cast<void*>(action);
    const char bypassAmmo = CSF_AT(char, actionData, 0x21);
    if (!bypassAmmo && static_cast<int32_t>(self->field_001c) <= 0)
        return bypassAmmo;

    self->vSlotB0();

    int position[3] = {0, 0, 0};
    int direction[3] = {0, 0, 0};
    ((void (__fastcall*)(void*, void*, int, int*, int*))
        CSF_AT(void**, self, 0)[0xC8 / 4])(
            self, nullptr, action, position, direction);

    int shotMode = 1;
    if (CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x74) == 3)
        shotMode = 5;

    alignas(4) uint8_t shotTransform[0x34];
    WeaponFirearms_BuildShotTransform(
        shotTransform, nullptr,
        static_cast<int>(self->field_0004 + 28), action);

    self->vSlotD0(position[0], position[1], position[2],
                  direction[0], direction[1], direction[2],
                  shotMode, reinterpret_cast<int>(shotTransform));

    if (self->vSlot9C() && !CSF_AT(uint8_t, actionData, 0x21))
    {
        const bool consumesAmmo =
            !static_cast<uint8_t>(IsSubObjectFieldFlagSet(
                CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x74))) &&
            !static_cast<uint8_t>(WeaponFirearms_IsAlternateWeaponType(
                CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x74)));

        if (!CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x50)
            || consumesAmmo)
        {
            if (--self->field_004c == 0)
            {
                const uint32_t remainingAmmo = self->field_001c - 1;
                self->field_004c = self->field_0050;
                self->field_001c = remainingAmmo;
            }
        }
    }

    alignas(4) uint8_t record[0x50];
    CSF_AT(int, record, 0x0C) = -1;
    CSF_AT(int, record, 0x10) = -1;
    CSF_AT(int, record, 0x18) = -1;
    CSF_AT(int, record, 0x1C) = -1;

    void* source = reinterpret_cast<void*>(self->field_0004 + 28);
    CSF_AT(void**, record, 0x08) = CSF_VTBL(0x0082D448);
    CSF_AT(void**, record, 0x14) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, record, 0x24) = 0;
    CSF_AT(int, record, 0x34) = 0;
    CSF_AT(int, record, 0x38) = 0;
    CSF_AT(int, record, 0x3C) = 0;
    CSF_AT(int, record, 0x40) = 0;
    CSF_AT(int, record, 0x44) = 0;
    CSF_AT(int, record, 0x48) = 0;
    CSF_AT(int, record, 0x04) = 3;
    CSF_AT(int, record, 0x20) = 0;
    CSF_AT(int, record, 0x2C) = 0;
    CSF_AT(int, record, 0x28) = 0;
    CSF_AT(uint8_t, record, 0x30) = 0;
    CSF_AT(void**, record, 0x00) = CSF_VTBL(0x00831638);

    CSF_AT(uint32_t, record, 0x0C) = CSF_AT(uint32_t, source, 0x04);
    const uint32_t sourceValue = CSF_AT(uint32_t, source, 0x08);
    const uint32_t owner = self->field_000c;
    CSF_AT(uint32_t, record, 0x10) = sourceValue;
    CSF_AT(int, record, 0x34) = position[0];
    CSF_AT(int, record, 0x38) = position[1];
    CSF_AT(uint32_t, record, 0x24) = owner;
    CSF_AT(uint32_t, record, 0x4C) = owner;
    CSF_AT(int, record, 0x3C) = position[2];
    CSF_AT(int, record, 0x40) = direction[0];
    CSF_AT(int, record, 0x44) = direction[1];
    CSF_AT(int, record, 0x48) = direction[2];

    const int dispatcher = GetGlobalObjectField184();
    DispatchRecordToGlobalObject(
        reinterpret_cast<void*>(dispatcher), nullptr, record, 1);

    if (self->vGetPageCountForContexts())
    {
        const uint32_t context = self->vGetPageCountForContexts();
        if (!static_cast<uint8_t>(WeaponFirearms_IsContextActive(
                reinterpret_cast<void*>(context), nullptr)))
        {
            WeaponFirearms_ActivateContext(
                reinterpret_cast<void*>(self->vGetPageCountForContexts()), nullptr);
        }
    }

    if (CSF_AT(uint8_t, actionData, 0x23))
    {
        WeaponFirearms_ApplyShotEffect(
            self, nullptr,
            CSF_AT(float, reinterpret_cast<void*>(self->field_0040), 0x128));
    }

    if (CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x74) == 20)
    {
        void* linkedObject = reinterpret_cast<void*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0004), 0x4C));
        if (linkedObject)
            WeaponFirearms_UpdateLinkedObject(linkedObject, nullptr);
    }

    return static_cast<char>(self->vSlot80());
}

HOOK(0x004772F0, C_008316d8_Fire, "auto");
