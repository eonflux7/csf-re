#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __cdecl, MpSpawnPoints_RegisterObject, 0x00475850, (int object));
CSF_FUNC(int, __fastcall, C_00844694_Initialize, 0x00551E20, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, MenuMpAccounts_CreateListSentinel, 0x00552DD0, ());
CSF_FUNC(int, __cdecl, MenuMpAccounts_CreateObject8453A4, 0x0055BDE0, ());
CSF_FUNC(void*, __fastcall, C_00867768_Construct, 0x0073F700, (void* self, void* edx));

CSF_GLOBAL(uint32_t, g_CharacterSelectState8B0694, 0x008B0694);

static uint32_t* __fastcall C_00844694_Construct(
    C_00844694* self, void* /*edx*/, int parameter)
{
    CSF_AT(void**, self, 0) = CSF_VTBL(0x00844694);

    auto* entry = reinterpret_cast<uint8_t*>(self) + 0x20;
    for (int remaining = 50; remaining; --remaining, entry += 64)
    {
        CSF_AT(uint32_t, entry, 4) = 7;
        CSF_AT(uint32_t, entry, 0) = 0;
        CSF_AT(uint16_t, entry, -0x10) = 0;
    }

    C_00867768_Construct(reinterpret_cast<uint8_t*>(self) + 0xC94, nullptr);

    entry = reinterpret_cast<uint8_t*>(self) + 0xCC0;
    for (int remaining = 32; remaining; --remaining, entry += 56)
    {
        CSF_AT(uint32_t, entry, 4) = 7;
        CSF_AT(uint32_t, entry, 0) = 0;
        CSF_AT(uint16_t, entry, -0x10) = 0;
    }

    self->field_1404 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_1408 = -1;
    self->field_140c = -1;
    self->field_1410 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_1414 = -1;
    self->field_1418 = -1;

    CSF_AT(uint32_t, self, 0x1468) =
        reinterpret_cast<uint32_t>(MenuMpAccounts_CreateListSentinel());
    CSF_AT(uint32_t, self, 0x146C) = 0;

    self->field_1490 = 0;
    self->field_1494 = 0;
    self->field_1498 = 0;
    self->field_149c = 10;

    C_00844694_Initialize(self, nullptr);
    MpSpawnPoints_RegisterObject(reinterpret_cast<int>(self));

    void* accountObject =
        reinterpret_cast<void*>(MenuMpAccounts_CreateObject8453A4());
    self->field_1484 = reinterpret_cast<uint32_t>(accountObject);
    if (accountObject)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, accountObject, 0)[1])(accountObject, nullptr);
    }

    void* selectionState = reinterpret_cast<void*>(g_CharacterSelectState8B0694);
    void* selectionObject = CSF_AT(void*, selectionState, 4);
    self->field_1480 = static_cast<uint8_t>(
        reinterpret_cast<int (__fastcall*)(void*, void*)>(
            CSF_AT(void**, selectionObject, 0)[2])(selectionObject, nullptr));

    self->field_13bc = parameter;
    self->field_14a8 = 2;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x00555550, C_00844694_Construct, "auto");
