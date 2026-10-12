#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Archive_SerializeMemberCollection, 0x0044EA10, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, CharActionFsm_SerializeStateData, 0x0048B5C0, (void* self, void* edx, void* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall S_00691860_SerializeActionState(
    S_00691860* self, void* /*edx*/, void* archive)
{
    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, backend, 0x00)[0x80 / 4])(
                backend, nullptr, 0, archive);
    }

    CSF_AT(uint8_t, archive, 0x00) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(self, reinterpret_cast<bool*>(archive));
    }

    bool canContinue = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        const int result = ((int(__fastcall*)(void*, void*, uint32_t*, void*))
            CSF_AT(void**, backend, 0x00)[0x58 / 4])(
                backend, nullptr, &self->field_0004, archive);
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }

    canContinue = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeMemberCollection(
            reinterpret_cast<uint8_t*>(self) + 0x08, nullptr, archive);
    }

    canContinue = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CharActionFsm_SerializeStateData(
            reinterpret_cast<uint8_t*>(self) + 0x2C, nullptr, archive);
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        const int result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, backend, 0x00)[0x7C / 4])(
                backend, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x0048A650, S_00691860_SerializeActionState, "auto");
