#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, S_00413570_GrowEventRecords, 0x00413570, (S_00413570* self, void* , int additionalCapacity));
CSF_FUNC(bool, __fastcall, sub_4E55C0, 0x004E55C0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(char, __fastcall, ComputeSlotStatusByte, 0x004EF150, (void* self, void* edx, void* a1, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083e9d0_CollectVisibleSceneEntities(C_0083e9d0* self, void* /*edx*/)
{
    int result = self->vResetStateFields();
    if (self->field_0044 != 0xFFFFFFFFu)
    {
        int ctx = GetValue8B0554();
        void* statusSelf = reinterpret_cast<void*>(static_cast<uintptr_t>(CSF_AT(uint32_t, ctx, 0x1FDC)));
        uint32_t count = CSF_AT(uint32_t, g_pCurrentScene, 0x28);
        result = static_cast<int>(count);
        for (uint32_t i = 0; i < count; ++i)
        {
            uint8_t* entity = CSF_AT(uint8_t**, g_pCurrentScene, 0x20)[i];
            if (!entity)
                continue;
            if (self->field_0008 == static_cast<uint32_t>(reinterpret_cast<uintptr_t>(entity)))
                continue;
            if (CSF_AT(uint32_t, entity, 0x78) == 0xFFFFFFFFu)
                continue;

            uint8_t* sub = entity + 0x1C;
            if (!self->vSlot9C(static_cast<int>(reinterpret_cast<uintptr_t>(sub)), 14))
                continue;
            if (!ComputeSlotStatusByte(statusSelf, nullptr,
                                       reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0044)),
                                       static_cast<int>(reinterpret_cast<uintptr_t>(sub))))
                continue;
            if (self->field_004d)
            {
                if (!sub_4E55C0(self, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(sub)), 14))
                    continue;
            }

            if (self->field_0014 >= self->field_0010)
                S_00413570_GrowEventRecords(reinterpret_cast<S_00413570*>(&self->field_000c), nullptr,
                                            CSF_AT(int, self, 0x18));
            uint32_t idx = self->field_0014;
            uint32_t base = self->field_000c;
            self->field_0014 = idx + 1;
            uint8_t* rec = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(base)) + 12 * idx;
            CSF_AT(uint32_t, rec, 4) = CSF_AT(uint32_t, sub, 4);
            CSF_AT(uint32_t, rec, 8) = CSF_AT(uint32_t, sub, 8);
        }
    }
    return result;
}

HOOK(0x004E65A0, C_0083e9d0_CollectVisibleSceneEntities, "auto");
