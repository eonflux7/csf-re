#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(std::uint32_t*, __fastcall, WeaponFirearms_InitializeState, 0x00475A10, (std::uint32_t* self, void*));
CSF_FUNC(int, __cdecl, sub_4920D0, 0x004920D0, (int a1));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, sub_519B50, 0x00519B50, (void* self, void* edx, int a2, unsigned int* a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

static void __fastcall WeaponAction_TriggerAnimation(C_00834598* self, void* /*edx*/)
{
    if (CSF_AT(int, self, 0x48) == -1)
        return;

    int globalBase = GetValue8B0554();
    void* sceneSetup = reinterpret_cast<void*>(CSF_AT(uint32_t, reinterpret_cast<void*>(globalBase), 0x1FC4));

    uint8_t* animEntry = reinterpret_cast<uint8_t*>(
        SceneMissionSetup_GetAnimationEntry(sceneSetup, nullptr, CSF_AT(int, self, 0x48)));

    int globalInt = GetGlobalInt();
    int accessor = GlobalInt_ResolveAccessor(reinterpret_cast<void*>(globalInt), nullptr);
    void** accessorVtbl = CSF_AT(void**, reinterpret_cast<void*>(accessor), 0);
    int state = ((int(__fastcall*)(void*, void*))accessorVtbl[0x14 / 4])(reinterpret_cast<void*>(accessor), nullptr);

    int slotValue = sub_519B50(sceneSetup, nullptr, CSF_AT(int, self, 0x48),
                               reinterpret_cast<unsigned int*>(reinterpret_cast<uint8_t*>(self) + 0x4C));
    int resolved = AccessorState_ResolveSlot(reinterpret_cast<void*>(state), nullptr, slotValue);

    void* owner = reinterpret_cast<void*>(self->field_0008);
    void** ownerVtbl = CSF_AT(void**, owner, 0);
    void* inner = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))ownerVtbl[0x28 / 4])(owner, nullptr));
    void** innerVtbl = CSF_AT(void**, inner, 0);
    void* target = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))innerVtbl[0x68 / 4])(inner, nullptr));
    void** targetVtbl = CSF_AT(void**, target, 0);

    bool busy = ((bool(__fastcall*)(void*, void*))targetVtbl[0x58 / 4])(target, nullptr);
    if (busy)
        return;

    targetVtbl = CSF_AT(void**, target, 0);
    int r = ((int(__fastcall*)(void*, void*, int))targetVtbl[0x18 / 4])(target, nullptr, resolved);
    if (r == -1 || r != 0)
        return;

    void** selfVtbl = CSF_AT(void**, self, 0);
    ((void(__fastcall*)(void*, void*, int))selfVtbl[0x80 / 4])(self, nullptr, CSF_AT(int, self, 0x44));

    alignas(8) uint8_t bufBytes[0x28];
    std::uint32_t* buf = reinterpret_cast<std::uint32_t*>(bufBytes);
    WeaponFirearms_InitializeState(buf, nullptr);

    uint8_t b59 = animEntry[0x59];
    uint8_t b58 = animEntry[0x58];
    uint8_t b5a = animEntry[0x5A];
    bufBytes[0x0C] = b58;
    bufBytes[0x0D] = b59;
    bufBytes[0x0E] = b5a;
    bufBytes[0x1C] = static_cast<uint8_t>(sub_4920D0(CSF_AT(int, self, 0x44)));
    uint8_t flag = 0;
    if (b59 || b5a)
        flag = 1;
    bufBytes[0x1D] = flag;
    buf[0] = CSF_AT(uint32_t, self, 0x60);
    buf[1] = 0;

    targetVtbl = CSF_AT(void**, target, 0);
    ((void(__fastcall*)(void*, void*, int, int, void*))targetVtbl[0x14 / 4])(target, nullptr, 0, resolved, bufBytes);
}

HOOK(0x004A5660, WeaponAction_TriggerAnimation, "auto");
