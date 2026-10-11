#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

using MenuServiceArray = void*[2];
CSF_GLOBAL(MenuServiceArray, g_MenuServices, 0x008B043C);
CSF_GLOBAL(void*, g_MenuServiceString, 0x008B0444);
CSF_GLOBAL(uint8_t, g_MenuServicesReady, 0x008B0448);

static int __cdecl HudAlertIcons_CleanupMenuServices()
{
    int result = 0;
    void* service = g_MenuServiceString;
    if (service)
    {
        if (CSF_AT(uint32_t, service, 0x18) >= 0x10u)
        {
            void* buffer = CSF_AT(void*, service, 0x04);
            if (buffer)
                Mem_FreeDebug(buffer);
        }
        CSF_AT(uint32_t, service, 0x18) = 15;
        CSF_AT(uint32_t, service, 0x14) = 0;
        CSF_AT(uint8_t, service, 0x04) = 0;
        result = Mem_Free(service);
        g_MenuServiceString = nullptr;
    }

    for (unsigned int i = 0; i < 2; ++i)
    {
        service = g_MenuServices[i];
        if (service)
        {
            if (CSF_AT(uint32_t, service, 0x40) >= 0x10u)
            {
                void* buffer = CSF_AT(void*, service, 0x2C);
                if (buffer)
                    Mem_FreeDebug(buffer);
            }
            CSF_AT(uint32_t, service, 0x40) = 15;
            CSF_AT(uint32_t, service, 0x3C) = 0;
            CSF_AT(uint8_t, service, 0x2C) = 0;

            if (CSF_AT(uint32_t, service, 0x24) >= 0x10u)
            {
                void* buffer = CSF_AT(void*, service, 0x10);
                if (buffer)
                    Mem_FreeDebug(buffer);
            }
            CSF_AT(uint32_t, service, 0x24) = 15;
            CSF_AT(uint32_t, service, 0x20) = 0;
            CSF_AT(uint8_t, service, 0x10) = 0;
            result = Mem_Free(service);
            g_MenuServices[i] = nullptr;
        }
    }

    g_MenuServicesReady = 0;
    return result;
}

HOOK(0x004A09A0, HudAlertIcons_CleanupMenuServices, "auto");
