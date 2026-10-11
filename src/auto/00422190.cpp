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

// Returns the float result in st0 (the original returns it on the x87 stack).
CSF_FUNC(float, __fastcall, TaskState_GetTurnValue, 0x0041E440, (void* self, void* edx));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_SoundActivityThreshold, 0x0082E114);
CSF_GLOBAL(float, g_HudNegativeHalfPi, 0x0082FACC);
CSF_GLOBAL(float, g_TransitionHalfPi, 0x0082FAD0);
CSF_GLOBAL(float, g_TurnScale, 0x0082FAD8);

typedef void* (__fastcall* VCall0)(void*, void*);
typedef void* (__fastcall* VCall1)(void*, void*, int);
typedef void* (__fastcall* VCall4)(void*, void*, int, int, int, int);

static char __fastcall TaskState_UpdateTurn(void* self, void* /*edx*/)
{
    uint8_t* me = reinterpret_cast<uint8_t*>(self);
    uint8_t* state = me + 0x244;

    float turn = TaskState_GetTurnValue(state, nullptr);

    bool finished = CSF_AT(uint8_t, me, 0x230) != 0 || fabs(turn) < g_SoundActivityThreshold;
    CSF_AT(uint8_t, me, 0x204) = finished ? 1 : 0;
    bool positive = turn > g_fZero;
    CSF_AT(uint8_t, state, 0) = positive ? 1 : 0;

    int result = positive ? 1 : 0;
    if (finished)
        return (char)result;

    void* owner = CSF_AT(void*, me, 0x14);
    void* anim = CSF_AT(void*, owner, 0x48);
    ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, anim, 0)[0x38 / 4])(anim, nullptr, self);

    owner = CSF_AT(void*, me, 0x14);
    int code;
    if (turn <= g_fZero) {
        if (turn >= g_HudNegativeHalfPi)
            code = 26;
        else
            code = 24;
    } else if (turn <= g_TransitionHalfPi) {
        code = 27;
    } else {
        code = 25;
    }
    ((VCall4)CSF_AT(void**, owner, 0)[0x8C / 4])(owner, nullptr, code, 0, -1, 0);

    owner = CSF_AT(void*, me, 0x14);
    void* edi = CSF_AT(void*, owner, 0x48);
    int r = (int)(intptr_t)((VCall0)CSF_AT(void**, edi, 0)[0x64 / 4])(edi, nullptr);
    if (r == -1) {
        CSF_AT(uint8_t, me, 0x204) = 1;
        return (char)r;
    }

    void* o1 = CSF_AT(void*, me, 0x14);
    void* o2 = ((VCall0)CSF_AT(void**, o1, 0)[0x28 / 4])(o1, nullptr);
    void* o3 = ((VCall0)CSF_AT(void**, o2, 0)[0x68 / 4])(o2, nullptr);
    void* p = ((VCall1)CSF_AT(void**, o3, 0)[0x20 / 4])(o3, nullptr, 0);
    if (!p)
        return (char)(intptr_t)p;

    double value = C_Unknown_GetValue(p, nullptr);
    float absTurn = (float)fabs(turn);
    CSF_AT(int, state, 0x10) = 0;
    CSF_AT(float, state, 0x14) = (float)(value * absTurn) * g_TurnScale;

    void* q = ((VCall0)CSF_AT(void**, edi, 0)[0x68 / 4])(edi, nullptr);
    if (CSF_AT(uint8_t, q, 0x5A) != 0)
        return (char)(intptr_t)q;

    CSF_AT(uint8_t, me, 0x204) = 1;
    void* last = ((VCall0)CSF_AT(void**, edi, 0)[0x68 / 4])(edi, nullptr);
    return (char)(intptr_t)last;
}

HOOK(0x00422190, TaskState_UpdateTurn, "auto");
