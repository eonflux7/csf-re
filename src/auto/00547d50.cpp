#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, MenuExtras_CopyStringRange, 0x00403B60,
         (void* destination, void* edx, const void* source,
          uint32_t start, uint32_t count));
CSF_FUNC(int, __cdecl, MenuExtras_DestroyThumbnailRange, 0x00546D60,
         (void* begin, void* end, void* container, uint32_t byteOffset));
CSF_FUNC(int, __fastcall, CMenuExtras_ResetActiveExtra, 0x005477E0,
         (CMenuExtras* self, void* edx));
CSF_FUNC(int, __cdecl, MenuExtras_FreeContainerStorage, 0x00690E60,
         (void* block));
CSF_FUNC(int, __cdecl, MenuExtras_FreeStringStorage, 0x00691110,
         (void* block));

CSF_GLOBAL(const char*, g_MenuExtrasCampaignMenuName, 0x00899AD8);

static int __fastcall CMenuExtras_HandleBackAction(
    CMenuExtras* self, void* /*edx*/, int /*unused*/)
{
    void* thumbnailControl = reinterpret_cast<void*>(self->field_002c);
    const uint8_t isActive =
        (reinterpret_cast<uint8_t(__fastcall*)(void*, void*)>(
            CSF_AT(void**, thumbnailControl, 0)[0xAC / 4]))(
                thumbnailControl, nullptr);

    if (!isActive)
    {
        thumbnailControl = reinterpret_cast<void*>(self->field_002c);
        (reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, thumbnailControl, 0)[0xA8 / 4]))(
                thumbnailControl, nullptr, 1);

        void* descriptionControl = reinterpret_cast<void*>(self->field_0030);
        const int result =
            (reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, descriptionControl, 0)[0xA8 / 4]))(
                    descriptionControl, nullptr, 1);

        self->field_0044 = 1;
        self->field_0045 = 1;
        self->field_0046 = 0;
        return result;
    }

    if (self->field_0047)
        CMenuExtras_ResetActiveExtra(self, nullptr);

    uint32_t index = 0;
    uint32_t byteOffset = 0;
    for (;;)
    {
        const uint32_t begin = self->field_0038;
        if (!begin)
            break;

        const int32_t byteCount =
            static_cast<int32_t>(CSF_AT(uint32_t, self, 0x3C) - begin);
        const uint32_t count = static_cast<uint32_t>(byteCount / 68);
        if (index >= count)
            break;

        auto* record =
            reinterpret_cast<uint8_t*>(self->field_0038 + byteOffset);

        // The original also copies the leading record word.
        volatile uint32_t recordWord = CSF_AT(uint32_t, record, 0);
        (void)recordWord;

        uint32_t firstString[7];
        firstString[6] = 15;
        firstString[5] = 0;
        *reinterpret_cast<uint8_t*>(&firstString[1]) = 0;
        MenuExtras_CopyStringRange(
            firstString, nullptr, record + 4, 0, UINT32_MAX);

        void* firstObject = CSF_AT(void*, record, 0x20);
        void* secondObject = CSF_AT(void*, record, 0x24);

        uint32_t secondString[7];
        secondString[6] = 15;
        secondString[5] = 0;
        *reinterpret_cast<uint8_t*>(&secondString[1]) = 0;
        MenuExtras_CopyStringRange(
            secondString, nullptr, record + 0x28, 0, UINT32_MAX);

        if (firstObject)
        {
            (reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, firstObject, 0)[0]))(
                    firstObject, nullptr, 1);
            firstObject = nullptr;
        }
        if (secondObject)
        {
            (reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, secondObject, 0)[0]))(
                    secondObject, nullptr, 1);
            secondObject = nullptr;
        }

        if (secondString[6] >= 16)
            MenuExtras_FreeStringStorage(
                reinterpret_cast<void*>(secondString[1]));

        const bool freeFirstString = firstString[6] >= 16;
        secondString[6] = 15;
        secondString[5] = 0;
        *reinterpret_cast<uint8_t*>(&secondString[1]) = 0;

        if (freeFirstString)
            MenuExtras_FreeStringStorage(
                reinterpret_cast<void*>(firstString[1]));

        firstString[6] = 15;
        firstString[5] = 0;
        *reinterpret_cast<uint8_t*>(&firstString[1]) = 0;
        ++index;
        byteOffset += 68;
    }

    if (self->field_0038)
    {
        MenuExtras_DestroyThumbnailRange(
            reinterpret_cast<void*>(self->field_0038),
            CSF_AT(void*, self, 0x3C),
            reinterpret_cast<uint8_t*>(self) + 0x34,
            byteOffset);

        if (self->field_0038)
            MenuExtras_FreeContainerStorage(
                reinterpret_cast<void*>(self->field_0038));
    }

    self->field_0038 = 0;
    CSF_AT(uint32_t, self, 0x3C) = 0;
    CSF_AT(uint32_t, self, 0x40) = 0;

    void* menuManager = reinterpret_cast<void*>(self->field_0010);
    (reinterpret_cast<void(__fastcall*)(void*, void*, CMenuExtras*, int)>(
        CSF_AT(void**, menuManager, 0)[0x60 / 4]))(
            menuManager, nullptr, self, 0);

    menuManager = reinterpret_cast<void*>(self->field_0010);
    int result =
        (reinterpret_cast<int(__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menuManager, 0)[0x58 / 4]))(
                menuManager, nullptr, g_MenuExtrasCampaignMenuName);

    void* parentScreen = reinterpret_cast<void*>(self->field_0018);
    if (parentScreen)
    {
        result =
            (reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, parentScreen, 0)[0x16C / 4]))(
                    parentScreen, nullptr);
    }

    self->field_0046 = 0;
    return result;
}

HOOK(0x00547D50, CMenuExtras_HandleBackAction, "auto");
