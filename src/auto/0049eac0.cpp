#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CScene_RemoveObjectSlot, 0x00453390, (CScene* self, void* , int a2));
CSF_FUNC(void, __fastcall, C_00834198_UpdateBase, 0x00493820, (void* self, void* edx, float a2));
CSF_FUNC(uint8_t, __fastcall, C_00834198_UpdateStandardBehavior, 0x0049E5E0, (C_00834198* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, C_00834198_UpdateSpecialBehavior, 0x0049E840, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static uint8_t __fastcall C_00834198_UpdateActiveBehavior(
    C_00834198* self, void* /*edx*/, int updateArgument)
{
    float elapsedTime;
    std::memcpy(&elapsedTime, &updateArgument, sizeof(elapsedTime));
    C_00834198_UpdateBase(self, nullptr, elapsedTime);

    if (!self->field_01b8)
    {
        if (CSF_AT(void*, self, 0x18))
            self->field_01b8 = 1;

        if (!self->field_01b8)
            return 1;
    }

    void* behavior = CSF_AT(void*, self, 0x18);
    if (!behavior)
    {
        return static_cast<uint8_t>(CScene_RemoveObjectSlot(
            static_cast<CScene*>(g_pCurrentScene), nullptr,
            static_cast<int>(reinterpret_cast<std::intptr_t>(
                reinterpret_cast<uint8_t*>(self) + 0x1C))));
    }

    if (!self->field_0225)
    {
        self->field_0225 = 1;
        if (!((bool(__fastcall*)(void*, void*))
                  CSF_AT(void**, behavior, 0)[0x08 / 4])(behavior, nullptr))
        {
            void* relatedObject = reinterpret_cast<void*>(self->field_004c);
            ((void(__fastcall*)(void*, void*))
                CSF_AT(void**, relatedObject, 0)[0x5C / 4])(
                relatedObject, nullptr);
        }
    }

    behavior = CSF_AT(void*, self, 0x18);
    ((void(__fastcall*)(void*, void*, int))
        CSF_AT(void**, behavior, 0)[0x0C / 4])(
        behavior, nullptr, updateArgument);

    if (self->field_01ba)
    {
        uint32_t parameter = self->field_01bc;
        auto applyPendingData =
            (void(__fastcall*)(C_00834198*, void*, uint32_t*, uint32_t))
                CSF_AT(void**, self, 0)[0xA4 / 4];
        self->field_01ba = 0;
        applyPendingData(self, nullptr, &self->field_01c0, parameter);
    }

    int behaviorState = CSF_AT(int, CSF_AT(void*, self, 0x18), 0x0C);
    if (behaviorState >= 5)
    {
        if (behaviorState <= 11)
            return C_00834198_UpdateStandardBehavior(self, nullptr, updateArgument);

        if (behaviorState == 12)
            return static_cast<uint8_t>(
                C_00834198_UpdateSpecialBehavior(self, nullptr, updateArgument));
    }

    return static_cast<uint8_t>(behaviorState);
}

HOOK(0x0049EAC0, C_00834198_UpdateActiveBehavior, "auto");
