#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __cdecl, WeaponFirearms_GetEffectStateId, 0x00478880, (int a1));
CSF_FUNC(void, __fastcall, C_00834198_SetField0198, 0x0049CF30, (C_00834198* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(C_00833908*, __fastcall, C_0085e7b8_LookupStateEntry, 0x004E9540, (C_0085e7b8* self, void* , int a2, int a3, int a4, int a5));

static int __fastcall C_008317c8_InitializeEffect(
    C_008317c8* self, void* /*edx*/, char suppressActivation)
{
    int result;
    if (static_cast<int32_t>(self->field_006c) >= 0)
    {
        result = CHandleTable_Resolve(
            g_pCurrentScene, nullptr,
            reinterpret_cast<int>(reinterpret_cast<uint8_t*>(self) + 0x64));
        if (result != 0)
            return result;
    }

    result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        self->vtbl[0x48 / 4])(self, nullptr);
    if (result != 3)
        return result;

    result = WeaponFirearms_GetEffectStateId(
        CSF_AT(int, reinterpret_cast<void*>(self->field_0040), 0x74));
    if (result == -1)
        return result;

    const int effectParameter = result == 21 ? 1084227584 : 0;
    auto* manager = reinterpret_cast<C_0085e7b8*>(GetValue8B0554());
    auto* effect = reinterpret_cast<C_00834198*>(
        C_0085e7b8_LookupStateEntry(
            manager, nullptr, result, -1, 0, effectParameter));

    self->field_0068 = effect->field_0020;
    self->field_006c = effect->field_0024;

    auto* ownerHandle = reinterpret_cast<uint8_t*>(self->field_0004) + 28;
    alignas(4) uint8_t context[0x2C] = {};
    CSF_AT(int, context, 0x18) = -1;
    CSF_AT(void**, context, 0x1C) = CSF_VTBL(0x0082D448);
    CSF_AT(uint32_t, context, 0x20) = CSF_AT(uint32_t, ownerHandle, 4);
    CSF_AT(uint32_t, context, 0x24) = CSF_AT(uint32_t, ownerHandle, 8);
    CSF_AT(int, context, 0x28) = effectParameter;

    int managerAddress;
    void* source;
    if (self->field_0070)
    {
        if (self->field_0074)
        {
            managerAddress = GetValue8B0554();
            source = reinterpret_cast<void*>(self->field_0074);
        }
        else
        {
            managerAddress = GetValue8B0554();
            source = reinterpret_cast<void*>(self->field_0070);
        }
    }
    else
    {
        managerAddress = GetValue8B0554();
        source = self;
    }

    void* registry = CSF_AT(void*, reinterpret_cast<void*>(managerAddress), 0x1FC0);
    auto* valuePointer = reinterpret_cast<uint32_t*>(
        Handle_GetValuePointer(source, nullptr));
    CSF_AT(int, context, 0x18) =
        reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, registry, 0)[0xC / 4])(
                registry, nullptr, *valuePointer);

    void* embeddedHandle = &effect->field_000c;
    reinterpret_cast<void (__fastcall*)(void*, void*, void*)>(
        CSF_AT(void**, embeddedHandle, 0)[0x8 / 4])(
            embeddedHandle, nullptr, context);

    C_00834198_SetField0198(effect, nullptr);

    void* relatedObject = reinterpret_cast<void*>(effect->field_0034);
    if (relatedObject && !suppressActivation)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, relatedObject, 0)[0xA4 / 4])(
                relatedObject, nullptr, 0);
    }

    void* effectObject = reinterpret_cast<void*>(effect->vGetField016c());
    return reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, effectObject, 0)[0x3C / 4])(
            effectObject, nullptr, 0);
}

HOOK(0x004794C0, C_008317c8_InitializeEffect, "auto");
