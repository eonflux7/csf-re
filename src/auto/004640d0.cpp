#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Archive context used by the serialisation routines:
//   +0x00 uint8_t  error flag
//   +0x04 stream object (virtual interface)
//   +0x15 uint8_t  per-field flag A
//   +0x16 uint8_t  per-field flag B
//   +0x1C uint32_t nesting / current-field value (saved and restored around the object)

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));

typedef int (__fastcall* StreamFieldFn)(void* stream, void* edx, void* field, void* archive);

// Resets the per-field state; returns true when the archive is still healthy.
static inline bool Archive_BeginField(void* archive, uint8_t flagA)
{
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (CSF_AT(uint8_t, archive, 0x00) != 0)
        return false;
    CSF_AT(uint8_t, archive, 0x15) = flagA;
    CSF_AT(uint8_t, archive, 0x16) = 0;
    return true;
}

// Stream virtual at vtbl + 0x44
static inline void Archive_Stream44(void* archive, void* field)
{
    if (Archive_BeginField(archive, 0))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        int rc = (*reinterpret_cast<StreamFieldFn**>(stream))[0x44 / 4](stream, nullptr, field, archive);
        CSF_AT(uint8_t, archive, 0x00) = (rc == -1);
    }
}

// Stream virtual at vtbl + 0x48
static inline void Archive_Stream48(void* archive, void* field)
{
    if (Archive_BeginField(archive, 0))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        int rc = (*reinterpret_cast<StreamFieldFn**>(stream))[0x48 / 4](stream, nullptr, field, archive);
        CSF_AT(uint8_t, archive, 0x00) = (rc == -1);
    }
}

// Stream virtual at vtbl + 0x54
static inline void Archive_Stream54(void* archive, void* field)
{
    if (Archive_BeginField(archive, 0))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        int rc = (*reinterpret_cast<StreamFieldFn**>(stream))[0x54 / 4](stream, nullptr, field, archive);
        CSF_AT(uint8_t, archive, 0x00) = (rc == -1);
    }
}

// Stream virtual at vtbl + 0x58
static inline void Archive_Stream58(void* archive, void* field)
{
    if (Archive_BeginField(archive, 0))
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        int rc = (*reinterpret_cast<StreamFieldFn**>(stream))[0x58 / 4](stream, nullptr, field, archive);
        CSF_AT(uint8_t, archive, 0x00) = (rc == -1);
    }
}

static inline void Archive_StringField(void* archive, void* field)
{
    if (Archive_BeginField(archive, 0))
        Archive_SerializeString((int)field, archive);
}

static inline void Archive_ObjectRefField(void* archive, void* field, uint8_t flagA)
{
    if (Archive_BeginField(archive, flagA))
        Archive_SerializeObjectRef(field, (bool*)archive);
}

static int __fastcall C_00831414_Serialize(C_00831414* self, void* /*edx*/, void* archive)
{
    // Open the object block.
    bool beginFailed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        uint8_t opened = (*reinterpret_cast<uint8_t(__fastcall***)(void*, void*, int, void*)>(stream))[0x80 / 4](
            stream, nullptr, 0, archive);
        beginFailed = (opened == 0);
    }
    uint32_t savedFieldState = CSF_AT(uint32_t, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = beginFailed;

    Archive_StringField(archive, &self->field_0004);
    Archive_Stream44(archive, &self->field_0080);
    Archive_Stream54(archive, &self->field_0014);
    Archive_Stream58(archive, &self->field_0018);
    Archive_Stream48(archive, &self->field_001c);
    Archive_ObjectRefField(archive, &self->field_0020, 1);
    Archive_StringField(archive, &self->field_0024);
    Archive_Stream54(archive, &self->field_0058);
    Archive_StringField(archive, &self->field_005c);
    Archive_Stream54(archive, &self->field_0068);
    Archive_Stream44(archive, &self->field_0081);
    Archive_Stream44(archive, &self->field_0082);
    Archive_Stream44(archive, &self->field_0083);
    Archive_Stream48(archive, &self->field_006c);
    Archive_ObjectRefField(archive, &self->field_0070, 0);
    Archive_Stream48(archive, &self->field_0074);
    Archive_Stream54(archive, &self->field_0078);
    Archive_ObjectRefField(archive, &self->field_007c, 0);

    // Close the object block.
    bool healthy = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(uint32_t, archive, 0x1C) = savedFieldState;
    if (healthy)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        int closed = (*reinterpret_cast<int(__fastcall***)(void*, void*, void*)>(stream))[0x7C / 4](
            stream, nullptr, archive);
        if ((uint8_t)closed)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return closed;
        }
    }
    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x004640D0, C_00831414_Serialize, "auto");
