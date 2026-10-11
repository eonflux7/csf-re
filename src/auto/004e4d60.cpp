#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, StateRecord_Reset, 0x00401870, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(int, __fastcall, StateOwner_GetProcessingStatus, 0x0048E210, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e7d8_ProcessBaseRecord, 0x004E1180, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_0083e7d8_UpdateRecordState, 0x004E4540, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e7d8_HandleCounterExhaustion, 0x004E4CE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(float, __fastcall, MenuSystem_GetTransitionScale_50D8B0, 0x0050D8B0, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083e7d8_ProcessStateRecord(
    C_0083e7d8* self, void* /*edx*/, uint32_t* record)
{
    int result = StateOwner_GetProcessingStatus(
        reinterpret_cast<void*>(self->field_000c), nullptr);
    if (static_cast<uint8_t>(result) != 0)
        return result;

    result = self->vSlotB0();
    if (static_cast<uint8_t>(result) != 0)
        return result;

    const bool accepted =
        reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t, uint32_t, uint32_t*)>(
            self->vtbl[0x90 / 4])(self, nullptr, record[3], record[9], record);

    if (accepted)
    {
        self->field_002c = record[6];
        CSF_AT(uint32_t, self, 0x30) = record[7];
        CSF_AT(uint32_t, self, 0x34) = record[8];
        C_0083e7d8_UpdateRecordState(self, nullptr);

        uint32_t decrement = record[4];
        if (static_cast<int32_t>(record[2]) >= 0)
        {
            const int resolved = CHandleTable_Resolve(
                g_pCurrentScene, nullptr, reinterpret_cast<int>(record));
            if (resolved &&
                CSF_AT(uint32_t, reinterpret_cast<void*>(resolved), 0x50) == 1)
            {
                const int globalState = GetGlobalState_50D190();
                const float scale = MenuSystem_GetTransitionScale_50D8B0(
                    reinterpret_cast<void*>(globalState), nullptr);
                decrement = static_cast<uint32_t>(static_cast<int64_t>(
                    scale * static_cast<double>(static_cast<int32_t>(record[4]))));
            }
        }

        const uint32_t recordType = record[5];
        if (recordType == 17)
        {
            if (self->field_0018 != 0)
            {
                const uint32_t remaining = self->field_0018 - decrement;
                self->field_0018 = remaining;
                if (static_cast<int32_t>(remaining) <= 0)
                {
                    void* controller = CSF_AT(
                        void*, reinterpret_cast<void*>(self->field_000c), 0x4C);
                    reinterpret_cast<void(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, controller, 0)[0x58 / 4])(controller, nullptr);
                    reinterpret_cast<void(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, controller, 0)[0x5C / 4])(controller, nullptr);

                    C_0083e7d8_HandleCounterExhaustion(
                        self, nullptr, reinterpret_cast<int>(record));
                    self->field_0018 = 0;
                    return C_0083e7d8_ProcessBaseRecord(self, nullptr, record);
                }
            }
        }
        else if (recordType == 20)
        {
            if (self->field_0024 != 0)
            {
                const uint32_t remaining = self->field_0024 - decrement;
                self->field_0024 = remaining;
                if (static_cast<int32_t>(remaining) <= 0)
                {
                    void* controller = CSF_AT(
                        void*, reinterpret_cast<void*>(self->field_000c), 0x4C);
                    reinterpret_cast<void(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, controller, 0)[0x58 / 4])(controller, nullptr);
                    reinterpret_cast<void(__fastcall*)(void*, void*)>(
                        CSF_AT(void**, controller, 0)[0x5C / 4])(controller, nullptr);

                    C_0083e7d8_HandleCounterExhaustion(
                        self, nullptr, reinterpret_cast<int>(record));
                    self->field_0024 = 0;
                }
            }

            alignas(8) uint8_t notificationBytes[0x34];
            C_0082d478& notification =
                *reinterpret_cast<C_0082d478*>(notificationBytes);
            StateRecord_Reset(&notification, nullptr, 42);

            void* owner = reinterpret_cast<void*>(self->field_000c);
            notification.field_000c = CSF_AT(uint32_t, owner, 0x20);
            notification.field_0010 = CSF_AT(uint32_t, owner, 0x24);

            const int dispatcher = GetGlobalObjectField184();
            DispatchRecordToGlobalObject(
                reinterpret_cast<void*>(dispatcher), nullptr, &notification, 0);
        }
    }

    return C_0083e7d8_ProcessBaseRecord(self, nullptr, record);
}

HOOK(0x004E4D60, C_0083e7d8_ProcessStateRecord, "auto");
