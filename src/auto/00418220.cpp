#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(int, __fastcall, C_0082e128_EndTimedAction, 0x00416000,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, C_0082e128_ApplyTransition, 0x00416450,
         (void* self, void* edx, int a2, void* a3, int a4));
CSF_FUNC(void, __fastcall, C_0082e128_UpdateState, 0x00417570,
         (void* self, void* edx, int a2));

static void __fastcall C_0082e128_UpdateTimers(
    C_0082e128* self, void* /*edx*/, int timeBits)
{
    if (!self->field_000c)
        return;

    float elapsed;
    std::memcpy(&elapsed, &timeBits, sizeof(elapsed));

    if (self->field_01fc)
    {
        void* owner = reinterpret_cast<void*>(self->field_0004);
        float remaining = self->field_020c - elapsed;
        self->field_020c = remaining;

        if (CSF_AT(int, owner, 0x50) != 10 ||
            remaining < g_fZero ||
            self->field_0210)
        {
            C_0082e128_EndTimedAction(self, nullptr, &self->field_0200);
            self->field_01fc = 0;
            self->field_0210 = 0;
        }
    }

    void* owner = reinterpret_cast<void*>(self->field_0004);
    if (owner)
    {
        void* child = CSF_AT(void*, owner, 0x18);
        if (child)
        {
            reinterpret_cast<void(__fastcall*)(void*, void*)>(
                CSF_AT(void**, child, 0)[0x8 / 4])(child, nullptr);
        }
    }

    C_0082e128_UpdateState(self, nullptr, timeBits);

    if (self->field_01f4)
    {
        float remaining = self->field_01f8 - elapsed;
        self->field_01f8 = remaining;

        if (remaining < g_fZero)
        {
            alignas(8) uint8_t transitionBytes[0xC] = {};
            C_0082e128_ApplyTransition(
                self, nullptr,
                static_cast<int>(reinterpret_cast<intptr_t>(transitionBytes)),
                &self->field_018c, 0);
        }
    }
}

HOOK(0x00418220, C_0082e128_UpdateTimers, "auto");
