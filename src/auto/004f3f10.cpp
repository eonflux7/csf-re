#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, HudStringShow_SerializeBase, 0x004F37F0,
         (void* arg0, int a2));
CSF_FUNC(void**, __fastcall, SceneLoader_RegisterArchivedObject, 0x0069ACB0,
         (void* self, void* edx, int a2));

static int __fastcall C_0083f0ec_Serialize(
    C_0083f0ec* self, void* /*edx*/, int archiveArg)
{
    void* archive = reinterpret_cast<void*>(archiveArg);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, archiveArg);
    }

    const uint32_t savedState = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        HudStringShow_SerializeBase(&self->field_0004, archiveArg);
    }

    bool canContinue = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_0028, archiveArg) == -1;
    }

    canContinue = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x54 / 4])(
                    serializer, nullptr, &self->field_002c, archiveArg) == -1;
    }

    canContinue = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (canContinue)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
        CSF_AT(uint8_t, archive, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0)[0x58 / 4])(
                    serializer, nullptr, &self->field_0024, archiveArg) == -1;
    }

    if (CSF_AT(uint8_t, archive, 0x14) == 0 &&
        CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        SceneLoader_RegisterArchivedObject(
            CSF_AT(void*, archive, 0x10), nullptr,
            reinterpret_cast<int>(self));
    }

    CSF_AT(uint32_t, archive, 0x1C) = savedState;
    if (CSF_AT(uint8_t, archive, 0x00) != 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    void* serializer = CSF_AT(void*, archive, 0x04);
    const int result =
        reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, archiveArg);
    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, archive, 0x00) = 0;
    return result;
}

HOOK(0x004F3F10, C_0083f0ec_Serialize, "auto");
