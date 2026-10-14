#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));

static int __fastcall S_00691860_Serialize(
    S_00691860* self, void* /*edx*/, uint8_t* context)
{
    const uint32_t savedReference = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 4);
        failed = !((bool(__fastcall*)(void*, void*, int, uint8_t*))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, context);
    }

    CSF_AT(uint8_t, context, 0) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SceneLoader_SerializeReference(self, reinterpret_cast<bool*>(context));
    }

    bool canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, uint32_t*, uint8_t*))
            CSF_AT(void**, serializer, 0)[0x58 / 4])(
                serializer, nullptr, &self->field_0004, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, uint8_t*, uint8_t*))
            CSF_AT(void**, serializer, 0)[0x14 / 4])(
                serializer, nullptr,
                reinterpret_cast<uint8_t*>(self) + 0x08, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, uint8_t*, uint8_t*))
            CSF_AT(void**, serializer, 0)[0x38 / 4])(
                serializer, nullptr,
                reinterpret_cast<uint8_t*>(self) + 0x10, context);
        CSF_AT(uint8_t, context, 0) = result == -1;
    }

    canContinue = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedReference;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, context, 4);
        // Original: call [edx+7Ch]; test al, al; je ...; ... ret 4 with eax
        // unchanged, so the callee's full eax is the return value.
        const uint32_t result = ((uint32_t(__fastcall*)(void*, void*, uint8_t*))
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, context);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return static_cast<int>(result);
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x00408AD0, S_00691860_Serialize, "auto");
