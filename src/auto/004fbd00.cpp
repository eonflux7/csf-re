#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

static int __fastcall C_0083f364_Serialize(
    C_0083f364* self, void* /*edx*/, bool* archive)
{
    bool failed;
    if (CSF_AT(uint8_t, archive, 0) != 0) {
        failed = false;
    } else {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, bool*))
            CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archive);
    }

    const uint32_t savedContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;

    if (!failed) {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, bool*))
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_0010, archive) == -1;
    }

    bool proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed) {
        void* backend = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0) =
            ((int(__fastcall*)(void*, void*, uint32_t*, bool*))
                CSF_AT(void**, backend, 0)[0x58 / 4])(
                    backend, nullptr, &self->field_0004, archive) == -1;
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed) {
        CSF_AT(uint8_t, archive, 0x15) = 1;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        SceneLoader_SerializeReference(&self->field_0008, archive);
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (proceed) {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(&CSF_AT(uint32_t, self, 0x0C), archive);
    }

    proceed = CSF_AT(uint8_t, archive, 0) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedContext;
    if (proceed) {
        void* backend = CSF_AT(void*, archive, 0x04);
        const bool result = ((bool(__fastcall*)(void*, void*, bool*))
            CSF_AT(void**, backend, 0)[0x7C / 4])(
                backend, nullptr, archive);
        if (result) {
            CSF_AT(uint8_t, archive, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0) = 1;
    return 1;
}

HOOK(0x004FBD00, C_0083f364_Serialize, "auto");
