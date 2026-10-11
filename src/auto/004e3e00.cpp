#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(void*, __fastcall, InitializePositionRecord_4460A0, 0x004460A0, (void* self, void* , int identifier, const void* source, const void* position, int parameter));
CSF_FUNC(uint32_t*, __fastcall, S_00445eb0_SubmitSceneRequest, 0x00446F10, (S_00445eb0* self, void* , int requestAddress));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_48E210, 0x0048E210, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4E1120, 0x004E1120, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_4E1180, 0x004E1180, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(char, __fastcall, sub_4E2FD0, 0x004E2FD0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(char, __fastcall, sub_4E3670, 0x004E3670, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_4E3DA0, 0x004E3DA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float, __fastcall, MenuSystem_GetTransitionScale_50D8B0, 0x0050D8B0, (void* self, void*));
CSF_FUNC(int, __cdecl, IsAlternateControlKind, 0x00755860, (int kind));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());

static int __fastcall C_0083e248_ApplyDamage(
    C_0083e248* self, void* /*edx*/, int eventAddress)
{
    auto* event = reinterpret_cast<uint32_t*>(eventAddress);
    int eventKind = static_cast<int32_t>(event[5]);
    int result = static_cast<int32_t>(self->field_0008);
    if (result <= 0)
        return result;

    result = sub_48E210(reinterpret_cast<void*>(self->field_000c), nullptr);
    if (static_cast<uint8_t>(result) != 0)
        return result;

    switch (event[3])
    {
    case 16:
        self->field_0044 = 1;
        break;
    case 18:
        self->field_0044 = 2;
        break;
    default:
        self->field_0044 = 0;
        break;
    }

    self->field_0010 = event[9];
    int damage = static_cast<int32_t>(event[4]);
    if (static_cast<int32_t>(event[2]) >= 0)
    {
        int sourceAddress = CHandleTable_Resolve(
            g_pCurrentScene, nullptr, eventAddress);
        if (sourceAddress != 0 &&
            CSF_AT(int, reinterpret_cast<void*>(sourceAddress), 0x50) == 1)
        {
            int stateAddress = GetGlobalState_50D190();
            float scale = MenuSystem_GetTransitionScale_50D8B0(
                reinterpret_cast<void*>(stateAddress), nullptr);
            damage = static_cast<int32_t>(static_cast<int64_t>(
                scale * static_cast<double>(static_cast<int32_t>(event[4]))));
        }
    }

    switch (eventKind)
    {
    case 1:
        self->field_0008 = 0;
        break;
    case 4:
        if (event[3] == 10)
            self->field_0008 = 0;
        else
            self->field_0008 -= damage;
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        self->field_0008 += damage / -2;
        break;
    default:
        self->field_0008 -= damage;
        break;
    }

    int parameter;
    if (event[9] == 1)
    {
        int globalsAddress = GetGlobalValue_8BC1E0();
        parameter = CSF_AT(int, reinterpret_cast<void*>(globalsAddress), 0xA84);
    }
    else
    {
        int globalsAddress = GetGlobalValue_8BC1E0();
        parameter = CSF_AT(int, reinterpret_cast<void*>(globalsAddress), 0xA80);
    }

    auto* owner = reinterpret_cast<C_008342e8*>(self->field_000c);
    const void* source = &owner->field_001c;
    const void* position = GetObjectPositionVector(owner, nullptr);
    alignas(8) uint8_t requestBytes[0x28];
    InitializePositionRecord_4460A0(
        requestBytes, nullptr, 10, source, position, parameter);
    S_00445eb0_SubmitSceneRequest(
        CSF_AT(S_00445eb0*, g_pCurrentScene, 0xB0),
        nullptr, reinterpret_cast<int>(requestBytes));

    sub_4E3DA0(self, nullptr, eventAddress);

    if (static_cast<uint8_t>(IsAlternateControlKind(
            CSF_AT(int, reinterpret_cast<void*>(self->field_000c), 0x50))) != 0)
    {
        if (static_cast<int32_t>(self->field_0008) <= 0)
        {
            sub_4E2FD0(self, nullptr, eventAddress, eventAddress);
            return sub_4E1180(self, nullptr, event);
        }

        int threshold = sub_4E1120(self, nullptr);
        if (static_cast<int32_t>(self->field_0008) < threshold)
        {
            sub_4E3670(self, nullptr, eventAddress);
            return sub_4E1180(self, nullptr, event);
        }
    }

    if (static_cast<int32_t>(self->field_0008) <= 0)
    {
        sub_4E2FD0(self, nullptr, eventAddress, eventAddress);
        return sub_4E1180(self, nullptr, event);
    }

    eventKind = static_cast<int32_t>(event[5]);
    void* control = reinterpret_cast<uint8_t*>(self->field_000c) + 4;
    reinterpret_cast<void(__fastcall*)(void*, void*, int*)>(
        CSF_AT(void**, control, 0)[0x28 / 4])(
            control, nullptr, &eventKind);
    return sub_4E1180(self, nullptr, event);
}

HOOK(0x004E3E00, C_0083e248_ApplyDamage, "auto");
