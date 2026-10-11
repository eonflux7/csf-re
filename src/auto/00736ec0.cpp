#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetIndexedTableValue_6AF8A0, 0x006AF8A0, (void* self, void* , int index));
CSF_FUNC(bool, __fastcall, C_008670f0_IsUpdateSuppressed, 0x00732C90, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_008670f0_UpdateTransform, 0x00733520, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_008670f0_UpdateAttachmentState, 0x00736D50, (void* self, void* edx));

static char __fastcall C_008670f0_UpdateLinkedObjects(
    C_008670f0* self, void* /*edx*/)
{
    if (!self->field_0014)
        return 0;

    if (C_008670f0_IsUpdateSuppressed(self, nullptr))
        return 1;

    if (self->field_000c == 1)
    {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        if (!reinterpret_cast<bool(__fastcall*)(void*, void*)>(
                CSF_AT(void**, owner, 0)[0x10 / 4])(owner, nullptr))
        {
            reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, self, 0)[0x10 / 4])(self, nullptr, 0);
        }
    }

    void* attachment = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void(__fastcall*)(void*, void*, float*)>(
        CSF_AT(void**, attachment, 0)[0x28 / 4])(
            attachment, nullptr, &self->field_00c0);

    C_008670f0_UpdateAttachmentState(self, nullptr);
    C_008670f0_UpdateTransform(self, nullptr);

    attachment = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void(__fastcall*)(void*, void*, uint32_t*, uint32_t, uint32_t)>(
        CSF_AT(void**, attachment, 0)[0x14 / 4])(
            attachment, nullptr, &self->field_0078,
            self->field_0104, self->field_0108);

    const uint32_t count = self->field_0090;
    if (!count)
        return 0;

    alignas(8) uint8_t updateBytes[0x14];
    CSF_AT(uint32_t, updateBytes, 0x10) = count;
    uint32_t entryOffset = 0;

    do
    {
        const int index = CSF_AT(
            int, reinterpret_cast<void*>(self->field_0088 + entryOffset), 0);
        void* linkedObject = reinterpret_cast<void*>(
            GetIndexedTableValue_6AF8A0(
                reinterpret_cast<void*>(self->field_0008), nullptr, index));

        void* values = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, linkedObject, 0)[0x6C / 4])(
                linkedObject, nullptr);

        CSF_AT(uint32_t, updateBytes, 0x00) = CSF_AT(uint32_t, values, 0x00);
        CSF_AT(uint32_t, updateBytes, 0x04) = CSF_AT(uint32_t, values, 0x04);
        CSF_AT(uint32_t, updateBytes, 0x08) = CSF_AT(uint32_t, values, 0x08);
        CSF_AT(float, updateBytes, 0x0C) = self->field_00b4;

        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, linkedObject, 0)[0x24 / 4])(
                linkedObject, nullptr,
                reinterpret_cast<void*>(self->field_0088 + entryOffset + 4));

        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, linkedObject, 0)[0x68 / 4])(
                linkedObject, nullptr, updateBytes);

        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, linkedObject, 0)[0x08 / 4])(
                linkedObject, nullptr);

        entryOffset += 12;
        --CSF_AT(uint32_t, updateBytes, 0x10);
    }
    while (CSF_AT(uint32_t, updateBytes, 0x10));

    return 0;
}

HOOK(0x00736EC0, C_008670f0_UpdateLinkedObjects, "auto");
