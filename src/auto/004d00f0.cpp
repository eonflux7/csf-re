#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(char, __fastcall, C_0083b580_UpdateWhenObjectReady, 0x004CFFD0, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, C_0083b580_UpdateWhenObjectNotReady, 0x004D0040, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static char __fastcall C_0083b580_UpdateTrackedObject(
    C_0083b580* self, void* /*edx*/, int updateArgument)
{
    const uint32_t trackedAddress = self->field_0054;
    auto* owner = reinterpret_cast<C_00833908*>(self->field_0014);

    if (!trackedAddress)
    {
        if (owner)
        {
            const uint32_t componentAddress = CSF_AT(uint32_t, owner, 0x18);
            if (componentAddress)
            {
                const uint32_t objectAddress = componentAddress - 128;
                self->field_0054 = objectAddress;
                return static_cast<char>(objectAddress);
            }
        }

        self->field_0054 = 0;
        return 0;
    }

    uint32_t currentAddress = 0;
    if (owner)
    {
        const uint32_t componentAddress = CSF_AT(uint32_t, owner, 0x18);
        if (componentAddress)
            currentAddress = componentAddress - 128;
    }

    if (currentAddress != trackedAddress)
    {
        const auto slotAddress = reinterpret_cast<std::uintptr_t>(owner) + 0x1C;
        return static_cast<char>(CScene_RemoveObjectSlot(
            reinterpret_cast<CScene*>(g_pCurrentScene),
            nullptr,
            static_cast<int>(slotAddress)));
    }

    auto* object = reinterpret_cast<uint8_t*>(currentAddress);
    void* component = object + 0x80;

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, component, 0)[0xC / 4])(
            component, nullptr, updateArgument);

    const uint32_t firstState = CSF_AT(uint32_t, object, 0x68);
    const uint32_t secondState = CSF_AT(uint32_t, object, 0x6C);
    if (self->field_0060 != firstState || self->field_0064 != secondState)
    {
        self->field_0060 = firstState;
        self->field_0064 = secondState;
        self->field_0058 = 0;
    }

    const bool ready = reinterpret_cast<bool (__fastcall*)(void*, void*)>(
        CSF_AT(void**, component, 0)[8 / 4])(component, nullptr);

    if (ready)
        return C_0083b580_UpdateWhenObjectReady(self, nullptr, updateArgument);

    return C_0083b580_UpdateWhenObjectNotReady(self, nullptr, updateArgument);
}

HOOK(0x004D00F0, C_0083b580_UpdateTrackedObject, "auto");
