#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(uint32_t*, __fastcall, Cheats_ResetMenuController, 0x00428800, (void* self, void* edx));
CSF_FUNC(int, __fastcall, Cheats_UpdateMenuController, 0x00428850, (void* self, void* edx, float a2));
CSF_FUNC(void, __fastcall, Cheats_UpdateMenuInput, 0x0042CAE0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, Cheats_UpdateBlockedMenu, 0x0042CB60, (void* self, void* edx, float a2));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));

static void __fastcall C_00830578_Update(C_00830578* self, void* /*edx*/, int updateArgument)
{
    float elapsedTime;
    std::memcpy(&elapsedTime, &updateArgument, sizeof(elapsedTime));

    void* sceneState = CSF_AT(void*, g_pCurrentScene, 0xC0);
    if (CSF_AT(uint8_t, sceneState, 0x09))
    {
        self->field_003f = 0;
        Cheats_UpdateBlockedMenu(self, nullptr, elapsedTime);
        self->field_002c = 0.0f;
        self->field_0030 = 0.0f;
        return;
    }

    const uint8_t previouslyFocused = self->field_003f;
    void* reference = reinterpret_cast<uint8_t*>(self) + 0x04;
    bool focused = false;

    if (!CSF_AT(uint32_t, GetReferencedObject(reference, nullptr), 0x18) ||
        reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, reference, 0)[0x48 / 4])(reference, nullptr))
    {
        void* parent = reinterpret_cast<void*>(self->field_0018);
        if (!parent ||
            !reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, parent, 0)[0x80 / 4])(parent, nullptr))
        {
            focused = true;
        }
        else
        {
            parent = reinterpret_cast<void*>(self->field_0018);
            focused = reinterpret_cast<void* (__fastcall*)(void*, void*)>(
                CSF_AT(void**, parent, 0)[0x80 / 4])(parent, nullptr) == self;
        }
    }

    self->field_003f = focused;
    if (focused && !previouslyFocused)
    {
        void* controller = reinterpret_cast<void*>(self->field_0014);
        reinterpret_cast<void (__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x48 / 4])(controller, nullptr);
    }

    if (CSF_AT(uint32_t, GetReferencedObject(reference, nullptr), 0x18) &&
        !reinterpret_cast<bool (__fastcall*)(void*, void*)>(
            CSF_AT(void**, reference, 0)[0x48 / 4])(reference, nullptr))
    {
        self->field_002c = 0.0f;
        self->field_0030 = 0.0f;
        return;
    }

    if (!self->field_0020)
    {
        Cheats_ResetMenuController(reinterpret_cast<void*>(self->field_0014), nullptr);
    }
    else if (self->field_003c)
    {
        void* controller = reinterpret_cast<void*>(self->field_0014);
        self->field_003c = 0;
        Cheats_UpdateMenuController(controller, nullptr, elapsedTime);
    }
    else
    {
        Cheats_UpdateMenuInput(self, nullptr, updateArgument);
        Cheats_UpdateMenuController(reinterpret_cast<void*>(self->field_0014), nullptr, elapsedTime);
    }

    self->field_002c = 0.0f;
    self->field_0030 = 0.0f;
}

HOOK(0x0042CE40, C_00830578_Update, "auto");
