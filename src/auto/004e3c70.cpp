#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_RemoveObjectFromCategories, 0x004528D0, (CScene* self, void* , int handleAddress, int categoryMask));
CSF_FUNC(int, __fastcall, SceneObject_PrepareStateReset, 0x00493270, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083e248_ProcessActiveState, 0x004E3610, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083e248_ApplyPendingState(C_0083e248* self, void* /*edx*/)
{
    int result = static_cast<int>(self->field_0018);

    switch (result)
    {
    case 2:
        return C_0083e248_ProcessActiveState(self, nullptr);

    case 3:
    {
        SceneObject_PrepareStateReset(
            reinterpret_cast<void*>(self->field_000c), nullptr);

        void* flagsObject = CSF_AT(
            void*, reinterpret_cast<void*>(self->field_000c), 0x34);
        CSF_AT(uint32_t, flagsObject, 0x18) |= 2u;

        auto* interfaceObject =
            reinterpret_cast<uint8_t*>(self->field_000c) + 4;
        alignas(8) uint8_t argumentBytes[0x1];
        argumentBytes[0] = 0;
        result = reinterpret_cast<int(__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, interfaceObject, 0)[0x50 / 4])(
                interfaceObject, nullptr,
                reinterpret_cast<char*>(argumentBytes));

        self->field_0018 = 0;
        return result;
    }

    case 4:
    case 6:
    {
        void* object = reinterpret_cast<void*>(self->field_000c);
        if (result == 6)
            result = static_cast<int>(self->field_000c);

        self->field_0018 = 7;
        if ((CSF_AT(uint8_t, object, 0x94) & 1) != 0)
        {
            result = CScene_RemoveObjectFromCategories(
                reinterpret_cast<CScene*>(g_pCurrentScene), nullptr,
                reinterpret_cast<int>(
                    reinterpret_cast<uint8_t*>(self->field_000c) + 0x1C),
                1);
        }
        self->field_0024 = 0.0f;
        return result;
    }

    default:
        return result;
    }
}

HOOK(0x004E3C70, C_0083e248_ApplyPendingState, "auto");
