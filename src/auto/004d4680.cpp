#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_DefaultActionMagnitude, 0x0082D81C);

CSF_FUNC(void, __fastcall, StoreAbsoluteFloatAt0x18, 0x00409430, (void* self, void* , float value));
CSF_FUNC(unsigned int, __fastcall, C_0083bc28_PrepareFirstAction, 0x004D3830, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0083bc28_PrepareSecondAction, 0x004D3870, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0083bc28_UpdateActionParameters, 0x004D3B30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083bc28_PrepareThirdAction, 0x004D3BF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083bc28_PrepareFourthAction, 0x004D3D30, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0083bc28_ProcessActionTwo, 0x004D3FE0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(int, __fastcall, BddLoader_ReleasePendingHandle, 0x0050E250, (void* self, void*));

static void __fastcall C_0083bc28_ApplyActionCommand(
    C_0083bc28* self, void* /*edx*/, int commandAddress)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* skin = CSF_AT(void*, owner, 0x4C);
    void* target =
        reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, skin, 0)[0x3C / 4])(skin, nullptr);

    void* command = reinterpret_cast<void*>(commandAddress);
    const uint32_t flags = self->field_0044;
    if (CSF_AT(uint8_t, command, 0x0C))
    {
        self->field_0044 = flags | 0x20;
        self->field_0058 = CSF_AT(uint32_t, command, 0x10);
        CSF_AT(uint32_t, self, 0x5C) = CSF_AT(uint32_t, command, 0x14);
        CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, command, 0x18);
    }
    else
    {
        self->field_0044 = flags & 0xFFFFFFDF;
    }

    self->field_0068 = CSF_AT(uint32_t, command, 0x04);
    self->field_006c = CSF_AT(uint32_t, command, 0x08);

    // The original unordered comparison also selects the default for NaN.
    if (!(CSF_AT(float, command, 0x1C) >= g_fZero))
        StoreAbsoluteFloatAt0x18(target, nullptr, g_DefaultActionMagnitude);
    else
        StoreAbsoluteFloatAt0x18(target, nullptr, CSF_AT(float, command, 0x1C));

    C_0083bc28_UpdateActionParameters(self, nullptr);

    switch (CSF_AT(uint32_t, command, 0x20))
    {
    case 2:
        C_0083bc28_ProcessActionTwo(self, nullptr);
        break;

    case 3:
        if ((self->field_0044 & 0x14) == 0)
        {
            if (self->field_004c == 5)
            {
                int globalState = GetGlobalState_50D190();
                BddLoader_ReleasePendingHandle(
                    reinterpret_cast<void*>(globalState), nullptr);
            }
            C_0083bc28_PrepareFirstAction(self, nullptr);
            self->field_004c = 1;
        }
        break;

    case 4:
        if ((self->field_0044 & 0x10) == 0)
        {
            if (self->field_004c == 5)
            {
                int globalState = GetGlobalState_50D190();
                BddLoader_ReleasePendingHandle(
                    reinterpret_cast<void*>(globalState), nullptr);
            }
            C_0083bc28_PrepareSecondAction(self, nullptr);
            self->field_004c = 2;
        }
        break;

    case 5:
    {
        const uint32_t currentFlags = self->field_0044;
        if ((currentFlags & 0x12) == 0)
            self->field_0044 = currentFlags | 4;
        break;
    }

    case 6:
    {
        const uint32_t currentFlags = self->field_0044;
        if ((currentFlags & 0x10) == 0 &&
            (currentFlags & 4) != 0 &&
            (currentFlags & 2) == 0)
        {
            self->field_0044 = currentFlags & 0xFFFFFFFB;
        }
        break;
    }

    case 7:
        if ((self->field_0044 & 0x14) == 0)
        {
            if (self->field_004c == 5)
            {
                int globalState = GetGlobalState_50D190();
                BddLoader_ReleasePendingHandle(
                    reinterpret_cast<void*>(globalState), nullptr);
            }
            C_0083bc28_PrepareThirdAction(self, nullptr);
            self->field_004c = 3;
        }
        break;

    case 8:
        if ((self->field_0044 & 0x10) == 0)
        {
            if (self->field_004c == 5)
            {
                int globalState = GetGlobalState_50D190();
                BddLoader_ReleasePendingHandle(
                    reinterpret_cast<void*>(globalState), nullptr);
            }
            C_0083bc28_PrepareFourthAction(self, nullptr);
            self->field_004c = 4;
        }
        break;

    case 9:
        self->field_0044 |= 0x10;
        break;

    default:
        return;
    }
}

HOOK(0x004D4680, C_0083bc28_ApplyActionCommand, "auto");
