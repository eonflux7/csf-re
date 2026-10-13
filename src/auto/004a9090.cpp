#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t, __fastcall, GetSceneMenuContext, 0x0044FC60, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, WeaponFirearms_ApplyRecoil, 0x00495500, (void* self, void* , float a1));
CSF_FUNC(uint32_t*, __fastcall, C_0083c280_DispatchItemAction, 0x004D5720, (void* self, void* edx, uint32_t* a2, char a3));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint32_t* __fastcall C_00835228_ActivateItemAndApplyRecoil(
    C_00835228* self, void* /*edx*/, int context, int action)
{
    void* owner = CSF_AT(void*, reinterpret_cast<void*>(self->field_0014), 0x44);
    void** ownerVtable = CSF_AT(void**, owner, 0);

    int itemKey =
        reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            ownerVtable[0xBC / 4])(owner, nullptr, 20);

    C_00862490* item =
        reinterpret_cast<C_00862490* (__fastcall*)(void*, void*, int)>(
            ownerVtable[0xCC / 4])(owner, nullptr, itemKey);

    if (!item)
        return nullptr;

    int count = GetCurrentItemCount(item, nullptr);
    if (count <= 0)
        return reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(count));

    bool notified =
        reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, item, 0)[0x2C / 4])(item, nullptr);
    if (!notified)
        return nullptr;

    uint32_t menuContext = GetSceneMenuContext(g_pCurrentScene, nullptr);
    char isCurrentContext = static_cast<uint32_t>(context) == menuContext;

    C_0083c280_DispatchItemAction(
        self, nullptr, reinterpret_cast<uint32_t*>(action), isCurrentContext);

    int valueAddress = Handle_GetValuePointer(item, nullptr);
    void* value = reinterpret_cast<void*>(valueAddress);
    if (!isCurrentContext)
        return reinterpret_cast<uint32_t*>(valueAddress);

    alignas(8) uint8_t recoilBytes[0x10] = {};
    uint32_t first = CSF_AT(uint32_t, value, 0xF8);
    uint32_t second = CSF_AT(uint32_t, value, 0xFC);
    CSF_AT(uint32_t, recoilBytes, 0x0) = first;
    uint32_t third = CSF_AT(uint32_t, value, 0x100);
    CSF_AT(uint32_t, recoilBytes, 0x4) = second;
    CSF_AT(uint8_t, recoilBytes, 0x9) = 0;
    CSF_AT(uint32_t, recoilBytes, 0xC) = third;
    CSF_AT(uint8_t, recoilBytes, 0x8) = 1;

    // The forwarder's float parameter carries the pointer's raw stack-slot bits.
    uint32_t recoilAddress = reinterpret_cast<uintptr_t>(recoilBytes);
    float recoilArgument;
    std::memcpy(&recoilArgument, &recoilAddress, sizeof(recoilArgument));

    return reinterpret_cast<uint32_t*>(WeaponFirearms_ApplyRecoil(
        reinterpret_cast<void*>(menuContext), nullptr, recoilArgument));
}

HOOK(0x004A9090, C_00835228_ActivateItemAndApplyRecoil, "auto");
