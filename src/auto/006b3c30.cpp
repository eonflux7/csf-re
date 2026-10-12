#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(uint32_t*, __cdecl, GfxObject_GetAttachmentController, 0x006E6AA0, (int a1));

static int __fastcall C_00860288_ApplyConfiguration(
    C_00860288* self, void* /*edx*/, int configurationAddress)
{
    void* configuration = reinterpret_cast<void*>(configurationAddress);
    const int controllerId = CSF_AT(int, configuration, 0x68);

    if (controllerId != -1)
    {
        uint32_t* controller = GfxObject_GetAttachmentController(controllerId);
        self->field_002c = reinterpret_cast<uint32_t>(controller);
        ((void(__fastcall*)(void*, void*, C_00860288*, void*))
            CSF_AT(void**, controller, 0)[0x1C / 4])(
                controller, nullptr, self,
                static_cast<uint8_t*>(configuration) + 0x6C);
    }

    int result = CSF_AT(int, configuration, 0x04);
    if (result)
    {
        void* globalObject = reinterpret_cast<void*>(GetGlobalInt());
        void* renderHandle = reinterpret_cast<void*>(
            RenderHandle_FromObject(globalObject, nullptr));

        result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, renderHandle, 0)[0x2C / 4])(
                renderHandle, nullptr,
                static_cast<uint8_t*>(configuration) + 0x04);
        self->field_006c = static_cast<uint32_t>(result);

        if (result)
        {
            alignas(8) uint8_t bindingBytes[0x8] = {};
            const uint8_t firstOption = CSF_AT(uint8_t, configuration, 0x64);
            const uint8_t secondOption = CSF_AT(uint8_t, configuration, 0x65);
            CSF_AT(uint8_t, bindingBytes, 0x04) = firstOption;
            CSF_AT(C_00860288*, bindingBytes, 0x00) = self;
            CSF_AT(uint8_t, bindingBytes, 0x05) = secondOption;

            void* binding = reinterpret_cast<void*>(result);
            ((void(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, binding, 0)[0x14 / 4])(
                    binding, nullptr, bindingBytes);

            const bool enabled = CSF_AT(uint8_t, configuration, 0x66) != 0;
            uint32_t flags = self->field_0064;
            if (enabled)
                flags |= 0x200;
            else
                flags &= 0xFFFFFDFF;

            self->field_0064 = flags;
            result = static_cast<int>(flags);
        }
    }
    return result;
}

HOOK(0x006B3C30, C_00860288_ApplyConfiguration, "auto");
