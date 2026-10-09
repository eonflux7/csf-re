#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __fastcall, IsItemUsableForCharacter, 0x0047A1E0,
         (void* self, void* edx, void* a2));
CSF_FUNC(void, __fastcall, CancelCurrentItemAction, 0x004846D0,
         (void* self, void* edx));

static void __fastcall CCharActionFsm_EnsureUsableItemSelected(
    CCharActionFsm* self, void* /*edx*/)
{
    const uint32_t itemsBegin = self->field_0078;
    if (!itemsBegin ||
        (static_cast<int32_t>(self->field_007c - itemsBegin) >> 2) == 0)
        return;

    auto* character = reinterpret_cast<C_008319c0*>(self->vGetIndexedEntryOrZero());
    const uint32_t selectedIndex = self->field_0090;
    C_0085e7b8* selectedItem = nullptr;

    if (selectedIndex != 0xffffffff)
    {
        selectedItem =
            reinterpret_cast<C_0085e7b8**>(self->field_0078)[selectedIndex];
        if (selectedItem &&
            IsItemUsableForCharacter(selectedItem, nullptr, character))
            return;
    }

    if (reinterpret_cast<C_0085e7b8*>(self->field_009c) == selectedItem)
        CancelCurrentItemAction(self, nullptr);

    uint32_t startIndex = self->field_0090;
    self->field_0090 = 0xffffffff;

    if (startIndex == 0xffffffff)
    {
        auto* firstItem =
            reinterpret_cast<C_0085e7b8**>(self->field_0078)[0];
        if (((bool(__fastcall*)(void*, void*))
                 firstItem->vtbl[0x98 / 4])(firstItem, nullptr) &&
            IsItemUsableForCharacter(firstItem, nullptr, character))
        {
            self->field_0090 = 0;
        }
        else
        {
            startIndex = 0;
        }
    }

    if (self->field_0090 == 0xffffffff)
    {
        const uint32_t begin = self->field_0078;
        const uint32_t itemCount = begin
            ? static_cast<uint32_t>(
                  static_cast<int32_t>(self->field_007c - begin) >> 2)
            : 0;
        uint32_t index = (startIndex + 1) % itemCount;

        if (index != startIndex)
        {
            do
            {
                if (self->field_0090 != 0xffffffff)
                    break;

                auto* item =
                    reinterpret_cast<C_0085e7b8**>(self->field_0078)[index];
                if (((bool(__fastcall*)(void*, void*))
                         item->vtbl[0x98 / 4])(item, nullptr))
                {
                    if (IsItemUsableForCharacter(item, nullptr, character))
                        self->field_0090 = index;
                }

                index = (index + 1) % itemCount;
            } while (index != startIndex);
        }
    }
}

HOOK(0x00486FC0, CCharActionFsm_EnsureUsableItemSelected, "auto");
