#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, ProfilesSavegames_SerializeBase, 0x004F1E60,
         (void* a1, int arg4));
CSF_FUNC(int, __fastcall, ProfilesSavegames_SerializeMember, 0x00511E40,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ProfilesSavegames_SerializeEmbeddedData, 0x00514390,
         (void* self, void* edx, int a2));

static int __fastcall S_004f21d0_Serialize(
    S_004f21d0* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0)
    {
        void* stream = CSF_AT(void*, context, 4);
        failed = !reinterpret_cast<bool (__fastcall*)(
            void*, void*, int, int)>(
                CSF_AT(void**, stream, 0)[0x80 / 4])(
                    stream, nullptr, 0, contextAddress);
    }

    CSF_AT(uint8_t, context, 0) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failed)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(
                void*, void*, uint32_t*, int)>(
                    CSF_AT(void**, stream, 0)[0x58 / 4])(
                        stream, nullptr, &self->field_0034, contextAddress) == -1;
    }

    bool ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        ProfilesSavegames_SerializeBase(self, contextAddress);
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        ProfilesSavegames_SerializeMember(
            &self->field_001c, nullptr, contextAddress);
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        ProfilesSavegames_SerializeMember(
            &self->field_0038, nullptr, contextAddress);
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        ProfilesSavegames_SerializeEmbeddedData(
            reinterpret_cast<uint8_t*>(self) + 0x6C, nullptr, contextAddress);
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(
                void*, void*, uint32_t*, int)>(
                    CSF_AT(void**, stream, 0)[0x48 / 4])(
                        stream, nullptr, &self->field_00ac, contextAddress) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(
                void*, void*, uint32_t*, int)>(
                    CSF_AT(void**, stream, 0)[0x48 / 4])(
                        stream, nullptr, &self->field_00b0, contextAddress) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(
                void*, void*, uint32_t*, int)>(
                    CSF_AT(void**, stream, 0)[0x58 / 4])(
                        stream, nullptr, &self->field_00c4, contextAddress) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(
                void*, void*, uint32_t*, int)>(
                    CSF_AT(void**, stream, 0)[0x58 / 4])(
                        stream, nullptr, &self->field_00b4, contextAddress) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(
                void*, void*, uint32_t*, int)>(
                    CSF_AT(void**, stream, 0)[0x48 / 4])(
                        stream, nullptr, &self->field_00b8, contextAddress) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* stream = CSF_AT(void*, context, 4);
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CSF_AT(uint8_t, context, 0) =
            reinterpret_cast<int (__fastcall*)(
                void*, void*, uint32_t*, int)>(
                    CSF_AT(void**, stream, 0)[0x58 / 4])(
                        stream, nullptr, &CSF_AT(uint32_t, self, 0xBC),
                        contextAddress) == -1;
    }

    ready = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (ready)
    {
        void* stream = CSF_AT(void*, context, 4);
        const int result = reinterpret_cast<int (__fastcall*)(
            void*, void*, int)>(
                CSF_AT(void**, stream, 0)[0x7C / 4])(
                    stream, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x005144C0, S_004f21d0_Serialize, "auto");
