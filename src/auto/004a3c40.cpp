#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, C_00833908_GetTargetName, 0x0048FE90, (C_00833908* self, void*));
CSF_FUNC(int, __fastcall, C_008344f0_SubmitAnimation, 0x004A3AE0, (C_008344f0* self, void* edx, uint32_t a2, uint32_t a3, uint32_t a4));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, sub_519B50, 0x00519B50, (void* self, void* edx, int a2, unsigned int* a3));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));
CSF_FUNC(int, __fastcall, AccessorState_ResolveSlot, 0x006C8EE0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_fZero, 0x82D4CC);
CSF_GLOBAL(float, g_SceneLoadMillisecondsToSeconds, 0x82D4F0);

static uint32_t __fastcall C_008344f0_PlayAnimation(C_008344f0* self, void* /*edx*/,
                                                    uint32_t a4, uint32_t a5, float a6,
                                                    uint32_t a7, float a8)
{
    // The original keeps the parameter block (0x20 bytes) followed by the saved `this` at +0x20.
    alignas(8) uint8_t paramBytes[0x24];
    CSF_AT(C_008344f0*, paramBytes, 0x20) = self;

    if (a5 == 0xFFFFFFFFu)
        return 0xFFFFFFFFu;

    int sceneSetup = CSF_AT(int, reinterpret_cast<void*>(GetValue8B0554()), 0x1FC4);
    int entry = SceneMissionSetup_GetAnimationEntry(reinterpret_cast<void*>(sceneSetup), nullptr, (int)a5);

    int globalInt = GetGlobalInt();
    int accessor = GlobalInt_ResolveAccessor(reinterpret_cast<void*>(globalInt), nullptr);
    int accessorState = ((int(__fastcall*)(void*, void*))CSF_AT(void**, reinterpret_cast<void*>(accessor), 0)[5])(
        reinterpret_cast<void*>(accessor), nullptr);

    int slotArg = sub_519B50(reinterpret_cast<void*>(sceneSetup), nullptr, (int)a5, &a7);
    int slot = AccessorState_ResolveSlot(reinterpret_cast<void*>(accessorState), nullptr, slotArg);
    if (!slot)
        return 0xFFFFFFFFu;

    C_008344f0* thisA = CSF_AT(C_008344f0*, paramBytes, 0x20);
    void* owner = reinterpret_cast<void*>(thisA->field_0008);
    void* obj = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[10])(owner, nullptr);

    uint8_t* flags = ((uint8_t*(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[8])(obj, nullptr);
    if ((*flags & 1) == 0) {
        C_008344f0* thisB = CSF_AT(C_008344f0*, paramBytes, 0x20);
        C_00833908_GetTargetName(reinterpret_cast<C_00833908*>(thisB->field_0008), nullptr);
        return 0xFFFFFFFFu;
    }

    void* target = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[26])(obj, nullptr);

    uint8_t* entryPtr = reinterpret_cast<uint8_t*>(entry);

    CSF_AT(float, paramBytes, 0x04) = CSF_AT(float, entryPtr, 0x34) * g_SceneLoadMillisecondsToSeconds;
    CSF_AT(uint8_t, paramBytes, 0x0D) = CSF_AT(uint8_t, entryPtr, 0x59);
    CSF_AT(uint8_t, paramBytes, 0x0E) = CSF_AT(uint8_t, entryPtr, 0x5A);
    CSF_AT(uint8_t, paramBytes, 0x0C) = CSF_AT(uint8_t, entryPtr, 0x58);
    CSF_AT(uint32_t, paramBytes, 0x00) = 0;
    CSF_AT(uint32_t, paramBytes, 0x08) = 0;
    CSF_AT(float, paramBytes, 0x10) = 1.0f;
    CSF_AT(float, paramBytes, 0x14) = 1.0f;
    CSF_AT(float, paramBytes, 0x18) = 1.0f;
    CSF_AT(uint8_t, paramBytes, 0x1C) = 0;
    CSF_AT(uint8_t, paramBytes, 0x1D) = 1;

    double value = C_Unknown_GetValue(reinterpret_cast<void*>(slot), nullptr);
    if (value >= (double)a8)
        CSF_AT(float, paramBytes, 0x00) = a8;
    else
        CSF_AT(float, paramBytes, 0x00) = (float)C_Unknown_GetValue(reinterpret_cast<void*>(slot), nullptr);

    char ok = ((char(__fastcall*)(void*, void*, int, int, void*))CSF_AT(void**, target, 0)[5])(
        target, nullptr, 0, slot, paramBytes);
    if (!ok)
        return 0xFFFFFFFFu;

    C_008344f0* cur = CSF_AT(C_008344f0*, paramBytes, 0x20);
    C_008344f0_SubmitAnimation(cur, nullptr, a4, a5, a7);

    if (a6 <= g_fZero) {
        if (CSF_AT(uint8_t, paramBytes, 0x0C) == 0) {
            double v = C_Unknown_GetValue(reinterpret_cast<void*>(slot), nullptr);
            cur->field_0064 = (float)(v - (double)CSF_AT(float, paramBytes, 0x00));
            return a7;
        }
        cur->field_0064 = FLT_MAX;
        return a7;
    }

    cur->field_0064 = a6 - CSF_AT(float, paramBytes, 0x00);
    return a7;
}

HOOK(0x004A3C40, C_008344f0_PlayAnimation, "auto");
