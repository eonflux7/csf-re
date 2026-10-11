#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TaskState_UpdateBase, 0x00420260, (void* self, void* edx, float a3));
CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(char, __fastcall, Owner_TryStartTransition, 0x0048E5D0, (void* self, void* edx, int a2, int a3, int a4));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static void CallOwnerSlot35(void* owner, int a, int b, int c, int d)
{
    typedef void(__fastcall * Fn)(void*, void*, int, int, int, int);
    ((Fn)CSF_AT(void**, owner, 0)[0x8C / 4])(owner, nullptr, a, b, c, d);
}

static unsigned int __fastcall C_0082e7c8_Update(C_0082e7c8* self, void* /*edx*/, float arg0)
{
    void* owner = reinterpret_cast<void*>(self->C_0082e598::field_0014);

    if (CSF_AT(uint32_t, owner, 0x50) || (CSF_AT(uint32_t, owner, 0x2E0) & 0x10000) == 0)
        TaskState_UpdateBase(self, nullptr, arg0);

    typedef unsigned int(__fastcall * SlotFn)(void*, void*, int);
    void* sub = CSF_AT(void*, CSF_AT(void*, self, 0x14), 0x48);
    void** subVtbl = CSF_AT(void**, sub, 0);
    unsigned int result;

    if ((unsigned char)((SlotFn)subVtbl[0x10 / 4])(sub, nullptr, (int)(intptr_t)self)) {
        goto active;
    }
    sub = CSF_AT(void*, CSF_AT(void*, self, 0x14), 0x48);
    subVtbl = CSF_AT(void**, sub, 0);
    result = ((SlotFn)subVtbl[0x24 / 4])(sub, nullptr, 0);
    if ((unsigned char)result) {
        goto active;
    }
    self->field_0024 = 1;
    return result;

active:
    if (self->field_0020 != self->field_0024) {
        self->field_0020 = self->field_0024;
        int v = (int)self->field_0024 - 1;
        void* cur = reinterpret_cast<void*>(self->C_0082e598::field_0014);
        if (v == 0) {
            if (Owner_TryStartTransition(cur, nullptr, (int)(intptr_t)self, 0, 0))
                CallOwnerSlot35(reinterpret_cast<void*>(self->C_0082e598::field_0014), 0, 0, -1, 0);
            self->field_0028 = 0;
        } else if (v == 1) {
            if (CSF_AT(uint32_t, cur, 0x1E0) == 1) {
                if (Owner_TryStartTransition(cur, nullptr, (int)(intptr_t)self, 0, 0))
                    CallOwnerSlot35(reinterpret_cast<void*>(self->C_0082e598::field_0014), 3, 0, -1, 0);
                self->field_0028 = 3;
            } else {
                if (Owner_TryStartTransition(cur, nullptr, (int)(intptr_t)self, 0, 0))
                    CallOwnerSlot35(reinterpret_cast<void*>(self->C_0082e598::field_0014), 1, 0, -1, 0);
                self->field_0028 = 1;
            }
        }
    }

    void* obj = reinterpret_cast<void*>(self->C_0082e598::field_0014);
    uint32_t* pos = reinterpret_cast<uint32_t*>(GetObjectPositionVector(obj, nullptr));
    uint32_t args[7];
    args[0] = 1;
    args[1] = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    args[2] = CSF_AT(uint32_t, obj, 0x20);
    args[3] = CSF_AT(uint32_t, obj, 0x24);
    args[4] = pos[0];
    args[5] = pos[1];
    args[6] = pos[2];
    int global = GetValue8B0554();
    void* dispatcher = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, (void*)(intptr_t)global, 0x1FD8)) + 0x54;
    result = DispatchWithArgBlock(dispatcher, nullptr, args);
    self->field_0024 = 1;
    return result;
}

HOOK(0x004203F0, C_0082e7c8_Update, "auto");
