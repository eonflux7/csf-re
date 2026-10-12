#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static unsigned int __fastcall C_00860e68_ClearCachedObjectFlags(C_00860e68* self, void* /*edx*/)
{
    C_0085e7b8* v2 = reinterpret_cast<C_0085e7b8*>(self->field_002c);
    CSF_AT(uint8_t, GetCachedObjectProvider(v2, nullptr), 2) = 0;
    GetCachedObjectProvider(v2, nullptr);

    C_0085e7b8* v3 = reinterpret_cast<C_0085e7b8*>(self->field_0030);
    CSF_AT(uint8_t, GetCachedObjectProvider(v3, nullptr), 2) = 0;
    GetCachedObjectProvider(v3, nullptr);

    for (uint32_t i = 0; i < self->field_003c; ++i)
    {
        C_0085e7b8* v5 = reinterpret_cast<C_0085e7b8**>(self->field_0034)[i];
        CSF_AT(uint8_t, GetCachedObjectProvider(v5, nullptr), 2) = 0;
        GetCachedObjectProvider(v5, nullptr);
    }

    unsigned int result = self->field_004c;
    for (uint32_t j = 0; j < result; ++j)
    {
        C_0085e7b8* v8 = reinterpret_cast<C_0085e7b8**>(self->field_0044)[j];
        CSF_AT(uint8_t, GetCachedObjectProvider(v8, nullptr), 2) = 0;
        GetCachedObjectProvider(v8, nullptr);
        result = self->field_004c;
    }
    return result;
}

HOOK(0x006D15C0, C_00860e68_ClearCachedObjectFlags, "auto");
