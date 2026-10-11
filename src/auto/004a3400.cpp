#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, sub_519B50, 0x00519B50, (void* self, void* edx, int a2, unsigned int* a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

typedef int (__fastcall* VCall0)(void* self, void* edx);
typedef uint8_t (__fastcall* VCallApplyB)(void* self, void* edx, int a, int b, void* c);

static void __fastcall C_008344f0_ApplyAnimationParams(C_008344f0* self, void* /*edx*/)
{
    if (self->field_0048 == 0xFFFFFFFFu || self->field_004c == 0xFFFFFFFFu)
        return;

    // Frame block: 0x20 bytes of parameters followed by the animation entry local at +0x20 (ebp-4).
    alignas(8) uint8_t params[0x24];

    int scene = GetValue8B0554();
    int sceneSetup = CSF_AT(int, reinterpret_cast<void*>(scene), 0x1FC4);

    CSF_AT(int, params, 0x20) = SceneMissionSetup_GetAnimationEntry(
        reinterpret_cast<void*>(sceneSetup), nullptr, static_cast<int>(self->field_0048));

    int globalInt = GetGlobalInt();
    void* accessorObj = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(reinterpret_cast<void*>(globalInt), nullptr));
    int accessor = ((VCall0)CSF_AT(void**, accessorObj, 0)[0x14 / 4])(accessorObj, nullptr);

    int key = sub_519B50(reinterpret_cast<void*>(sceneSetup), nullptr, static_cast<int>(self->field_0048),
                         reinterpret_cast<unsigned int*>(reinterpret_cast<uint8_t*>(self) + 0x4C));
    int slot = AccessorState_ResolveSlot(reinterpret_cast<void*>(accessor), nullptr, key);
    if (slot == 0)
        return;

    void* owner = reinterpret_cast<void*>(self->field_0008);
    void* inner = reinterpret_cast<void*>(((VCall0)CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr));

    uint8_t* flags = reinterpret_cast<uint8_t*>(((VCall0)CSF_AT(void**, inner, 0)[0x20 / 4])(inner, nullptr));
    if ((*flags & 1) == 0)
        C_00833908_GetTargetName(reinterpret_cast<C_00833908*>(self->field_0008), nullptr);

    void* target = reinterpret_cast<void*>(((VCall0)CSF_AT(void**, inner, 0)[0x68 / 4])(inner, nullptr));

    uint8_t* entry = reinterpret_cast<uint8_t*>(CSF_AT(int, params, 0x20));
    uint8_t b58 = entry[0x58];
    uint8_t b59 = entry[0x59];
    uint8_t b5a = entry[0x5A];

    CSF_AT(uint8_t, params, 0x0E) = b5a;
    CSF_AT(uint8_t, params, 0x0C) = b58;
    CSF_AT(float, params, 0x00) = self->field_0060;
    CSF_AT(float, params, 0x08) = 0.0f;
    CSF_AT(float, params, 0x10) = 1.0f;
    CSF_AT(float, params, 0x14) = 1.0f;
    CSF_AT(float, params, 0x18) = 1.0f;
    CSF_AT(uint8_t, params, 0x1C) = 0;
    CSF_AT(uint8_t, params, 0x1D) = 1;
    CSF_AT(uint8_t, params, 0x0D) = b59;
    CSF_AT(float, params, 0x04) = 0.0f;

    VCallApplyB apply = (VCallApplyB)CSF_AT(void**, target, 0)[0x14 / 4];
    uint8_t ok = apply(target, nullptr, 0, slot, params);
    if (ok && self->field_0064 == g_fZero && self->field_0060 == g_fZero)
    {
        CSF_AT(float, params, 0x00) =
            static_cast<float>(C_Unknown_GetValue(reinterpret_cast<void*>(slot), nullptr));
        apply = (VCallApplyB)CSF_AT(void**, target, 0)[0x14 / 4];
        apply(target, nullptr, 0, slot, params);
    }
}

HOOK(0x004A3400, C_008344f0_ApplyAnimationParams, "auto");
