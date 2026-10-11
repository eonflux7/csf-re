#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Vehicle_SerializeBase, 0x0041B360,
         (void* self, void* edx, int a2));

static int __fastcall C_0082e428_Serialize(C_0082e428* self, void* /*edx*/, int a2)
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
    Vehicle_SerializeBase(self, nullptr, a2);

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
                     serializer, nullptr, &self->field_008c, a2) == -1;
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
                     serializer, nullptr, &self->field_0090, a2) == -1;
    }

    canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, float*, int))
                 CSF_AT(void**, serializer, 0)[0x38 / 4])(
                     serializer, nullptr, &self->field_0094, a2) == -1;
    }

    canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            ((int(__fastcall*)(void*, void*, float*, int))
                 CSF_AT(void**, serializer, 0)[0x54 / 4])(
                     serializer, nullptr, &CSF_AT(float, self, 0xA0), a2) == -1;
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

HOOK(0x0041DC60, C_0082e428_Serialize, "auto");
