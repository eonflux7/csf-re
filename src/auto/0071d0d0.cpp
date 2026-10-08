#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, MenuExtras_CloseFile, 0x00694380,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, ClearRecordStringRange, 0x0071B600,
         (std::uint32_t first, std::uint32_t last));

static int __fastcall C_00864508_DestroyMembers(
    C_00864508* self, void* /*edx*/)
{
    self->vtbl = CSF_VTBL(0x00864508);

    const std::uint32_t recordBegin = self->field_0014;
    std::uint32_t remainingRecords = recordBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0018 - recordBegin) >> 6)
        : 0;

    if (remainingRecords != 0)
    {
        std::uint32_t recordOffset = 0;
        do
        {
            void* attachment = reinterpret_cast<void*>(
                CSF_AT(std::uint32_t,
                       reinterpret_cast<void*>(self->field_0014 + recordOffset),
                       0x3C));
            if (attachment != nullptr)
            {
                MenuExtras_CloseFile(attachment, nullptr);
                Mem_Free(attachment);
                CSF_AT(std::uint32_t,
                       reinterpret_cast<void*>(self->field_0014 + recordOffset),
                       0x3C) = 0;
            }
            recordOffset += 0x40;
        }
        while (--remainingRecords != 0);
    }

    if (self->field_0014 != 0)
    {
        ClearRecordStringRange(self->field_0014, self->field_0018);
        if (self->field_0014 != 0)
            Mem_FreeDebug(reinterpret_cast<void*>(self->field_0014));
    }
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;

    const std::uint32_t childBegin = self->field_0048;
    const std::uint32_t childCount = childBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_004c - childBegin) >> 2)
        : 0;

    for (std::uint32_t index = 0; index < childCount; ++index)
    {
        void* child = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->field_0048 + index * 4u), 0));
        void* secondaryBase = static_cast<std::uint8_t*>(child) + 4;
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, secondaryBase, 0)[0x4C / 4])(
                secondaryBase, nullptr);

        // The callback can replace the array or its current element.
        child = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->field_0048 + index * 4u), 0));
        void* target = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, child, 0)[0x20 / 4])(child, nullptr);
        if (target != nullptr)
        {
            ((void(__fastcall*)(void*, void*, int))
                CSF_AT(void**, target, 0)[0x138 / 4])(target, nullptr, 14);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, target, 0)[0xF4 / 4])(target, nullptr);
        }
    }

    void* ownedObject = reinterpret_cast<void*>(self->field_0040);
    if (ownedObject != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_0040 = 0;
    }

    ownedObject = reinterpret_cast<void*>(self->field_000c);
    if (ownedObject != nullptr)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, ownedObject, 0)[0])(ownedObject, nullptr, 1);
        self->field_000c = 0;
    }

    if (self->field_0094 >= 0x10u && self->field_0080 != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0080));
    self->field_0094 = 15;
    self->field_0090 = 0;
    *reinterpret_cast<std::uint8_t*>(&self->field_0080) = 0;

    if (self->field_0058 != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0058));
    self->field_0058 = 0;
    self->field_005c = 0;
    self->field_0060 = 0;

    if (self->field_0048 != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0048));
    self->field_0048 = 0;
    self->field_004c = 0;
    self->field_0050 = 0;

    if (self->field_0034 != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0034));
    self->field_0034 = 0;
    self->field_0038 = 0;
    self->field_003c = 0;

    if (self->field_0024 != 0)
        Mem_FreeDebug(reinterpret_cast<void*>(self->field_0024));
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_002c = 0;

    std::uint32_t result = self->field_0014;
    if (result != 0)
    {
        ClearRecordStringRange(result, self->field_0018);
        result = self->field_0014;
        if (result != 0)
            result = static_cast<std::uint32_t>(
                Mem_FreeDebug(reinterpret_cast<void*>(result)));
    }
    self->field_0014 = 0;
    self->field_0018 = 0;
    self->field_001c = 0;

    self->vtbl = CSF_VTBL(0x00830438);
    return static_cast<int>(result);
}

HOOK(0x0071D0D0, C_00864508_DestroyMembers, "auto");
