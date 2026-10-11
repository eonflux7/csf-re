#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, MenuControls_IsPressed, 0x004285B0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, MenuControls_QueryFlag, 0x004285D0, (void* self, void* edx, int value));
CSF_FUNC(double, __fastcall, MenuControls_GetValue, 0x00428610, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, DispatchSelectedCharacterCommand, 0x00450BF0, (void* self, void* , void* command));
CSF_FUNC(int, __fastcall, ResolveEntryLookupValue, 0x00471540, (void* self, void* edx));
CSF_FUNC(void, __fastcall, InvokeInnerVirtualAt0x50, 0x004F50D0, (void* self, void* edx, float amount));
CSF_FUNC(int, __fastcall, CharacterModel_ForwardToChildSlot19, 0x004F50E0, (void* self, void* , int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_flt_0082E25C, 0x0082E25C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static void __fastcall C_00830578_UpdateCharacterMovement(
    C_00830578* self, void* /*edx*/, float elapsedTime)
{
    alignas(8) uint8_t commandBytes[0x1C];
    float& horizontal = CSF_AT(float, commandBytes, 0x10);
    int& unusedAxis = CSF_AT(int, commandBytes, 0x14);
    float& vertical = CSF_AT(float, commandBytes, 0x18);

    horizontal = 0.0f;
    unusedAxis = 0;
    vertical = 0.0f;

    if (MenuControls_IsPressed(reinterpret_cast<void*>(self->field_0014), nullptr, 0) ||
        MenuControls_QueryFlag(reinterpret_cast<void*>(self->field_0014), nullptr, 0))
    {
        vertical = static_cast<float>(
            MenuControls_GetValue(reinterpret_cast<void*>(self->field_0014), nullptr, 0));
    }

    if (MenuControls_IsPressed(reinterpret_cast<void*>(self->field_0014), nullptr, 1) ||
        MenuControls_QueryFlag(reinterpret_cast<void*>(self->field_0014), nullptr, 1))
    {
        vertical = static_cast<float>(vertical -
            MenuControls_GetValue(reinterpret_cast<void*>(self->field_0014), nullptr, 1));
    }

    if (MenuControls_IsPressed(reinterpret_cast<void*>(self->field_0014), nullptr, 2) ||
        MenuControls_QueryFlag(reinterpret_cast<void*>(self->field_0014), nullptr, 2))
    {
        horizontal = static_cast<float>(
            MenuControls_GetValue(reinterpret_cast<void*>(self->field_0014), nullptr, 2));
    }

    if (MenuControls_IsPressed(reinterpret_cast<void*>(self->field_0014), nullptr, 3) ||
        MenuControls_QueryFlag(reinterpret_cast<void*>(self->field_0014), nullptr, 3))
    {
        horizontal = static_cast<float>(horizontal -
            MenuControls_GetValue(reinterpret_cast<void*>(self->field_0014), nullptr, 3));
    }

    if (horizontal != g_fZero || vertical != g_fZero)
    {
        int globalValue = GetGlobalValue_4FBDE0();
        int firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
        void* provider = GetCachedObjectProvider(
            reinterpret_cast<void*>(firstElement), nullptr);

        if (ResolveEntryLookupValue(provider, nullptr) == 4)
        {
            globalValue = GetGlobalValue_4FBDE0();
            firstElement = GetFirstElement(reinterpret_cast<void*>(globalValue), nullptr);
            provider = GetCachedObjectProvider(reinterpret_cast<void*>(firstElement), nullptr);

            float verticalAmount = vertical * elapsedTime * g_flt_0082E25C;
            InvokeInnerVirtualAt0x50(provider, nullptr, verticalAmount);

            float horizontalAmount = horizontal * elapsedTime * g_flt_0082E25C;
            int horizontalBits;
            std::memcpy(&horizontalBits, &horizontalAmount, sizeof(horizontalBits));
            CharacterModel_ForwardToChildSlot19(provider, nullptr, horizontalBits);
        }
        else
        {
            CSF_AT(float, commandBytes, 0x04) = horizontal;
            CSF_AT(int, commandBytes, 0x08) = unusedAxis;
            CSF_AT(int, commandBytes, 0x00) = 0;
            CSF_AT(float, commandBytes, 0x0C) = vertical;
            DispatchSelectedCharacterCommand(g_pCurrentScene, nullptr, commandBytes);
        }
    }
}

HOOK(0x0042C3B0, C_00830578_UpdateCharacterMovement, "auto");
