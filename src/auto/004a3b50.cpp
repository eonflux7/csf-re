#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_008344f0_ApplyStep, 0x004A38E0, (void* self, void* edx, float a2));
CSF_FUNC(double, __fastcall, C_Unknown_GetValue, 0x006C2050, (void* self, void*));

CSF_GLOBAL(float, g_fZero, 0x82D4CC);

static char __fastcall C_008344f0_UpdateTimer(C_008344f0* self, void* /*edx*/, float deltaTime)
{
    void* owner = reinterpret_cast<void*>(self->field_0008);

    // owner->vtbl[0x28/4]()
    void* target = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    char result = (char)(uintptr_t)target;
    if (!target)
        return result;

    // target->vtbl[0x20/4]() returns pointer to flags byte
    uint8_t* flags = ((uint8_t*(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[0x20 / 4])(target, nullptr);
    result = (char)(uintptr_t)flags;
    if ((*flags & 1) == 0)
        return result;

    uint32_t timerBits;
    std::memcpy(&timerBits, &self->field_0064, sizeof(timerBits));
    if (self->field_0064 > g_fZero && timerBits != 0x7F7FFFFF)
    {
        self->field_0064 = self->field_0064 - deltaTime;

        void* sub = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, target, 0)[0x68 / 4])(target, nullptr);
        if (sub)
        {
            char ok = ((char(__fastcall*)(void*, void*, int))CSF_AT(void**, sub, 0)[0x2C / 4])(sub, nullptr, 0);
            if (ok)
            {
                void* item = ((void*(__fastcall*)(void*, void*, int))CSF_AT(void**, sub, 0)[0x20 / 4])(sub, nullptr, 0);
                if (item)
                {
                    float value = (float)C_Unknown_GetValue(item, nullptr);
                    double other = ((double(__fastcall*)(void*, void*, int))CSF_AT(void**, sub, 0)[0x34 / 4])(sub, nullptr, 0);
                    if (value - other > self->field_0064)
                    {
                        ((void(__fastcall*)(void*, void*, int, int))CSF_AT(void**, sub, 0)[0x30 / 4])(sub, nullptr, 0, 0);
                    }
                }
            }
        }
    }

    result = (char)self->field_0040;
    if (self->field_0040)
    {
        if ((int)self->field_0048 != -1)
        {
            C_008344f0_ApplyStep(self, nullptr, deltaTime);
            result = (char)self->vSlot34();
            if (result)
            {
                self->field_0040 = 0;
                self->field_0064 = 0.0f;
                self->field_0060 = 0.0f;

                uint8_t* ownerNow = reinterpret_cast<uint8_t*>(self->field_0008);
                uint8_t* subObj = ownerNow + 4;
                result = ((char(__fastcall*)(void*, void*))CSF_AT(void**, subObj, 0)[0xC / 4])(subObj, nullptr);
            }
        }
    }
    return result;
}

HOOK(0x004A3B50, C_008344f0_UpdateTimer, "auto");
