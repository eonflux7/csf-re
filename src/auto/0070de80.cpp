#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, CoreMemory_AllocUntagged, 0x006910F0, (std::size_t size));
CSF_FUNC(char, __cdecl, CoreVariantValues_InitializeFileSystems, 0x006971B0, (int context));
CSF_FUNC(int, __cdecl, ParticleSystemDefs_ConvertAndShowMessage, 0x00701EC0, (int a1, int a2));
CSF_FUNC(uint8_t*, __fastcall, AudioEffect_ConstructDefault, 0x0070EC70, (C_00863ac0* self, void*));

CSF_GLOBAL(int32_t, g_Value_8bbf2c, 0x008BBF2C);

static char __cdecl Audio_InitializeSoundManager(char useAudioEffect, int request)
{
    if (useAudioEffect)
    {
        auto* storage = reinterpret_cast<C_00863ac0*>(
            CoreMemory_AllocUntagged(60));
        uint8_t* effect = storage
            ? AudioEffect_ConstructDefault(storage, nullptr)
            : nullptr;

        void** vtable = CSF_AT(void**, effect, 0);
        g_Value_8bbf2c = reinterpret_cast<int32_t>(effect);

        char result = static_cast<char>(
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                vtable[0xA4 / 4])(effect, nullptr, request));

        if (!result)
        {
            auto* failedEffect = reinterpret_cast<uint8_t*>(g_Value_8bbf2c);
            if (failedEffect)
            {
                reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, failedEffect, 0)[0])(
                        failedEffect, nullptr, 1);
                g_Value_8bbf2c = 0;
            }

            ParticleSystemDefs_ConvertAndShowMessage(
                reinterpret_cast<int>(CSF_STR(0x008612F8, "g025")),
                reinterpret_cast<int>(CSF_STR(0x0086394C, "g208")));
        }

        return result;
    }

    auto* manager = reinterpret_cast<uint8_t*>(
        CoreMemory_AllocUntagged(4));
    if (manager)
    {
        CSF_AT(void**, manager, 0) = CSF_VTBL(0x008638A8);
        g_Value_8bbf2c = reinterpret_cast<int32_t>(manager);
    }
    else
    {
        g_Value_8bbf2c = 0;
    }

    return CoreVariantValues_InitializeFileSystems(0);
}

HOOK(0x0070DE80, Audio_InitializeSoundManager, "auto");
