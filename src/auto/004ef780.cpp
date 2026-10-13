#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, StateRecord_Reset, 0x00401870, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalObjectField184, 0x00475470, ());
CSF_FUNC(uint32_t, __fastcall, DispatchRecordToGlobalObject, 0x004758B0, (void* self, void* edx, void* record, char flag));
CSF_FUNC(char, __stdcall, TextDatabase_TestSceneEntryCondition, 0x004EF080, (void* a1, int a2));
CSF_FUNC(char, __fastcall, TextDatabase_TestControllerCondition, 0x004EF230, (void* self, void* edx, void* a1, unsigned int a2, int a3));
CSF_FUNC(int, __fastcall, AiAttackTasks_FindSceneEntryIndex, 0x00770610, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneEntry, 0x00770690, (void* self, void* edx, int a2));

static unsigned int __fastcall S_004ef780_UpdateManagedController(
    S_004ef780* self, void* /*edx*/, int /*unused*/)
{
    unsigned int processed = 0;
    uint32_t entryOffset = 0;

    if (self->field_0024 == 0)
        return 0;

    do
    {
        auto* entry = reinterpret_cast<uint8_t*>(self->field_001c) + entryOffset;
        char currentState = 0;

        switch (CSF_AT(int, entry, 0x00))
        {
        case 10:
        {
            const int index = AiAttackTasks_FindSceneEntryIndex(
                self, nullptr, CSF_AT(uint32_t, entry, 0x04));
            if (index != -1)
            {
                void* condition = entry + 0x08;
                const int sceneEntry = SceneLoader_GetSceneEntry(self, nullptr, index);
                currentState = TextDatabase_TestSceneEntryCondition(
                    reinterpret_cast<void*>(sceneEntry),
                    reinterpret_cast<int>(condition));

                if (currentState && !CSF_AT(uint8_t, entry, 0x18))
                {
                    alignas(8) uint8_t recordBytes[0x3C];
                    auto& record = *reinterpret_cast<C_0082d478*>(recordBytes);
                    StateRecord_Reset(&record, nullptr, 10);
                    record.field_000c = CSF_AT(uint32_t, entry, 0x0C);
                    record.field_0010 = CSF_AT(uint32_t, entry, 0x10);
                    record.field_0024 = CSF_AT(uint32_t, entry, 0x04);

                    void* globalObject = reinterpret_cast<void*>(GetGlobalObjectField184());
                    DispatchRecordToGlobalObject(globalObject, nullptr, &record, 1);

                    record.field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                    record.field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                    record.vtbl = reinterpret_cast<decltype(record.vtbl)>(CSF_VTBL(0x0082D410));
                }
            }
            break;
        }
        case 11:
        {
            const int index = AiAttackTasks_FindSceneEntryIndex(
                self, nullptr, CSF_AT(uint32_t, entry, 0x04));
            if (index != -1)
            {
                void* condition = entry + 0x08;
                const int sceneEntry = SceneLoader_GetSceneEntry(self, nullptr, index);
                currentState = TextDatabase_TestSceneEntryCondition(
                    reinterpret_cast<void*>(sceneEntry),
                    reinterpret_cast<int>(condition));
            }

            if (!currentState && CSF_AT(uint8_t, entry, 0x18))
            {
                alignas(8) uint8_t recordBytes[0x34];
                auto& record = *reinterpret_cast<C_0082d478*>(recordBytes);
                StateRecord_Reset(&record, nullptr, 11);
                record.field_000c = CSF_AT(uint32_t, entry, 0x0C);
                record.field_0010 = CSF_AT(uint32_t, entry, 0x10);
                record.field_0024 = CSF_AT(uint32_t, entry, 0x04);

                void* globalObject = reinterpret_cast<void*>(GetGlobalObjectField184());
                DispatchRecordToGlobalObject(globalObject, nullptr, &record, 1);

                record.field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                record.field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                record.vtbl = reinterpret_cast<decltype(record.vtbl)>(CSF_VTBL(0x0082D410));
            }
            break;
        }
        case 12:
        {
            currentState = TextDatabase_TestControllerCondition(
                self, nullptr,
                CSF_AT(void*, entry, 0x04),
                CSF_AT(unsigned int, entry, 0x14),
                CSF_AT(int, entry, 0x1C));

            if (currentState && !CSF_AT(uint8_t, entry, 0x18))
            {
                alignas(8) uint8_t recordBytes[0x34];
                auto& record = *reinterpret_cast<C_0082d478*>(recordBytes);
                StateRecord_Reset(&record, nullptr, 12);
                record.field_0020 = CSF_AT(uint32_t, entry, 0x14);
                record.field_0024 = CSF_AT(uint32_t, entry, 0x04);

                void* globalObject = reinterpret_cast<void*>(GetGlobalObjectField184());
                DispatchRecordToGlobalObject(globalObject, nullptr, &record, 1);

                record.field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                record.field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                record.vtbl = reinterpret_cast<decltype(record.vtbl)>(CSF_VTBL(0x0082D410));
            }
            break;
        }
        case 13:
        {
            currentState = TextDatabase_TestControllerCondition(
                self, nullptr,
                CSF_AT(void*, entry, 0x04),
                CSF_AT(unsigned int, entry, 0x14),
                CSF_AT(int, entry, 0x1C));

            if (!currentState && CSF_AT(uint8_t, entry, 0x18))
            {
                alignas(8) uint8_t recordBytes[0x34];
                auto& record = *reinterpret_cast<C_0082d478*>(recordBytes);
                StateRecord_Reset(&record, nullptr, 13);
                record.field_0020 = CSF_AT(uint32_t, entry, 0x14);
                record.field_0024 = CSF_AT(uint32_t, entry, 0x04);

                void* globalObject = reinterpret_cast<void*>(GetGlobalObjectField184());
                DispatchRecordToGlobalObject(globalObject, nullptr, &record, 1);

                record.field_0014 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                record.field_0008 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
                record.vtbl = reinterpret_cast<decltype(record.vtbl)>(CSF_VTBL(0x0082D410));
            }
            break;
        }
        default:
            break;
        }

        CSF_AT(char, entry, 0x18) = currentState;
        ++processed;
        entryOffset += 32;
    } while (processed < self->field_0024);

    return processed;
}

HOOK(0x004EF780, S_004ef780_UpdateManagedController, "auto");
