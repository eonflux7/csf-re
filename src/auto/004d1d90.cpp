#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetRelatedObjectAndClearFlag, 0x0048E6F0, (void* self, void*));
CSF_FUNC(void, __fastcall, C_008343a4_UpdateAlertEntryForReference, 0x004A08F0, (C_008343a4* self, void* , int a2, int a3, int a4));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall S_004d1d90_InitializeAlertState(
    S_004d1d90* self, void* /*edx*/)
{
    if (self->field_0062)
        return;

    GetRelatedObjectAndClearFlag(
        reinterpret_cast<void*>(self->field_0014), nullptr);

    void* related = CSF_AT(
        void*, reinterpret_cast<void*>(self->field_0014), 0x44);
    if (related)
    {
        void* object =
            (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, related, 0)[0xD8 / 4]))(related, nullptr);

        if (object)
        {
            if ((reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                    CSF_AT(void**, object, 0)[0xA8 / 4]))(object, nullptr))
            {
                void* child =
                    (reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                        CSF_AT(void**, object, 0)[0xA8 / 4]))(object, nullptr);

                (reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, child, 0)[0x3C / 4]))(child, nullptr, 0);
            }
        }
    }

    auto* reference =
        reinterpret_cast<std::uint8_t*>(self->field_0014) + 0x1C;

    alignas(8) std::uint8_t colorBytes[0x10];
    auto* color = reinterpret_cast<float*>(colorBytes);
    color[0] = 255.0f;
    color[1] = 255.0f;
    color[2] = 255.0f;
    color[3] = 255.0f;

    C_008343a4_UpdateAlertEntryForReference(
        CSF_AT(C_008343a4*, g_pCurrentScene, 0xAC),
        nullptr,
        reinterpret_cast<int>(reference),
        0,
        reinterpret_cast<int>(color));

    self->field_0063 = 0;
    self->field_0062 = 1;
}

HOOK(0x004D1D90, S_004d1d90_InitializeAlertState, "auto");
