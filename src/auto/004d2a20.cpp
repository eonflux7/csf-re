#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, ForwardToNestedObjectMethod, 0x0048E700, (void* self, void*));
CSF_FUNC(void, __fastcall, C_008343a4_UpdateAlertEntryForReference, 0x004A08F0, (C_008343a4* self, void* , int a2, int a3, int a4));
CSF_FUNC(void*, __fastcall, MenuMpAccounts_GetPlayerName, 0x00741010, (void* self, void*));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_0083ba40_ActivatePlayerAlert(
    C_0083ba40* self, void* /*edx*/)
{
    if (!self->field_0062)
        return;

    ForwardToNestedObjectMethod(
        reinterpret_cast<void*>(self->field_0014), nullptr);

    void* linkedObject = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0014), 0x44);
    if (linkedObject)
    {
        void* alertObject =
            reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, linkedObject, 0)[0xD8 / 4])(
                    linkedObject, nullptr);
        if (alertObject)
        {
            if (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, alertObject, 0)[0xA8 / 4])(
                        alertObject, nullptr))
            {
                void* indicator =
                    reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, alertObject, 0)[0xA8 / 4])(
                            alertObject, nullptr);
                reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, indicator, 0)[0x3C / 4])(
                        indicator, nullptr, 1);
            }
        }
    }

    const uint32_t embeddedObject = CSF_AT(
        uint32_t, reinterpret_cast<void*>(self->field_0014), 0x18);
    void* character = embeddedObject
        ? reinterpret_cast<void*>(embeddedObject - 0x6B0)
        : nullptr;
    const uint8_t team = CSF_AT(uint8_t, character, 0x443);
    self->field_0064 = team;

    alignas(8) uint8_t colorBytes[0x20];
    int* colors = reinterpret_cast<int*>(colorBytes);
    if (team == 1)
    {
        colors[4] = 1128792064;
        colors[5] = 1101004800;
        colors[6] = 1112014848;
    }
    else if (team == 2)
    {
        colors[4] = 1101004800;
        colors[5] = 1125515264;
        colors[6] = 1128792064;
    }
    else
    {
        colors[4] = 1132396544;
        colors[5] = 1132396544;
        colors[6] = 1132396544;
    }
    colors[7] = 1132396544;
    colors[0] = colors[4];
    colors[1] = colors[5];
    colors[2] = colors[6];
    colors[3] = colors[7];

    void* playerName = MenuMpAccounts_GetPlayerName(character, nullptr);
    void* nameData;
    if (CSF_AT(uint32_t, playerName, 0x18) < 8u)
        nameData = static_cast<uint8_t*>(playerName) + 4;
    else
        nameData = CSF_AT(void*, playerName, 4);

    void* reference =
        reinterpret_cast<uint8_t*>(self->field_0014) + 0x1C;
    C_008343a4_UpdateAlertEntryForReference(
        CSF_AT(C_008343a4*, g_pCurrentScene, 0xAC),
        nullptr,
        reinterpret_cast<int>(reference),
        reinterpret_cast<int>(nameData),
        reinterpret_cast<int>(colorBytes));

    self->field_0063 = 1;
    self->field_0062 = 0;
}

HOOK(0x004D2A20, C_0083ba40_ActivatePlayerAlert, "auto");
