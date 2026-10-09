#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void, __fastcall, SetProviderHeadingCacheEnabled, 0x004F5100,
         (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, InvokeMemberVirtualSlot8, 0x004F5170,
         (void* self, void* edx, float angleRadians));
CSF_FUNC(int, __fastcall, InvokeInnerObjectVirtual28, 0x004F5190,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10,
         (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_753A30, 0x00753A30, ());

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);
CSF_GLOBAL(float, g_DegreesToRadians, 0x00833438);

static int __fastcall CCharActionFsm_UpdateHeading(
    CCharActionFsm* self, void* /*edx*/,
    float angleRadians, float turnRate, char cacheHeading)
{
    void* settings = reinterpret_cast<void*>(GetGlobalValue_753A30());
    float headingScale =
        CSF_AT(float, settings, 0x13C) * g_DegreesToRadians;

    void* configuration = reinterpret_cast<void*>(GetValue8B0554());
    const int headingOverride = CSF_AT(int, configuration, 0x90C);
    auto* headingSettings =
        reinterpret_cast<std::uint8_t*>(configuration) + 84;
    if (headingOverride != -1082130432)
    {
        headingScale =
            CSF_AT(float, headingSettings, 2232) * g_DegreesToRadians;
    }

    void* manager = reinterpret_cast<void*>(GetGlobalValue_4FBDE0());
    void* firstElement =
        reinterpret_cast<void*>(GetFirstElement(manager, nullptr));
    void* provider = GetCachedObjectProvider(firstElement, nullptr);

    SetProviderHeadingCacheEnabled(provider, nullptr, cacheHeading);

    const std::uint8_t headingLocked = CSF_AT(
        std::uint8_t, reinterpret_cast<void*>(self->field_0008), 0x228);
    int result = headingLocked;
    if (!headingLocked)
    {
        if (turnRate > g_fZero)
        {
            // The tail forwarder passes both arguments to virtual slot 0x28.
            using ForwardHeading = int (__fastcall*)(
                void*, void*, float, float);
            result = reinterpret_cast<ForwardHeading>(
                InvokeInnerObjectVirtual28)(
                    provider, nullptr, angleRadians, turnRate);
        }
        else
        {
            result =
                InvokeMemberVirtualSlot8(provider, nullptr, angleRadians);
        }
    }

    CSF_AT(float, reinterpret_cast<void*>(self->field_0008), 0x390) =
        angleRadians / headingScale;
    return result;
}

HOOK(0x00482DC0, CCharActionFsm_UpdateHeading, "auto");
