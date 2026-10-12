#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, C_008319c0_GetTotalCapacity, 0x00476AB0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, C_008319c0_GetRecordScalarValue, 0x00476B20, (C_008319c0* self, void*));
CSF_FUNC(int, __cdecl, Get_008b0678, 0x00525130, ());
CSF_FUNC(void, __fastcall, C_00833908_SendCharacterEvent, 0x00525B30, (C_00833908* self, void* , int character, int eventType));
CSF_FUNC(int, __cdecl, GetGlobalValue_8BC1E0, 0x00756A10, ());
CSF_FUNC(int, __cdecl, CharActionFsm_CheckSubObjectState, 0x00761330, (int a1));

CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(float, g_ProgressScale, 0x0082E03C);

static void __fastcall CPlayerCharacter_RefreshState(
    CPlayerCharacter* self, void* /*edx*/)
{
    const uint32_t flags = self->field_02e0;
    if ((flags & 1) != 0 && (flags & 4) != 0)
    {
        auto* eventManager =
            reinterpret_cast<C_00833908*>(Get_008b0678());
        C_00833908_SendCharacterEvent(
            eventManager, nullptr, reinterpret_cast<int>(self), 101);
    }

    void* characterState = reinterpret_cast<void*>(self->field_0038);
    void* settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    const int maximumValue =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, characterState, 0)[0x84 / 4])(
                characterState, nullptr);
    const int valueThreshold = static_cast<int>(static_cast<int64_t>(
        static_cast<float>(maximumValue) * g_ProgressScale *
        CSF_AT(float, settings, 0x950)));

    characterState = reinterpret_cast<void*>(self->field_0038);
    const int currentValue =
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, characterState, 0)[0xA0 / 4])(
                characterState, nullptr);
    if (currentValue < valueThreshold)
    {
        auto* eventManager =
            reinterpret_cast<C_00833908*>(Get_008b0678());
        C_00833908_SendCharacterEvent(
            eventManager, nullptr, reinterpret_cast<int>(self), 106);
    }

    void* equipment = reinterpret_cast<void*>(self->field_0044);
    if (((C_008319c0*(__fastcall*)(void*, void*))
            CSF_AT(void**, equipment, 0)[0xDC / 4])(
                equipment, nullptr))
    {
        C_008319c0* record =
            ((C_008319c0*(__fastcall*)(void*, void*))
                CSF_AT(void**, equipment, 0)[0xDC / 4])(
                    equipment, nullptr);
        const int subObject = C_008319c0_GetSubObjectField74(record, nullptr);
        if (static_cast<uint8_t>(CharActionFsm_CheckSubObjectState(subObject)))
        {
            settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
            const int capacity = C_008319c0_GetTotalCapacity(record, nullptr);
            const int capacityThreshold = static_cast<int>(static_cast<int64_t>(
                static_cast<float>(capacity) * g_ProgressScale *
                CSF_AT(float, settings, 0x94C)));

            if (C_008319c0_GetRecordScalarValue(record, nullptr) < capacityThreshold)
            {
                auto* eventManager =
                    reinterpret_cast<C_00833908*>(Get_008b0678());
                C_00833908_SendCharacterEvent(
                    eventManager, nullptr, reinterpret_cast<int>(self), 105);
            }
        }
    }

    settings = reinterpret_cast<void*>(GetGlobalValue_8BC1E0());
    if (static_cast<double>(self->field_03e4) >
        CSF_AT(float, settings, 0x954))
    {
        auto* eventManager =
            reinterpret_cast<C_00833908*>(Get_008b0678());
        C_00833908_SendCharacterEvent(
            eventManager, nullptr, reinterpret_cast<int>(self), 110);
    }
    self->field_03e4 = 0;
}

HOOK(0x004947A0, CPlayerCharacter_RefreshState, "auto");
