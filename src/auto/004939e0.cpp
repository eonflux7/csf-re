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

CSF_FUNC(int, __fastcall, C_00833908_Serialize_491950, 0x00491950,
         (C_00833908* self, void* edx, int contextAddress));

static int __fastcall C_00833cf8_Serialize(C_00833cf8* self, void* /*edx*/, int a2)
{
    void* context = reinterpret_cast<void*>(a2);
    bool failed = false;

    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        failed = !((bool(__fastcall*)(void*, void*, int, int))
                       CSF_AT(void**, serializer, 0)[0x80 / 4])(
                           serializer, nullptr, 0, a2);
    }

    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failed;
    C_00833908_Serialize_491950(self, nullptr, a2);

    bool canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                 CSF_AT(void**, serializer, 0)[0x58 / 4])(
                     serializer, nullptr, &self->field_0184, a2) == -1;
    }

    canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                 CSF_AT(void**, serializer, 0)[0x58 / 4])(
                     serializer, nullptr, &self->field_0188, a2) == -1;
    }

    canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                 CSF_AT(void**, serializer, 0)[0x58 / 4])(
                     serializer, nullptr, &self->field_018c, a2) == -1;
    }

    canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, int))
                 CSF_AT(void**, serializer, 0)[0x58 / 4])(
                     serializer, nullptr, &CSF_AT(uint32_t, self, 0x190), a2) == -1;
    }

    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        const int result =
            ((int(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, serializer, 0)[0x7C / 4])(
                     serializer, nullptr, a2);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004939E0, C_00833cf8_Serialize, "auto");
