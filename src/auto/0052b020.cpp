#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(int, __cdecl, FxParticleManager_HasNamedEffect, 0x00437D60, (int a1, const char* a2));
CSF_FUNC(int, __fastcall, CMenuMessageBox_RegisterMaskedButton, 0x0052AEE0, (CMenuMessageBox* self, void* , void* button, uint32_t mask));
CSF_FUNC(void*, __cdecl, InitStringFromLiteral, 0x00692720, (void* returnSlot, const char* text, int count));

// EAX is an input only for the empty-list return path. The caller repairs
// that return below; the nonempty path overwrites EAX before using it.
CSF_FUNC(int, __fastcall, CMenuMessageBox_LayoutMaskedButtons, 0x0052AAA0,
         (CMenuMessageBox* self, void* edx, unsigned int width, int left));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static constexpr uint32_t CMenuMessageBox::* kButtonFields[] = {
    &CMenuMessageBox::field_002c,
    &CMenuMessageBox::field_0034,
    &CMenuMessageBox::field_003c,
    &CMenuMessageBox::field_0040,
    &CMenuMessageBox::field_0030,
    &CMenuMessageBox::field_0038,
    &CMenuMessageBox::field_0044,
    &CMenuMessageBox::field_0048,
    &CMenuMessageBox::field_004c,
    &CMenuMessageBox::field_0050,
    &CMenuMessageBox::field_0054
};

static void CMenuMessageBox_UpdateSlotLabel(
    CMenuMessageBox* self,
    uint32_t CMenuMessageBox::* buttonField,
    std::size_t stringOffset)
{
    void* button = reinterpret_cast<void*>(self->*buttonField);
    if (!button)
        return;

    if (((bool(__fastcall*)(void*, void*))
         CSF_AT(void**, button, 0)[172 / 4])(button, nullptr))
        return;

    auto* slotString = reinterpret_cast<uint8_t*>(self) + stringOffset;
    if (static_cast<uint8_t>(FxParticleManager_HasNamedEffect(
            reinterpret_cast<int>(slotString),
            reinterpret_cast<const char*>(&g_byte_82E65C))) == 0)
        return;

    button = reinterpret_cast<void*>(self->*buttonField);
    void* label = ((void*(__fastcall*)(void*, void*))
                  CSF_AT(void**, button, 0)[272 / 4])(button, nullptr);
    if (!label)
        return;

    // The return slot and the string storage together occupy the original
    // 0x20-byte frame region.
    alignas(8) uint8_t localizedBytes[0x20];
    CSF_AT(int, localizedBytes, 0x14) = 0;
    CSF_AT(uint16_t, localizedBytes, 0x04) = 0;
    CSF_AT(unsigned int, localizedBytes, 0x18) = 7;

    const char* text;
    if (CSF_AT(unsigned int, slotString, 0x18) < 0x10u)
        text = reinterpret_cast<const char*>(slotString + 4);
    else
        text = CSF_AT(const char*, slotString, 4);

    InitStringFromLiteral(localizedBytes, text, 1);

    void* localizedText = CSF_AT(void*, localizedBytes, 0x04);
    if (CSF_AT(unsigned int, localizedBytes, 0x18) < 8)
        localizedText = localizedBytes + 4;

    ((void(__fastcall*)(void*, void*, void*))
     CSF_AT(void**, label, 0)[64 / 4])(label, nullptr, localizedText);
    SaveGame_DestroyLocalizedString(localizedBytes, nullptr);
}

static int __fastcall CMenuMessageBox_PreCloseCleanup(
    CMenuMessageBox* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0894);
    void* layout = ((void*(__fastcall*)(void*, void*))
                   CSF_AT(void**, owner, 0)[460 / 4])(owner, nullptr);
    if (!layout)
        return 0;

    for (auto field : kButtonFields)
    {
        void* button = reinterpret_cast<void*>(self->*field);
        if (button)
        {
            ((int(__fastcall*)(void*, void*, int))
             CSF_AT(void**, button, 0)[660 / 4])(button, nullptr, 0);
        }
    }

    void* firstButton = reinterpret_cast<void*>(self->field_002c);
    self->field_095c = 0;
    CMenuMessageBox_RegisterMaskedButton(self, nullptr, firstButton, 1);
    CMenuMessageBox_RegisterMaskedButton(
        self, nullptr, reinterpret_cast<void*>(self->field_0034), 4);
    CMenuMessageBox_RegisterMaskedButton(
        self, nullptr, reinterpret_cast<void*>(self->field_003c), 32);
    CMenuMessageBox_RegisterMaskedButton(
        self, nullptr, reinterpret_cast<void*>(self->field_0040), 16);
    CMenuMessageBox_RegisterMaskedButton(
        self, nullptr, reinterpret_cast<void*>(self->field_0030), 2);
    CMenuMessageBox_RegisterMaskedButton(
        self, nullptr, reinterpret_cast<void*>(self->field_0038), 8);
    CMenuMessageBox_RegisterMaskedButton(
        self, nullptr, reinterpret_cast<void*>(self->field_0044), 64);
    CMenuMessageBox_RegisterMaskedButton(
        self, nullptr, reinterpret_cast<void*>(self->field_0048), 128);
    CMenuMessageBox_RegisterMaskedButton(
        self, nullptr, reinterpret_cast<void*>(self->field_004c), 256);
    CMenuMessageBox_RegisterMaskedButton(
        self, nullptr, reinterpret_cast<void*>(self->field_0050), 512);
    CMenuMessageBox_RegisterMaskedButton(
        self, nullptr, reinterpret_cast<void*>(self->field_0054), 1024);

    CMenuMessageBox_UpdateSlotLabel(self, &CMenuMessageBox::field_0044, 0x8BC);
    CMenuMessageBox_UpdateSlotLabel(self, &CMenuMessageBox::field_0048, 0x8D8);
    CMenuMessageBox_UpdateSlotLabel(self, &CMenuMessageBox::field_004c, 0x8F4);
    CMenuMessageBox_UpdateSlotLabel(self, &CMenuMessageBox::field_0050, 0x910);
    CMenuMessageBox_UpdateSlotLabel(self, &CMenuMessageBox::field_0054, 0x92C);

    unsigned int width = *(
        (unsigned int*(__fastcall*)(void*, void*))
        CSF_AT(void**, layout, 0)[40 / 4])(layout, nullptr);

    int* leftPointer = ((int*(__fastcall*)(void*, void*))
                        CSF_AT(void**, layout, 0)[32 / 4])(layout, nullptr);
    int left = *leftPointer;

    const bool emptyButtonList = self->field_095c == 0;
    int result = CMenuMessageBox_LayoutMaskedButtons(self, nullptr, width, left);
    if (emptyButtonList)
        result = left;

    self->field_0078 = -1;
    for (auto field : kButtonFields)
    {
        void* button = reinterpret_cast<void*>(self->*field);
        if (button)
        {
            result = ((int(__fastcall*)(void*, void*, int))
                      CSF_AT(void**, button, 0)[660 / 4])(button, nullptr, 1);
        }
    }
    return result;
}

HOOK(0x0052B020, CMenuMessageBox_PreCloseCleanup, "auto");
