#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, Object_SerializeBase, 0x004A3DD0, (int a1, int edx0, void* a2));
CSF_FUNC(unsigned int, __fastcall, Object_SetAnimationEntry, 0x004A5960, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, Object_SerializeList, 0x004A5A70, (unsigned int a1, int a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneMissionSetup_GetAnimationEntry, 0x00772120, (void* self, void* edx, int a2));

typedef char (__fastcall* StreamCheckFn)(void* self, void* edx, int zero, void* ctx);
typedef int (__fastcall* StreamIoFn)(void* self, void* edx, void* data, void* ctx);
typedef int (__fastcall* StreamFinishFn)(void* self, void* edx, void* ctx);

static int __fastcall AnimatedObject_Serialize(void* self, void* /*edx*/, void* ctx)
{
    uint8_t* c = reinterpret_cast<uint8_t*>(ctx);
    uint8_t* me = reinterpret_cast<uint8_t*>(self);

    // high byte of the argument pointer is the original's flag byte
    uint8_t flag = (uint8_t)((uintptr_t)ctx >> 24);

    void* stream;
    void** vtbl;

    uint8_t err;
    if (CSF_AT(uint8_t, c, 0) == 0) {
        stream = CSF_AT(void*, c, 4);
        vtbl = CSF_AT(void**, stream, 0);
        char r = ((StreamCheckFn)vtbl[0x80 / 4])(stream, nullptr, 0, ctx);
        err = (r == 0) ? 1 : 0;
    } else {
        err = 0;
    }

    int savedValue = CSF_AT(int, c, 0x1C);
    CSF_AT(uint8_t, c, 0) = err;
    Object_SerializeBase((int)(uintptr_t)self, 0, ctx);

    bool ok = CSF_AT(uint8_t, c, 0) == 0;
    CSF_AT(int, c, 0x1C) = 0;
    if (ok) {
        CSF_AT(uint8_t, c, 0x15) = 0;
        CSF_AT(uint8_t, c, 0x16) = 0;
        Object_SerializeList((unsigned int)(uintptr_t)(me + 0x78), (int)(uintptr_t)ctx);
    }

    if (CSF_AT(uint8_t, c, 0x14) != 0)
        flag = (CSF_AT(int, me, 0x88) != 0);

    ok = CSF_AT(uint8_t, c, 0) == 0;
    CSF_AT(int, c, 0x1C) = 0;
    if (ok) {
        stream = CSF_AT(void*, c, 4);
        CSF_AT(uint8_t, c, 0x15) = 0;
        CSF_AT(uint8_t, c, 0x16) = 0;
        vtbl = CSF_AT(void**, stream, 0);
        int r = ((StreamIoFn)vtbl[0x44 / 4])(stream, nullptr, &flag, ctx);
        CSF_AT(uint8_t, c, 0) = (r == -1);
    }

    if (flag != 0) {
        ok = CSF_AT(uint8_t, c, 0) == 0;
        CSF_AT(int, c, 0x1C) = 0;
        if (ok) {
            stream = CSF_AT(void*, c, 4);
            CSF_AT(uint8_t, c, 0x15) = 0;
            CSF_AT(uint8_t, c, 0x16) = 0;
            vtbl = CSF_AT(void**, stream, 0);
            int r = ((StreamIoFn)vtbl[0x48 / 4])(stream, nullptr, me + 0x44, ctx);
            CSF_AT(uint8_t, c, 0) = (r == -1);
        }

        if (CSF_AT(uint8_t, c, 0x14) == 0 && CSF_AT(uint8_t, c, 0) == 0) {
            int g = GetValue8B0554();
            int entry = SceneMissionSetup_GetAnimationEntry(
                CSF_AT(void*, g, 0x1FC4), nullptr, CSF_AT(int, me, 0x44));
            Object_SetAnimationEntry(me - 4, nullptr, entry);
        }

        ok = CSF_AT(uint8_t, c, 0) == 0;
        CSF_AT(int, c, 0x1C) = 0;
        if (ok) {
            CSF_AT(uint8_t, c, 0x15) = 1;
            CSF_AT(uint8_t, c, 0x16) = 0;
            SceneLoader_SerializeReference(me + 0x88, reinterpret_cast<bool*>(ctx));
        }
    }

    ok = CSF_AT(uint8_t, c, 0) == 0;
    CSF_AT(int, c, 0x1C) = savedValue;
    if (ok) {
        stream = CSF_AT(void*, c, 4);
        vtbl = CSF_AT(void**, stream, 0);
        int r = ((StreamFinishFn)vtbl[0x7C / 4])(stream, nullptr, ctx);
        if ((uint8_t)r != 0) {
            CSF_AT(uint8_t, c, 0) = 0;
            return r;
        }
    }
    CSF_AT(uint8_t, c, 0) = 1;
    return 1;
}

HOOK(0x004A5DE0, AnimatedObject_Serialize, "auto");
