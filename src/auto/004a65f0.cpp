#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CCharActionFsm_ResolveActionTarget_486E60, 0x00486E60, (CCharActionFsm* self, void*));
CSF_FUNC(int, __fastcall, sub_4A5870, 0x004A5870, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, sub_771B80, 0x00771B80, (void* self, void* edx, int a2));

static int __fastcall CActionState_LookupActionValue(C_00834640* self, void* /*edx*/, S_00771b80* key)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    void* target = reinterpret_cast<void*>(
        CCharActionFsm_ResolveActionTarget_486E60(
            reinterpret_cast<CCharActionFsm*>(CSF_AT(uint32_t, owner, 0x44)), nullptr));

    if (!target)
        return sub_4A5870(self, nullptr, key);

    owner = reinterpret_cast<uint8_t*>(self->field_0008);
    if (CSF_AT(uint8_t, owner, 0x228) != 0) {
        int base = Handle_GetValuePointer(target, nullptr);
        int idx = sub_771B80(reinterpret_cast<void*>(base + 0x174), nullptr, reinterpret_cast<int>(key));
        if (idx == -1)
            return -1;
        uint8_t* p = reinterpret_cast<uint8_t*>(Handle_GetValuePointer(target, nullptr));
        uint8_t* arr = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, p, 0x178));
        return CSF_AT(int, arr + idx * 12, 8);
    } else {
        int base = Handle_GetValuePointer(target, nullptr);
        int idx = sub_771B80(reinterpret_cast<void*>(base + 0x164), nullptr, reinterpret_cast<int>(key));
        if (idx == -1)
            return -1;
        uint8_t* p = reinterpret_cast<uint8_t*>(Handle_GetValuePointer(target, nullptr));
        uint8_t* arr = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, p, 0x168));
        return CSF_AT(int, arr + idx * 12, 8);
    }
}

HOOK(0x004A65F0, CActionState_LookupActionValue, "auto");
