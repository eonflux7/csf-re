#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, MpRules_DestroyEntryRange, 0x00559910, (int a1, int a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

static int __fastcall C_00844928_Destroy(C_00844928* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00844928);

    uint32_t entryIndex = 0;
    for (uint32_t entryOffset = 0; ; entryOffset += 20, ++entryIndex)
    {
        const uint32_t begin = self->field_01dc;
        if (!begin ||
            entryIndex >= static_cast<uint32_t>(
                static_cast<int32_t>(CSF_AT(uint32_t, self, 0x1E0) - begin) / 20))
        {
            break;
        }

        uint32_t childIndex = 0;
        for (uint32_t childOffset = 0; ; childOffset += 20, ++childIndex)
        {
            auto* entry = reinterpret_cast<uint8_t*>(
                static_cast<uintptr_t>(self->field_01dc)) + entryOffset;
            const uint32_t childBegin = CSF_AT(uint32_t, entry, 0x8);
            if (!childBegin ||
                childIndex >=
                    (CSF_AT(uint32_t, entry, 0xC) - childBegin) / 20)
            {
                break;
            }

            auto* child = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(
                CSF_AT(uint32_t, entry, 0x8))) + childOffset;
            void* block = CSF_AT(void*, child, 0x8);
            if (block)
                Mem_FreeDebug(block);

            CSF_AT(uint32_t, child, 0x8) = 0;
            CSF_AT(uint32_t, child, 0xC) = 0;
            CSF_AT(uint32_t, child, 0x10) = 0;
        }

        auto* entry = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(self->field_01dc)) + entryOffset;
        const uint32_t childBegin = CSF_AT(uint32_t, entry, 0x8);
        if (childBegin)
        {
            const uint32_t childEnd = CSF_AT(uint32_t, entry, 0xC);
            uint32_t childAddress = childBegin;
            if (childAddress != childEnd)
            {
                do
                {
                    auto* child = reinterpret_cast<uint8_t*>(
                        static_cast<uintptr_t>(childAddress));
                    if (CSF_AT(uint32_t, child, 0x8))
                        Mem_FreeDebug(CSF_AT(void*, child, 0x8));

                    CSF_AT(uint32_t, child, 0x8) = 0;
                    CSF_AT(uint32_t, child, 0xC) = 0;
                    CSF_AT(uint32_t, child, 0x10) = 0;
                    childAddress += 20;
                }
                while (childAddress != childEnd);
            }

            if (CSF_AT(uint32_t, entry, 0x8))
                Mem_FreeDebug(CSF_AT(void*, entry, 0x8));
        }

        CSF_AT(uint32_t, entry, 0x8) = 0;
        CSF_AT(uint32_t, entry, 0xC) = 0;
        CSF_AT(uint32_t, entry, 0x10) = 0;
    }

    if (self->field_01dc)
    {
        MpRules_DestroyEntryRange(
            static_cast<int>(self->field_01dc),
            CSF_AT(int, self, 0x1E0));
        if (self->field_01dc)
            Mem_FreeDebug(reinterpret_cast<void*>(
                static_cast<uintptr_t>(self->field_01dc)));
    }

    self->field_01dc = 0;
    CSF_AT(uint32_t, self, 0x1E0) = 0;
    CSF_AT(uint32_t, self, 0x1E4) = 0;

    if (self->field_0200 >= 0x10 && self->field_01ec)
        Mem_FreeDebug(reinterpret_cast<void*>(
            static_cast<uintptr_t>(self->field_01ec)));

    self->field_0200 = 15;
    CSF_AT(uint32_t, self, 0x1FC) = 0;
    reinterpret_cast<uint8_t*>(&self->field_01ec)[0] = 0;

    if (self->field_01dc)
    {
        MpRules_DestroyEntryRange(
            static_cast<int>(self->field_01dc),
            CSF_AT(int, self, 0x1E0));
        if (self->field_01dc)
            Mem_FreeDebug(reinterpret_cast<void*>(
                static_cast<uintptr_t>(self->field_01dc)));
    }

    self->field_01dc = 0;
    CSF_AT(uint32_t, self, 0x1E0) = 0;
    CSF_AT(uint32_t, self, 0x1E4) = 0;

    int result = CSF_AT(int, self, 0xC);
    if (result)
        result = Mem_FreeDebug(CSF_AT(void*, self, 0xC));

    CSF_AT(uint32_t, self, 0xC) = 0;
    CSF_AT(uint32_t, self, 0x10) = 0;
    CSF_AT(uint32_t, self, 0x14) = 0;
    self->vtbl = CSF_VTBL(0x008446CC);
    return result;
}

HOOK(0x00559E90, C_00844928_Destroy, "auto");
