#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, IsGlobalValueNonZero, 0x00521BF0, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static int __fastcall OptionsFile_ApplySettings(void* self, void* /*edx*/)
{
    if (static_cast<uint8_t>(IsGlobalValueNonZero()))
    {
        if (GetGlobalValue_70DDE0())
        {
            void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
            void* child =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, manager, 0)[0x70 / 4])(manager, nullptr);
            if (child)
            {
                uint32_t value = CSF_AT(uint32_t, self, 0x04);
                ((void(__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, child, 0)[0x1C / 4])(child, nullptr, value);
            }
        }
    }

    if (static_cast<uint8_t>(IsGlobalValueNonZero()))
    {
        if (GetGlobalValue_70DDE0())
        {
            void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
            uint32_t value = CSF_AT(uint32_t, self, 0x08);
            ((void(__fastcall*)(void*, void*, uint32_t))
                CSF_AT(void**, manager, 0)[0x10 / 4])(manager, nullptr, value);

            manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
            void* child =
                ((void*(__fastcall*)(void*, void*))
                    CSF_AT(void**, manager, 0)[0x70 / 4])(manager, nullptr);
            if (child)
            {
                value = CSF_AT(uint32_t, self, 0x08);
                ((void(__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, child, 0)[0x24 / 4])(child, nullptr, value);
            }
        }
    }

    if (static_cast<uint8_t>(IsGlobalValueNonZero()) && GetGlobalValue_70DDE0())
    {
        void* manager = reinterpret_cast<void*>(GetGlobalValue_70DDE0());
        uint32_t value = CSF_AT(uint32_t, self, 0x0C);
        ((void(__fastcall*)(void*, void*, uint32_t))
            CSF_AT(void**, manager, 0)[0x68 / 4])(manager, nullptr, value);
    }

    int result = IsGlobalValueNonZero();
    if (static_cast<uint8_t>(result))
        return GetGlobalValue_70DDE0();
    return result;
}

HOOK(0x0050A980, OptionsFile_ApplySettings, "auto");
