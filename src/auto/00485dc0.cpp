#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_008319c0_GetSubObjectField74, 0x004760A0, (C_008319c0* self, void*));
CSF_FUNC(int, __fastcall, CCharActionFsm_FindFallbackRecord, 0x00483210, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, FindRecordIndexById, 0x00485980, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static int __fastcall CCharActionFsm_GetActionAvailability(
    CCharActionFsm* self, void* /*edx*/, int actionId)
{
    void* owner = reinterpret_cast<void*>(self->field_0008);
    if (!((bool(__fastcall*)(void*, void*, int))
              CSF_AT(void**, owner, 0)[0xC0 / 4])(owner, nullptr, actionId))
    {
        return 0;
    }

    owner = reinterpret_cast<void*>(self->field_0008);
    const int mode = CSF_AT(int, owner, 0x54);
    void* record = nullptr;

    if (mode == 1)
    {
        const int index = self->vFindProviderIndex(actionId);
        if (index != -1)
        {
            record = reinterpret_cast<void*>(self->vGetIndexedEntry_33(index));
            const int type = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, record, 0)[0x4C / 4])(record, nullptr);

            if (type == 10 && FindRecordIndexById(self, nullptr, actionId) == -1)
                return 1;
        }
        else
        {
            const int recordIndex = FindRecordIndexById(self, nullptr, actionId);
            if (recordIndex != -1)
            {
                record = reinterpret_cast<void*>(
                    reinterpret_cast<uint32_t*>(self->field_0078)[recordIndex]);
            }
            else
            {
                record = reinterpret_cast<void*>(
                    CCharActionFsm_FindFallbackRecord(self, nullptr, actionId));
            }
        }
    }
    else if (mode > 1 && mode <= 3)
    {
        record = reinterpret_cast<void*>(self->vLookupEntryOrZero(actionId));
    }

    if (record)
    {
        return ((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, record, 0)[0x58 / 4])(record, nullptr) ? 1 : 0;
    }

    void* registry = reinterpret_cast<void*>(GetValue8B0554());
    void* records = CSF_AT(void*, registry, 0x1FC0);
    void* definition = ((void*(__fastcall*)(void*, void*, int))
        CSF_AT(void**, records, 0)[0x8 / 4])(records, nullptr, actionId);

    if (CSF_AT(int, definition, 0x74) == 8)
        return 1;

    void* handle = reinterpret_cast<void*>(
        self->vFindMatchingHandle(CSF_AT(int, definition, 0xDC), 8));

    if (!CSF_AT(int, definition, 0xDC) || !handle)
        return 1;

    void* value = reinterpret_cast<void*>(Handle_GetValuePointer(handle, nullptr));
    if (CSF_AT(uint8_t, value, 0xE4))
    {
        const int type = C_008319c0_GetSubObjectField74(
            reinterpret_cast<C_008319c0*>(handle), nullptr);
        if (CSF_AT(int, definition, 0x74) != type)
            return 0;
    }

    return 2;
}

HOOK(0x00485DC0, CCharActionFsm_GetActionAvailability, "auto");
