#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Stores a pending request (two header dwords, two dwords, a 3-dword vector and three flag bytes)
// and marks it as pending (flag 0x10 at +0x2E4) unless one is already pending.
static void __fastcall Game_SetPendingRequest(void* self, void* /*edx*/, void* request)
{
    if ((CSF_AT(uint8_t, self, 0x2E4) & 0x10) != 0)
        return;

    CSF_AT(uint8_t, self, 0x3DD) = 0;

    CSF_AT(uint32_t, self, 0x31C) = CSF_AT(uint32_t, request, 0x0);
    CSF_AT(uint32_t, self, 0x320) = CSF_AT(uint32_t, request, 0x4);
    CSF_AT(uint32_t, self, 0x328) = CSF_AT(uint32_t, request, 0xC);
    CSF_AT(uint32_t, self, 0x32C) = CSF_AT(uint32_t, request, 0x10);

    // embedded 3-dword vector
    CSF_AT(uint32_t, self, 0x330) = CSF_AT(uint32_t, request, 0x14);
    CSF_AT(uint32_t, self, 0x334) = CSF_AT(uint32_t, request, 0x18);
    CSF_AT(uint32_t, self, 0x338) = CSF_AT(uint32_t, request, 0x1C);

    CSF_AT(uint8_t, self, 0x33C) = CSF_AT(uint8_t, request, 0x20);
    CSF_AT(uint8_t, self, 0x33D) = CSF_AT(uint8_t, request, 0x21);
    CSF_AT(uint8_t, self, 0x33E) = CSF_AT(uint8_t, request, 0x22);

    if ((CSF_AT(uint32_t, self, 0x2E0) & 0x2000) != 0)
    {
        CSF_AT(uint32_t, self, 0x2E4) |= 0x10u;

        int linkedEntity = CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                                (int)((uint8_t*)self + 0x1B4));
        if (linkedEntity)
        {
            void* linkedComponent = CSF_AT(void*, linkedEntity, 0x44);
            if (linkedComponent)
            {
                int slotData = ((int(__fastcall*)(void*, void*, int))
                                    CSF_AT(void**, linkedComponent, 0)[0xC8 / 4])(linkedComponent, nullptr, 0);
                if (slotData)
                    CSF_AT(uint8_t, slotData, 0x16) = 1;
            }
        }
    }
    else
    {
        void* ownComponent = CSF_AT(void*, self, 0x44);
        void* handler = ((void*(__fastcall*)(void*, void*))
                             CSF_AT(void**, ownComponent, 0)[0xD8 / 4])(ownComponent, nullptr);
        if (handler)
        {
            int accepted = ((int(__fastcall*)(void*, void*))
                                CSF_AT(void**, handler, 0)[0x48 / 4])(handler, nullptr);
            if (accepted)
                CSF_AT(uint32_t, self, 0x2E4) |= 0x10u;
        }
    }
}

HOOK(0x00496CD0, Game_SetPendingRequest, "auto");
