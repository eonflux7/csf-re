#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall S_00691860_Serialize(
    S_00691860* self, void* /*edx*/, void* archive)
{
    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, archive);
    }

    uint32_t savedState = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;

    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(self, reinterpret_cast<bool*>(archive));
    }

    bool canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x58 / 4])(
                serializer, nullptr, &self->field_0004, archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x58 / 4])(
                serializer, nullptr, &CSF_AT(uint32_t, self, 0x08), archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    canContinue = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        int result = ((int(__fastcall*)(void*, void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x58 / 4])(
                serializer, nullptr, &CSF_AT(uint32_t, self, 0x0C), archive);
        CSF_AT(uint8_t, archive, 0) = result == -1;
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedState;
    if (CSF_AT(uint8_t, archive, 0) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        int result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x006E76C0, S_00691860_Serialize, "auto");
