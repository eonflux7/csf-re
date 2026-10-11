#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082f9f8_Start, 0x004226C0, (C_0082f9f8* self, void* edx, float a2, char a3));
CSF_FUNC(int, __fastcall, CharTask_StartMovement, 0x004DB140, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, TFncGirar_OnRotationComplete, 0x004DB1D0, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_DegToRad, 0x0083C548);

static int __fastcall TAutonomo_StartMovementOrRotate(TAutonomo* self, void* /*edx*/, int a2, int a3)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);

    bool useDefault = false;
    if (CSF_AT(uint8_t, owner, 0x314) != 0) {
        useDefault = true;
    } else {
        void* obj = reinterpret_cast<void*>(self->field_0010);
        if (obj == nullptr) {
            useDefault = true;
        } else {
            int r = ((int(__fastcall*)(void*, void*))CSF_AT(void**, obj, 0)[0x80 / 4])(obj, nullptr);
            if (r != 0)
                useDefault = true;
        }
    }

    if (!useDefault) {
        float angle = self->field_0074 * g_DegToRad;
        int started = CharTask_StartMovement(self, nullptr, (int)(uintptr_t)(base + 0x28));
        uint8_t* p8 = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, started, 0));
        (void)p8;
        uint32_t startedPtr = static_cast<uint32_t>(started);
        uint32_t a = CSF_AT(uint32_t, startedPtr, 8);
        uint32_t b = CSF_AT(uint32_t, a, 0x14);
        uint32_t c = CSF_AT(uint32_t, b, 0x30);
        return C_0082f9f8_Start(reinterpret_cast<C_0082f9f8*>(c), nullptr, angle, 1);
    }

    uint32_t v = self->field_007c;
    uint32_t cfg = CSF_AT(uint32_t, self->field_0008, 0x14);
    if (v == CSF_AT(uint32_t, cfg, 0x1C4)) {
        self->field_0064 = 0;
        TFncGirar_OnRotationComplete(self, nullptr, 0);
        return 0;
    }
    if (v != 0)
        return CharTask_StartMovement(self, nullptr, (int)(uintptr_t)(base + 0x50));
    return CharTask_StartMovement(self, nullptr, (int)(uintptr_t)(base + 0x3C));
}

HOOK(0x004D86C0, TAutonomo_StartMovementOrRotate, "auto");
