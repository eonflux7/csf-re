#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static int __fastcall C_00860e68_UpdateProviderLightTypes(
    C_00860e68* self, void* /*edx*/)
{
    CSF_AT(uint8_t, GetCachedObjectProvider(
        reinterpret_cast<void*>(self->field_002c), nullptr), 0x02) = 3;
    GetCachedObjectProvider(reinterpret_cast<void*>(self->field_002c), nullptr);

    CSF_AT(uint8_t, GetCachedObjectProvider(
        reinterpret_cast<void*>(self->field_0030), nullptr), 0x02) = 1;
    GetCachedObjectProvider(reinterpret_cast<void*>(self->field_0030), nullptr);

    for (uint32_t index = 0; index < self->field_003c; ++index)
    {
        C_0085e7b8* light =
            reinterpret_cast<C_0085e7b8**>(self->field_0034)[index];
        uint8_t providerType = 0;

        switch (((int(__fastcall*)(C_0085e7b8*, void*))
            CSF_AT(void**, light, 0)[0x1C / 4])(light, nullptr))
        {
        case 0:
        case 3:
            providerType = 3;
            break;
        case 1:
        case 2:
            providerType = 1;
            break;
        default:
            break;
        }

        CSF_AT(uint8_t, GetCachedObjectProvider(light, nullptr), 0x02) =
            providerType;
        GetCachedObjectProvider(light, nullptr);
    }

    uint32_t result = self->field_004c;
    uint32_t index = 0;
    if (result != 0)
    {
        do
        {
            C_0085e7b8* light =
                reinterpret_cast<C_0085e7b8**>(self->field_0044)[index];
            uint8_t providerType = 0;

            switch (((int(__fastcall*)(C_0085e7b8*, void*))
                CSF_AT(void**, light, 0)[0x1C / 4])(light, nullptr))
            {
            case 0:
            case 3:
                providerType = 3;
                break;
            case 1:
            case 2:
                providerType = 1;
                break;
            default:
                break;
            }

            CSF_AT(uint8_t, GetCachedObjectProvider(light, nullptr), 0x02) =
                providerType;
            GetCachedObjectProvider(light, nullptr);
            result = ++index;
        }
        while (index < self->field_004c);
    }

    return static_cast<int>(result);
}

HOOK(0x006D14B0, C_00860e68_UpdateProviderLightTypes, "auto");
