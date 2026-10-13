#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(int, __fastcall, SaveGame_DestroyLocalizedString, 0x00428E90, (void* self, void*));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, CMenuEula_ConstructStringCopy, 0x00533530, (void* self, void* , char* pDest, unsigned int a3, unsigned int a4));
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0,
         (void* self, void* edx, const char* text));

CSF_GLOBAL(const char*, g_pszMenu_P_MePrin, 0x00899AE4);
CSF_GLOBAL(const char*, g_pszMenu_P_LbLaWa, 0x00899B74);

[[maybe_unused]] static const auto kArrayCapacityVtable = CSF_VTBL(0x0041E5F0);

static int __fastcall CMenuEula_HandleButtonEvent(
    CMenuEula* self, void* /*edx*/, std::uint32_t* event)
{
    if (event[0] != 13)
        return static_cast<int>(reinterpret_cast<std::uintptr_t>(event));

    const std::uint32_t button = event[1];

    if (button == self->field_0028)
    {
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(GetGlobalState_50D190()), 0xD0) = 0;
        self->vSlot4C();

        void* menu = reinterpret_cast<void*>(self->field_0014);
        return reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x58 / 4])(
                menu, nullptr, g_pszMenu_P_MePrin);
    }

    if (button == self->field_0024)
    {
        self->vSlot4C();

        void* menu = reinterpret_cast<void*>(self->field_0014);
        return reinterpret_cast<int (__fastcall*)(void*, void*, const char*)>(
            CSF_AT(void**, menu, 0)[0x58 / 4])(
                menu, nullptr, g_pszMenu_P_LbLaWa);
    }

    if (button == self->field_002c)
    {
        const std::uint32_t pageCount = self->field_0160;
        const std::uint32_t previousOffset =
            reinterpret_cast<std::uint32_t*>(self->field_0158)[pageCount - 2];
        self->field_0038 = previousOffset;
        self->field_0160 = pageCount - 1;

        alignas(8) std::uint8_t previousPageBytes[0x1C];
        void* pageString = reinterpret_cast<void*>(CMenuEula_ConstructStringCopy(
            reinterpret_cast<std::uint8_t*>(self) + 0x3C, nullptr,
            reinterpret_cast<char*>(previousPageBytes), self->field_0038, 0x5DCu));

        const char* text;
        if (CSF_AT(std::uint32_t, pageString, 0x18) < 8u)
            text = reinterpret_cast<char*>(pageString) + 4;
        else
            text = CSF_AT(const char*, pageString, 4);

        C_00720DF0_ForwardCallToChildVtblSlot16(
            reinterpret_cast<void*>(self->m_textControl), nullptr, text);
        const int result = SaveGame_DestroyLocalizedString(previousPageBytes, nullptr);
        self->field_0018 = 5;
        return result;
    }

    if (button == self->field_0030)
    {
        alignas(8) std::uint8_t pageInfoBytes[0x1C];
        CSF_AT(std::uint32_t, pageInfoBytes, 0x18) = 7;
        CSF_AT(std::uint32_t, pageInfoBytes, 0x14) = 0;
        CSF_AT(std::uint16_t, pageInfoBytes, 4) = 0;

        void* textControl = reinterpret_cast<void*>(self->m_textControl);
        void* pageInfo =
            reinterpret_cast<void* (__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, textControl, 0)[0x24C / 4])(
                    textControl, nullptr, pageInfoBytes, 0);
        const int advance = CSF_AT(int, pageInfo, 0x14);

        if (advance > 0)
        {
            self->field_0038 += advance;
            if (self->field_0160 >= CSF_AT(std::uint32_t, self, 0x15C))
            {
                S_0041e5f0_GrowArrayCapacity(
                    reinterpret_cast<S_0041e5f0*>(
                        reinterpret_cast<std::uint8_t*>(self) + 0x158),
                    nullptr, CSF_AT(int, self, 0x164));
            }

            const std::uint32_t pageCount = self->field_0160;
            std::uint32_t* pageOffset =
                reinterpret_cast<std::uint32_t*>(self->field_0158) + pageCount;
            self->field_0160 = pageCount + 1;
            *pageOffset = self->field_0038;

            alignas(8) std::uint8_t nextPageBytes[0x1C];
            void* pageString = reinterpret_cast<void*>(CMenuEula_ConstructStringCopy(
                reinterpret_cast<std::uint8_t*>(self) + 0x3C, nullptr,
                reinterpret_cast<char*>(nextPageBytes), self->field_0038, 0x5DCu));

            const char* text;
            if (CSF_AT(std::uint32_t, pageString, 0x18) < 8u)
                text = reinterpret_cast<char*>(pageString) + 4;
            else
                text = CSF_AT(const char*, pageString, 4);

            C_00720DF0_ForwardCallToChildVtblSlot16(
                reinterpret_cast<void*>(self->m_textControl), nullptr, text);
            SaveGame_DestroyLocalizedString(nextPageBytes, nullptr);
            self->field_0018 = 3;
        }

        return SaveGame_DestroyLocalizedString(pageInfoBytes, nullptr);
    }

    return static_cast<int>(button);
}

HOOK(0x00533750, CMenuEula_HandleButtonEvent, "auto");
