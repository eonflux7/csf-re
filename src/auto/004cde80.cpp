#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, WeaponFirearms_ApplyRecoil, 0x00495500, (void* self, void* , float a1));
CSF_FUNC(uint32_t*, __fastcall, C_0083c280_DispatchItemAction, 0x004D5720, (C_0083c280* self, void* , uint32_t* itemPayload, char actionFlag));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint32_t* __fastcall C_0083ac40_ApplyRecoilForCurrentItem(
    C_0083ac40* self, void* /*edx*/, int contextId, uint32_t* payload)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* state = CSF_AT(void*, owner, 0x4C);
    if ((CSF_AT(uint8_t, state, 0x34) & 4) != 0)
        return reinterpret_cast<uint32_t*>(owner);

    void* provider = CSF_AT(void*, owner, 0x44);
    void** providerVtable = CSF_AT(void**, provider, 0);
    int itemId =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            providerVtable[0xBC / 4])(provider, nullptr, 14);

    void* item =
        reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
            providerVtable[0xCC / 4])(provider, nullptr, itemId);
    if (!item)
        return nullptr;

    int itemCount = GetCurrentItemCount(item, nullptr);
    if (itemCount <= 0)
        return reinterpret_cast<uint32_t*>(static_cast<uint32_t>(itemCount));

    uint32_t sceneContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    char isCurrentContext = static_cast<uint32_t>(contextId) == sceneContext;
    C_0083c280_DispatchItemAction(reinterpret_cast<C_0083c280*>(self), nullptr, payload, isCurrentContext);

    int valueAddress = Handle_GetValuePointer(item, nullptr);
    void* value = reinterpret_cast<void*>(static_cast<uint32_t>(valueAddress));
    if (!isCurrentContext)
        return reinterpret_cast<uint32_t*>(value);

    alignas(8) uint8_t recoilBytes[0x14]{};
    CSF_AT(uint32_t, recoilBytes, 0x00) = CSF_AT(uint32_t, value, 0xF8);
    CSF_AT(uint32_t, recoilBytes, 0x04) = CSF_AT(uint32_t, value, 0xFC);
    CSF_AT(uint8_t, recoilBytes, 0x08) = 1;
    CSF_AT(uint8_t, recoilBytes, 0x09) = 0;
    CSF_AT(uint32_t, recoilBytes, 0x0C) = CSF_AT(uint32_t, value, 0x100);
    // Slot 0x10 holds the saved `this` (the original's [ebp-4]); same value as self.
    CSF_AT(C_0083ac40*, recoilBytes, 0x10) = self;

    // The legacy float signature forwards this stack slot as a pointer.
    uint32_t recoilAddress =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(recoilBytes));
    float recoilArgument;
    std::memcpy(&recoilArgument, &recoilAddress, sizeof(recoilArgument));

    int result = WeaponFirearms_ApplyRecoil(
        reinterpret_cast<void*>(sceneContext), nullptr, recoilArgument);
    return reinterpret_cast<uint32_t*>(static_cast<uint32_t>(result));
}

HOOK(0x4CDE80, C_0083ac40_ApplyRecoilForCurrentItem, "auto");
