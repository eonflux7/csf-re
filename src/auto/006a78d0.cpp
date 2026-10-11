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

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_791150, 0x00791150, (void* self, void* edx, void* a2, int a3));

// Rebinds this object's resource when a binding id is set: releases the previous resource
// (slot 0 with arg 1), allocates and constructs a new one, binds it through sub_791150,
// and updates the state flags. Without a binding id it only clears the state flag.
static char __fastcall NetGatheringDdl_RebindResource(S_006a7640* self, void* /*edx*/)
{
    if (self->field_0038 != 0)
    {
        C_0085e760* oldResource = reinterpret_cast<C_0085e760*>(self->field_0034);
        if (oldResource)
        {
            ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, oldResource, 0)[0])(oldResource, nullptr, 1);
            self->field_0034 = 0;
        }

        void* rawMemory = NetZ_OperatorNew(112);
        C_0085e760* newResource = nullptr;
        if (rawMemory)
            newResource = reinterpret_cast<C_0085e760*>(ConstructResource(rawMemory, nullptr));

        uint32_t bindingId = self->field_0038;
        self->field_0090 = reinterpret_cast<uint32_t>(newResource);
        char bindResult = sub_791150(reinterpret_cast<void*>(self->field_0028), nullptr, newResource, static_cast<int>(bindingId));

        self->field_02c8 = 1;
        self->field_0020 = 2;
        return bindResult;
    }

    self->field_02c8 = 0;
    self->field_0020 = 2;
    return 0;
}

HOOK(0x006A78D0, NetGatheringDdl_RebindResource, "auto");
