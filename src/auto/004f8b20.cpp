#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(unsigned int, __stdcall, CharacterModel_SerializeSubmodelData, 0x004F8920, (void* a1, int a2));

// Each operation clears the per-field state even when an earlier operation failed.
#define SERIALIZE_FIELD(byteSlot, fieldAddress)                                      \
    do {                                                                            \
        const bool canSerialize = CSF_AT(uint8_t, context, 0) == 0;                  \
        CSF_AT(uint32_t, context, 0x1C) = 0;                                          \
        if (canSerialize) {                                                         \
            void* archive = CSF_AT(void*, context, 0x04);                            \
            CSF_AT(uint8_t, context, 0x15) = 0;                                      \
            CSF_AT(uint8_t, context, 0x16) = 0;                                      \
            const int status =                                                      \
                ((int(__fastcall*)(void*, void*, void*, int))                        \
                     CSF_AT(void**, archive, 0)[(byteSlot) / 4])                     \
                    (archive, nullptr, (fieldAddress), archiveContext);             \
            CSF_AT(uint8_t, context, 0) = status == -1;                              \
        }                                                                           \
    } while (false)

#define SERIALIZE_REFERENCE(fieldAddress)                                           \
    do {                                                                            \
        const bool canSerialize = CSF_AT(uint8_t, context, 0) == 0;                  \
        CSF_AT(uint32_t, context, 0x1C) = 0;                                          \
        if (canSerialize) {                                                         \
            CSF_AT(uint8_t, context, 0x15) = 1;                                      \
            CSF_AT(uint8_t, context, 0x16) = 0;                                      \
            SceneLoader_SerializeReference(                                         \
                (fieldAddress), reinterpret_cast<bool*>(context));                  \
        }                                                                           \
    } while (false)

#define SERIALIZE_STRING(fieldAddress)                                              \
    do {                                                                            \
        const bool canSerialize = CSF_AT(uint8_t, context, 0) == 0;                  \
        CSF_AT(uint32_t, context, 0x1C) = 0;                                          \
        if (canSerialize) {                                                         \
            CSF_AT(uint8_t, context, 0x15) = 0;                                      \
            CSF_AT(uint8_t, context, 0x16) = 0;                                      \
            Archive_SerializeString(                                                \
                reinterpret_cast<int>(fieldAddress),                               \
                reinterpret_cast<S_00691860*>(context));                            \
        }                                                                           \
    } while (false)

static int __fastcall C_0083f1a0_Serialize(
    C_0083f1a0* self, void* /*edx*/, int archiveContext)
{
    void* context = reinterpret_cast<void*>(archiveContext);

    bool failedToBegin = false;
    if (CSF_AT(uint8_t, context, 0) == 0) {
        void* archive = CSF_AT(void*, context, 0x04);
        failedToBegin =
            !((bool(__fastcall*)(void*, void*, int, int))
                  CSF_AT(void**, archive, 0)[128 / 4])
                 (archive, nullptr, 0, archiveContext);
    }

    const uint32_t savedFieldState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failedToBegin;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (!failedToBegin) {
        CSF_AT(uint8_t, context, 0x15) = 1;
        CSF_AT(uint8_t, context, 0x16) = 0;
        SceneLoader_SerializeReference(
            &self->field_0004, reinterpret_cast<bool*>(context));
    }

    SERIALIZE_FIELD(88, &self->field_0008);
    SERIALIZE_FIELD(88, &self->field_000c);
    SERIALIZE_FIELD(72, &self->field_0014);
    SERIALIZE_FIELD(68, &self->field_0018);
    SERIALIZE_REFERENCE(&self->field_0010);
    SERIALIZE_STRING(&self->field_0074);
    SERIALIZE_FIELD(84, &self->field_0080);
    SERIALIZE_FIELD(84, &CSF_AT(uint32_t, self, 0x24));
    SERIALIZE_FIELD(84, &self->field_0058);
    SERIALIZE_FIELD(84, &self->field_005c);
    SERIALIZE_FIELD(84, &self->field_0060);
    SERIALIZE_FIELD(84, &self->field_0064);
    SERIALIZE_FIELD(84, &self->field_0068);
    SERIALIZE_FIELD(84, &self->field_006c);
    SERIALIZE_FIELD(84, &self->field_0070);
    SERIALIZE_FIELD(84, &self->field_0084);
    SERIALIZE_FIELD(84, &self->field_0088);
    SERIALIZE_FIELD(84, &self->field_00a4);
    SERIALIZE_FIELD(84, &self->field_00a8);
    SERIALIZE_FIELD(56, &self->field_00ac);
    SERIALIZE_FIELD(84, &self->field_00b8);
    SERIALIZE_FIELD(84, &self->field_00bc);
    SERIALIZE_FIELD(84, &self->field_00c0);
    SERIALIZE_FIELD(84, &self->field_00c4);
    SERIALIZE_FIELD(84, &self->field_00c8);
    SERIALIZE_FIELD(28, &self->field_008c);
    SERIALIZE_FIELD(28, &self->field_009c);
    SERIALIZE_FIELD(84, &CSF_AT(uint32_t, self, 0x94));
    SERIALIZE_FIELD(84, &CSF_AT(uint32_t, self, 0x98));
    SERIALIZE_STRING(&self->field_00d0);
    SERIALIZE_STRING(&self->field_00dc);
    SERIALIZE_FIELD(56, &self->field_00e8);
    SERIALIZE_FIELD(56, &self->field_00f4);
    SERIALIZE_FIELD(84, &self->field_0104);
    SERIALIZE_FIELD(84, &self->field_0100);
    SERIALIZE_FIELD(84, &self->field_0108);
    SERIALIZE_FIELD(56, &self->field_0110);
    SERIALIZE_FIELD(84, &self->field_010c);
    SERIALIZE_FIELD(68, &self->field_00cc);

    const bool canSerializeSubmodel = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (canSerializeSubmodel) {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        CharacterModel_SerializeSubmodelData(&self->field_011c, archiveContext);
    }

    SERIALIZE_FIELD(72, &self->field_012c);
    SERIALIZE_FIELD(68, &self->field_0130);
    SERIALIZE_FIELD(84, &CSF_AT(uint32_t, self, 0x134));

    CSF_AT(uint32_t, context, 0x1C) = savedFieldState;
    if (CSF_AT(uint8_t, context, 0) != 0) {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    void* archive = CSF_AT(void*, context, 0x04);
    const int result =
        ((int(__fastcall*)(void*, void*, int))
             CSF_AT(void**, archive, 0)[124 / 4])
            (archive, nullptr, archiveContext);
    if (static_cast<uint8_t>(result) == 0) {
        CSF_AT(uint8_t, context, 0) = 1;
        return 1;
    }

    CSF_AT(uint8_t, context, 0) = 0;
    return result;
}

#undef SERIALIZE_FIELD
#undef SERIALIZE_REFERENCE
#undef SERIALIZE_STRING

HOOK(0x004F8B20, C_0083f1a0_Serialize, "auto");
