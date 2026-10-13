#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(char, __fastcall, C_008656d0_ProcessSkinnedItemCount, 0x006C67C0, (C_008656d0* self, void* , C_0086754c* a2));

static char __fastcall S_006dfeb0_ProcessSkinnedItems(
    S_006dfeb0* self, void* /*edx*/)
{
    if (!self->field_004c)
        return 0;

    uint32_t index = 0;
    const uint32_t initialCount = self->field_001c;
    self->field_0054 = 0;

    if (initialCount != 0)
    {
        do
        {
            auto* items =
                reinterpret_cast<C_0086754c**>(self->field_0014);
            C_0086754c* item = items[index];

            if (item &&
                !((bool(__fastcall*)(C_0086754c*, void*))
                    item->vtbl[0x34 / 4])(item, nullptr))
            {
                auto* manager = reinterpret_cast<C_008656d0*>(
                    GetGlobalValue_6C5760());
                C_008656d0_ProcessSkinnedItemCount(manager, nullptr, item);
                ++self->field_0054;
            }

            ++index;
        } while (index < self->field_001c);
    }

    uint32_t remainingGroups = CSF_AT(uint32_t, self, 0x3C);
    uint32_t groupOffset = 0;

    if (remainingGroups != 0)
    {
        do
        {
            auto* group = reinterpret_cast<uint8_t*>(
                CSF_AT(uint32_t, self, 0x34)) + groupOffset;
            auto* item = reinterpret_cast<C_0086754c*>(
                CSF_AT(uint32_t, group, 0));
            const uint32_t stride = CSF_AT(uint32_t, group, 4);
            const uint32_t totalBytes =
                stride * CSF_AT(uint32_t, group, 8);
            uint32_t itemOffset = 0;

            if (totalBytes != 0)
            {
                do
                {
                    if (!((bool(__fastcall*)(C_0086754c*, void*))
                            item->vtbl[0x34 / 4])(item, nullptr))
                    {
                        auto* manager = reinterpret_cast<C_008656d0*>(
                            GetGlobalValue_6C5760());
                        C_008656d0_ProcessSkinnedItemCount(
                            manager, nullptr, item);
                        ++self->field_0054;
                    }

                    itemOffset += stride;
                    item = reinterpret_cast<C_0086754c*>(
                        reinterpret_cast<uint8_t*>(item) + stride);
                } while (itemOffset < totalBytes);
            }

            groupOffset += 12;
            --remainingGroups;
        } while (remainingGroups != 0);
    }

    return 0;
}

HOOK(0x006DFEB0, S_006dfeb0_ProcessSkinnedItems, "auto");
