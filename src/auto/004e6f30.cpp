#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_0083eb70_FillActionRequest(C_0083eb70* self, void* /*edx*/, int a2)
{
    uint8_t* req = reinterpret_cast<uint8_t*>(a2);
    int handle = a2 + 20;

    CSF_AT(int, req, 4) = -1;

    void* owner = reinterpret_cast<void*>(self->field_0008);
    if (CSF_AT(int, owner, 0x1D8) == 3)
    {
        if (self->vSlot9C(handle, 6))
        {
            int obj = CHandleTable_Resolve(g_pCurrentScene, nullptr, handle);
            if (obj)
            {
                int kind = CSF_AT(int, reinterpret_cast<void*>(obj), 0x50);
                if (kind == 8 || kind == 16)
                {
                    CSF_AT(int, req, 8) = 6;
                    CSF_AT(int, req, 4) = 2;
                }
            }
        }
    }
    else
    {
        if (self->vSlot9C(handle, 5))
        {
            CSF_AT(int, req, 8) = 5;
            CSF_AT(int, req, 4) = 2;
        }
        else if (self->vSlot9C(handle, 6))
        {
            CSF_AT(int, req, 8) = 6;
            CSF_AT(int, req, 4) = 2;
        }
    }
    return CSF_AT(int, req, 4) != -1;
}

HOOK(0x004E6F30, C_0083eb70_FillActionRequest, "auto");
