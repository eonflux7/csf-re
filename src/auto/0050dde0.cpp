#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Forwards a2 (object) to the panel at this->field_0014: if a2 is non-null and
// its slot 8 query reports not-ready, asks the panel (slot 0x88) for a handle
// for a3; when that handle is non-null, attaches it to a2 (slot 0x38), then
// notifies the panel (slot 0x8C) with a3.
static void __fastcall CMenuManager_ConnectObjectToPanel(CMenuManager* self, void* /*edx*/, uint32_t a2, uint32_t a3)
{
    if (a2 != 0)
    {
        void* obj = reinterpret_cast<void*>(a2);
        int isReady = reinterpret_cast<int (__fastcall*)(void*, void*)>(CSF_AT(void**, obj, 0)[8])(obj, nullptr);
        if (!isReady)
        {
            int handle = reinterpret_cast<int (__fastcall*)(void*, void*, uint32_t)>(
                CSF_AT(void**, reinterpret_cast<void*>(self->field_0014), 0)[34])(
                reinterpret_cast<void*>(self->field_0014), nullptr, a3);
            if (handle)
            {
                reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t, int)>(
                    CSF_AT(void**, obj, 0)[14])(obj, nullptr, self->field_0014, handle);
                reinterpret_cast<void (__fastcall*)(void*, void*, uint32_t)>(
                    CSF_AT(void**, reinterpret_cast<void*>(self->field_0014), 0)[35])(
                    reinterpret_cast<void*>(self->field_0014), nullptr, a3);
            }
        }
    }
}

HOOK(0x0050DDE0, CMenuManager_ConnectObjectToPanel, "auto");
