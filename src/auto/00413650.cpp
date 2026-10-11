#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_0082deb8_Serialize, 0x00410860,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080,
         (int archiveAddress, S_00691860* string));

static bool Archive_BeginField(void* archive)
{
    const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    return ready;
}

static void Archive_ClearFieldFlags(void* archive)
{
    CSF_AT(uint8_t, archive, 0x15) = 0;
    CSF_AT(uint8_t, archive, 0x16) = 0;
}

static void Archive_SerializeFloatField(void* archive, void* field)
{
    if (Archive_BeginField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        Archive_ClearFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int (__fastcall*)(void*, void*, void*, int))
                 CSF_AT(void**, backend, 0)[0x54 / 4])(
                backend, nullptr, field, reinterpret_cast<int>(archive)) == -1;
    }
}

static void Archive_SerializeByteField(void* archive, uint8_t* field)
{
    if (Archive_BeginField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        Archive_ClearFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int (__fastcall*)(void*, void*, uint8_t*, int))
                 CSF_AT(void**, backend, 0)[0x44 / 4])(
                backend, nullptr, field, reinterpret_cast<int>(archive)) == -1;
    }
}

static void Archive_SerializeVectorField(void* archive, void* field)
{
    if (Archive_BeginField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        Archive_ClearFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int (__fastcall*)(void*, void*, void*, int))
                 CSF_AT(void**, backend, 0)[0x38 / 4])(
                backend, nullptr, field, reinterpret_cast<int>(archive)) == -1;
    }
}

static void Archive_SerializeReferenceField(void* archive, uint32_t* field)
{
    if (Archive_BeginField(archive))
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        Archive_ClearFieldFlags(archive);
        CSF_AT(uint8_t, archive, 0x00) =
            ((int (__fastcall*)(void*, void*, uint32_t*, int))
                 CSF_AT(void**, backend, 0)[0x04 / 4])(
                backend, nullptr, field, reinterpret_cast<int>(archive)) == -1;
    }
}

static void Archive_SerializeStringField(void* archive, uint32_t* field)
{
    if (Archive_BeginField(archive))
    {
        Archive_ClearFieldFlags(archive);
        Archive_SerializeString(
            reinterpret_cast<int>(field),
            reinterpret_cast<S_00691860*>(archive));
    }
}

static int __fastcall C_0082dfd0_Serialize(
    C_0082dfd0* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* backend = CSF_AT(void*, archive, 0x04);
        failed =
            !((bool (__fastcall*)(void*, void*, int, int))
                  CSF_AT(void**, backend, 0)[0x80 / 4])(
                backend, nullptr, 0, archiveAddress);
    }

    const uint32_t savedFieldContext = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    C_0082deb8_Serialize(self, nullptr, archiveAddress);

    Archive_SerializeFloatField(archive, &self->field_0084);
    Archive_SerializeByteField(archive, &self->field_0088);
    Archive_SerializeByteField(archive, &self->field_0089);
    Archive_SerializeByteField(archive, &self->field_008a);
    Archive_SerializeByteField(archive, &self->field_008c);
    Archive_SerializeByteField(archive, &self->field_008d);

    Archive_SerializeFloatField(archive, &self->field_00e0);
    Archive_SerializeFloatField(archive, &self->field_00e4);
    Archive_SerializeFloatField(archive, &self->field_00e8);
    Archive_SerializeFloatField(archive, &self->field_00ec);
    Archive_SerializeFloatField(archive, &self->field_00f0);
    Archive_SerializeFloatField(archive, &self->field_00f4);
    Archive_SerializeFloatField(archive, &self->field_00f8);
    Archive_SerializeFloatField(archive, &self->field_00fc);
    Archive_SerializeFloatField(archive, &self->field_0100);
    Archive_SerializeFloatField(archive, &self->field_0104);
    Archive_SerializeFloatField(archive, &self->field_0108);

    Archive_SerializeByteField(archive, &self->field_010c);
    Archive_SerializeByteField(archive, &self->field_010d);
    Archive_SerializeByteField(archive, &self->field_010e);
    Archive_SerializeByteField(archive, &self->field_010f);
    Archive_SerializeFloatField(archive, &self->field_0110);
    Archive_SerializeByteField(archive, &self->field_0114);
    Archive_SerializeByteField(archive, &self->field_0115);

    Archive_SerializeStringField(archive, &self->field_0118);
    Archive_SerializeVectorField(archive, &self->field_0124);
    Archive_SerializeFloatField(archive, &self->field_0130);
    Archive_SerializeReferenceField(archive, &self->field_00a8);
    Archive_SerializeFloatField(archive, &self->field_00d4);
    Archive_SerializeFloatField(archive, &self->field_00d8);
    Archive_SerializeFloatField(archive, &self->field_00dc);
    Archive_SerializeVectorField(archive, &self->field_0090);
    Archive_SerializeVectorField(archive, &self->field_009c);
    Archive_SerializeFloatField(archive, &CSF_AT(uint8_t, self, 0x138));
    Archive_SerializeFloatField(archive, &self->field_0134);
    Archive_SerializeStringField(archive, &self->field_0140);
    Archive_SerializeVectorField(archive, &CSF_AT(uint32_t, self, 0x15C));

    CSF_AT(uint32_t, archive, 0x1C) = savedFieldContext;
    if (CSF_AT(uint8_t, archive, 0x00) != 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    void* backend = CSF_AT(void*, archive, 0x04);
    const int result =
        ((int (__fastcall*)(void*, void*, int))
             CSF_AT(void**, backend, 0)[0x7C / 4])(
            backend, nullptr, archiveAddress);

    if (static_cast<uint8_t>(result) == 0)
    {
        CSF_AT(uint8_t, archive, 0x00) = 1;
        return 1;
    }

    CSF_AT(uint8_t, archive, 0x00) = 0;
    return result;
}

HOOK(0x00413650, C_0082dfd0_Serialize, "auto");
