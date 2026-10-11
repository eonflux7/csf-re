#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, CoverAttack_TestPositionAgainstObject, 0x004E5A30, (void* self, void* edx, float* a3, uint32_t* a4));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);
CSF_GLOBAL(float, g_ZeroVectorX, 0x008BB420);
CSF_GLOBAL(float, g_ZeroVectorY, 0x008BB424);
CSF_GLOBAL(float, g_ZeroVectorZ, 0x008BB428);

static int __fastcall AiCoverAttackTasks_TestCoverPosition(C_0083e9d0* self, void* /*edx*/, float* arg0, int a2, int a4)
{
    uint8_t* owner = CSF_AT(uint8_t*, self, 0x8);
    if (CSF_AT(int, reinterpret_cast<void*>(a2), 4) == CSF_AT(int, owner, 0x20))
        return 1;

    void** obj = reinterpret_cast<void**>(CHandleTable_Resolve(g_pCurrentScene, nullptr, a2));
    if (!obj)
        return 0;
    if (a4 != 10 && (SceneObj_IsInactive(obj, nullptr) & 0xFF) != 0)
        return 0;

    // [0..2] = position passed to the callee, [3..5] = last queried point
    float pos[6];
    pos[3] = g_ZeroVectorX;
    pos[4] = g_ZeroVectorY;
    pos[5] = g_ZeroVectorZ;
    pos[0] = arg0[0];
    pos[1] = arg0[1];
    pos[2] = arg0[2];

    uint8_t tmp[0x10];
    bool ok = true;
    uint32_t i = 0;

    void** vtbl = CSF_AT(void**, obj, 0);
    uint32_t count = ((uint32_t(__fastcall*)(void*, void*))vtbl[0x3C / 4])(obj, nullptr);
    if (count) {
        do {
            if (!ok)
                break;
            void** vt = CSF_AT(void**, obj, 0);
            float* p = ((float*(__fastcall*)(void*, void*, void*, uint32_t))vt[0x40 / 4])(obj, nullptr, tmp, i);
            pos[3] = p[0];
            pos[4] = p[1];
            pos[5] = p[2];
            ok = !CoverAttack_TestPositionAgainstObject(self, nullptr, pos, reinterpret_cast<uint32_t*>(obj));
            ++i;
        } while (i < count);
    }
    return !ok;
}

HOOK(0x004E5C70, AiCoverAttackTasks_TestCoverPosition, "auto");
