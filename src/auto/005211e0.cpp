#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, SceneLoader_SerializeReference, 0x00442E70, (void* a3, bool* a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int archiveAddress, S_00691860* string));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, FxParticleManager_GrowParticleCache, 0x0051FB70, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, FxParticleManager_GrowInstanceCache, 0x0051FC20, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, FxParticleManager_GrowEffectCache, 0x0051FCD0, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, FxParticleManager_GrowNamedEffectCache, 0x00520AE0, (void* self, void* edx, int a2));
CSF_FUNC(void**, __fastcall, SceneLoader_RegisterArchivedObject, 0x0069ACB0, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __cdecl, FxParticleManager_GetParticleController, 0x006CA810, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetObjectRegistry, 0x00726C40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0085e7b8_GetSlotEntry, 0x00768740, (C_0085e7b8* self, void* , int slotIndex));

static void* FxParticleManager_AsPointer(uint32_t address)
{
    return reinterpret_cast<void*>(address);
}

static bool FxParticleManager_PrepareArchiveField(S_00691860* archive, uint8_t reference)
{
    const bool failed = archive->pad_0000[0] != 0;
    CSF_AT(uint32_t, archive, 0x1C) = 0;
    if (failed)
        return false;
    reinterpret_cast<uint8_t*>(&archive->field_0014)[1] = reference;
    reinterpret_cast<uint8_t*>(&archive->field_0014)[2] = 0;
    return true;
}

static bool FxParticleManager_IsSaving(S_00691860* archive)
{
    return reinterpret_cast<uint8_t*>(&archive->field_0014)[0] != 0;
}

static void FxParticleManager_SerializeByte(S_00691860* archive, void* value)
{
    if (FxParticleManager_PrepareArchiveField(archive, 0)) {
        void* stream = FxParticleManager_AsPointer(archive->field_0004);
        archive->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, stream, 0)[0x44 / 4])(stream, nullptr, value, archive) == -1;
    }
}

static void FxParticleManager_SerializeDword(S_00691860* archive, void* value)
{
    if (FxParticleManager_PrepareArchiveField(archive, 0)) {
        void* stream = FxParticleManager_AsPointer(archive->field_0004);
        archive->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, stream, 0)[0x48 / 4])(stream, nullptr, value, archive) == -1;
    }
}

static void FxParticleManager_SerializeEffectValue(S_00691860* archive, void* value)
{
    if (FxParticleManager_PrepareArchiveField(archive, 0)) {
        void* stream = FxParticleManager_AsPointer(archive->field_0004);
        archive->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, stream, 0)[0x54 / 4])(stream, nullptr, value, archive) == -1;
    }
}

static void FxParticleManager_SerializeInstanceValue(S_00691860* archive, void* value)
{
    if (FxParticleManager_PrepareArchiveField(archive, 0)) {
        void* stream = FxParticleManager_AsPointer(archive->field_0004);
        archive->pad_0000[0] =
            ((int(__fastcall*)(void*, void*, void*, S_00691860*))
                CSF_AT(void**, stream, 0)[0x58 / 4])(stream, nullptr, value, archive) == -1;
    }
}

static void FxParticleManager_SerializeReference(S_00691860* archive, void* reference)
{
    if (FxParticleManager_PrepareArchiveField(archive, 1))
        SceneLoader_SerializeReference(reference, reinterpret_cast<bool*>(archive));
}

static uint8_t FxParticleManager_GetArchiveExclusion(uint32_t particleAddress)
{
    void* particle = FxParticleManager_AsPointer(particleAddress);
    if (((int(__fastcall*)(void*, void*))
            CSF_AT(void**, particle, 0)[0x10 / 4])(particle, nullptr))
        return 0;

    void* controller = FxParticleManager_AsPointer(
        FxParticleManager_GetParticleController(static_cast<int>(particleAddress)));
    return static_cast<uint8_t>(
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, controller, 0)[0x20 / 4])(controller, nullptr));
}

static int FxParticleManager_FindArchivedDefinition(int definitionId)
{
    void* manager = FxParticleManager_AsPointer(GetValue8B0554());
    void* definitions = CSF_AT(void*, manager, 0x1FCC);
    return ((int(__fastcall*)(void*, void*, int))
        CSF_AT(void**, definitions, 0)[8 / 4])(definitions, nullptr, definitionId);
}

static uint32_t FxParticleManager_CreateArchivedParticle(int definitionId)
{
    const int definition = FxParticleManager_FindArchivedDefinition(definitionId);
    void* loader = FxParticleManager_AsPointer(GetGlobalInt());
    void* registry = FxParticleManager_AsPointer(SceneLoader_GetObjectRegistry(loader, nullptr));
    return ((uint32_t(__fastcall*)(void*, void*, int))
        CSF_AT(void**, registry, 0)[0x14 / 4])(registry, nullptr, definition + 4);
}

static int __fastcall FxParticleManager_SerializeCaches(
    void* self, void* /*edx*/, S_00691860* archive)
{
    // Reserve the entire original local frame, including overlapping record locals.
    alignas(8) uint8_t localBytes[0x38] = {};
    int& owner = CSF_AT(int, localBytes, 0);
    void*& stringVtable = CSF_AT(void*, localBytes, 4);
    int& stringFirst = CSF_AT(int, localBytes, 8);
    int& stringSecond = CSF_AT(int, localBytes, 0xC);
    int& entryId = CSF_AT(int, localBytes, 0x10);
    S_00691860& record = *reinterpret_cast<S_00691860*>(localBytes + 0x14);
    int& temporaryReference = CSF_AT(int, localBytes, 0x2C);
    uint32_t& count = CSF_AT(uint32_t, localBytes, 0x34);

    bool failed = false;
    if (!archive->pad_0000[0]) {
        void* stream = FxParticleManager_AsPointer(archive->field_0004);
        failed = !((bool(__fastcall*)(void*, void*, int, S_00691860*))
            CSF_AT(void**, stream, 0)[0x80 / 4])(stream, nullptr, 0, archive);
    }

    CSF_AT(uint32_t, record.pad_0010, 0) = CSF_AT(uint32_t, archive, 0x1C);
    archive->pad_0000[0] = failed;
    if (FxParticleManager_IsSaving(archive))
        count = CSF_AT(uint32_t, self, 0x30);
    FxParticleManager_SerializeDword(archive, &count);

    record.field_0014 = 0;
    uint32_t offset = 0;
    while (record.field_0014 < count) {
        entryId = -1;
        CSF_AT(int, record.pad_0000, 0) = -1;
        record.field_0004 = 0;
        CSF_AT(int, record.pad_0008, 0) = -1;
        record.field_000c = 0;
        uint8_t excluded = 0;

        if (FxParticleManager_IsSaving(archive)) {
            uint8_t* entry = CSF_AT(uint8_t*, self, 0x28) + offset;
            entryId = CSF_AT(int, entry, 0);
            CSF_AT(int, record.pad_0000, 0) = CSF_AT(int, entry, 4);
            record.field_0004 = CSF_AT(uint32_t, entry, 8);
            CSF_AT(int, record.pad_0008, 0) = CSF_AT(int, entry, 0xC);
            record.field_000c = CSF_AT(uint32_t, entry, 0x10);
            excluded = FxParticleManager_GetArchiveExclusion(record.field_0004);
        }

        FxParticleManager_SerializeByte(archive, &excluded);
        if (!excluded) {
            FxParticleManager_SerializeDword(archive, record.pad_0008);
            if (CSF_AT(int, record.pad_0008, 0) != -1) {
                FxParticleManager_SerializeDword(archive, &entryId);
                FxParticleManager_SerializeDword(archive, &record);
                if (!FxParticleManager_IsSaving(archive) && !archive->pad_0000[0]) {
                    record.field_0004 = FxParticleManager_CreateArchivedParticle(
                        CSF_AT(int, record.pad_0008, 0));
                    void* manager = FxParticleManager_AsPointer(GetValue8B0554());
                    int slot = C_0085e7b8_GetSlotEntry(
                        CSF_AT(C_0085e7b8*, manager, 0x1F90), nullptr,
                        CSF_AT(int, record.pad_0000, 0));
                    void* particle = FxParticleManager_AsPointer(record.field_0004);
                    ((void(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, particle, 0)[0x38 / 4])(particle, nullptr, slot + 4);

                    if (CSF_AT(uint32_t, self, 0x30) >= CSF_AT(uint32_t, self, 0x2C))
                        FxParticleManager_GrowParticleCache(
                            static_cast<uint8_t*>(self) + 0x28, nullptr,
                            CSF_AT(int, self, 0x34));
                    uint32_t size = CSF_AT(uint32_t, self, 0x30);
                    uint8_t* data = CSF_AT(uint8_t*, self, 0x28);
                    CSF_AT(uint32_t, self, 0x30) = size + 1;
                    uint8_t* entry = data + 20 * size;
                    CSF_AT(int, entry, 0) = entryId;
                    CSF_AT(int, entry, 4) = CSF_AT(int, record.pad_0000, 0);
                    CSF_AT(uint32_t, entry, 8) = record.field_0004;
                    CSF_AT(int, entry, 0xC) = CSF_AT(int, record.pad_0008, 0);
                    CSF_AT(uint32_t, entry, 0x10) = record.field_000c;
                }
                FxParticleManager_SerializeReference(archive, &record.field_0004);
            }
        }
        ++record.field_0014;
        offset += 20;
    }

    if (FxParticleManager_IsSaving(archive))
        count = CSF_AT(uint32_t, self, 0x40);
    FxParticleManager_SerializeDword(archive, &count);
    record.field_0014 = 0;
    offset = 0;
    while (record.field_0014 < count) {
        record.field_0004 = -1;
        CSF_AT(int, record.pad_0008, 0) = -1;
        record.field_000c = 0;
        if (FxParticleManager_IsSaving(archive)) {
            uint8_t* entry = CSF_AT(uint8_t*, self, 0x38) + offset;
            record.field_0004 = CSF_AT(uint32_t, entry, 0);
            CSF_AT(int, record.pad_0008, 0) = CSF_AT(int, entry, 4);
            record.field_000c = CSF_AT(uint32_t, entry, 8);
        }

        FxParticleManager_SerializeDword(archive, record.pad_0008);
        if (CSF_AT(int, record.pad_0008, 0) != -1) {
            FxParticleManager_SerializeInstanceValue(archive, &record.field_0004);
            if (!FxParticleManager_IsSaving(archive) && !archive->pad_0000[0]) {
                int definition = FxParticleManager_FindArchivedDefinition(
                    CSF_AT(int, record.pad_0008, 0));
                void* loader = FxParticleManager_AsPointer(GetGlobalInt());
                void* registry = FxParticleManager_AsPointer(
                    SceneLoader_GetObjectRegistry(loader, nullptr));
                int particle = ((int(__fastcall*)(void*, void*, int, int))
                    CSF_AT(void**, registry, 0)[0x1C / 4])(
                        registry, nullptr, definition + 4, 0);
                record.field_000c = FxParticleManager_GetParticleController(particle);
                if (CSF_AT(uint32_t, self, 0x40) >= CSF_AT(uint32_t, self, 0x3C))
                    FxParticleManager_GrowInstanceCache(
                        static_cast<uint8_t*>(self) + 0x38, nullptr,
                        CSF_AT(int, self, 0x44));
                uint32_t size = CSF_AT(uint32_t, self, 0x40);
                uint8_t* entry = CSF_AT(uint8_t*, self, 0x38) + 12 * size;
                uint32_t value = record.field_0004;
                CSF_AT(uint32_t, self, 0x40) = size + 1;
                CSF_AT(uint32_t, entry, 0) = value;
                CSF_AT(int, entry, 4) = CSF_AT(int, record.pad_0008, 0);
                CSF_AT(uint32_t, entry, 8) = record.field_000c;
            }
            void* controller = FxParticleManager_AsPointer(record.field_000c);
            temporaryReference = ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, controller, 0)[4 / 4])(controller, nullptr);
            FxParticleManager_SerializeReference(archive, &temporaryReference);
        }
        ++record.field_0014;
        offset += 12;
    }

    if (FxParticleManager_IsSaving(archive))
        count = CSF_AT(uint32_t, self, 0x50);
    FxParticleManager_SerializeDword(archive, &count);
    record.field_0014 = 0;
    offset = 0;
    while (record.field_0014 < count) {
        CSF_AT(uint32_t, record.pad_0000, 0) = 0;
        record.field_0004 = 0;
        CSF_AT(int, record.pad_0008, 0) = -1;
        record.field_000c = -1;
        uint8_t excluded = 0;
        if (FxParticleManager_IsSaving(archive)) {
            uint8_t* entry = CSF_AT(uint8_t*, self, 0x48) + offset;
            CSF_AT(uint32_t, record.pad_0000, 0) = CSF_AT(uint32_t, entry, 0);
            record.field_0004 = CSF_AT(uint32_t, entry, 4);
            CSF_AT(int, record.pad_0008, 0) = CSF_AT(int, entry, 8);
            record.field_000c = CSF_AT(uint32_t, entry, 0xC);
            excluded = FxParticleManager_GetArchiveExclusion(
                CSF_AT(uint32_t, record.pad_0000, 0));
        }
        FxParticleManager_SerializeByte(archive, &excluded);
        if (!excluded) {
            FxParticleManager_SerializeDword(archive, record.pad_0008);
            if (CSF_AT(int, record.pad_0008, 0) != -1) {
                FxParticleManager_SerializeEffectValue(archive, &record.field_0004);
                if (!FxParticleManager_IsSaving(archive) && !archive->pad_0000[0]) {
                    CSF_AT(uint32_t, record.pad_0000, 0) =
                        FxParticleManager_CreateArchivedParticle(
                            CSF_AT(int, record.pad_0008, 0));
                    void* particle = CSF_AT(void*, record.pad_0000, 0);
                    ((void(__fastcall*)(void*, void*, int))
                        CSF_AT(void**, particle, 0)[0x28 / 4])(particle, nullptr, 0);
                    if (CSF_AT(uint32_t, self, 0x50) >= CSF_AT(uint32_t, self, 0x4C))
                        FxParticleManager_GrowEffectCache(
                            static_cast<uint8_t*>(self) + 0x48, nullptr,
                            CSF_AT(int, self, 0x54));
                    uint8_t* entry = CSF_AT(uint8_t*, self, 0x48) +
                        16 * CSF_AT(uint32_t, self, 0x50);
                    uint32_t particleAddress = CSF_AT(uint32_t, record.pad_0000, 0);
                    ++CSF_AT(uint32_t, self, 0x50);
                    CSF_AT(uint32_t, entry, 0) = particleAddress;
                    CSF_AT(uint32_t, entry, 4) = record.field_0004;
                    CSF_AT(int, entry, 8) = CSF_AT(int, record.pad_0008, 0);
                    CSF_AT(uint32_t, entry, 0xC) = record.field_000c;
                }
                FxParticleManager_SerializeReference(archive, &record);
            }
        }
        ++record.field_0014;
        offset += 16;
    }

    if (FxParticleManager_IsSaving(archive))
        count = CSF_AT(uint32_t, self, 0x60);
    FxParticleManager_SerializeDword(archive, &count);
    record.field_0014 = 0;
    offset = 0;
    while (record.field_0014 < count) {
        stringVtable = CSF_VTBL(0x0082D448);
        stringFirst = -1;
        stringSecond = -1;
        owner = 0;
        entryId = -1;
        CSF_AT(uint32_t, record.pad_0000, 0) = 0;
        record.field_0004 = 0;
        CSF_AT(int, record.pad_0008, 0) = -1;
        record.field_000c = -1;
        uint8_t excluded = 0;
        if (FxParticleManager_IsSaving(archive)) {
            uint8_t* entry = CSF_AT(uint8_t*, self, 0x58) + offset;
            owner = CSF_AT(int, entry, 0);
            stringFirst = CSF_AT(int, entry, 8);
            stringSecond = CSF_AT(int, entry, 0xC);
            entryId = CSF_AT(int, entry, 0x10);
            CSF_AT(uint32_t, record.pad_0000, 0) = CSF_AT(uint32_t, entry, 0x14);
            record.field_0004 = CSF_AT(uint32_t, entry, 0x18);
            CSF_AT(int, record.pad_0008, 0) = CSF_AT(int, entry, 0x1C);
            record.field_000c = CSF_AT(uint32_t, entry, 0x20);
            excluded = FxParticleManager_GetArchiveExclusion(
                CSF_AT(uint32_t, record.pad_0000, 0));
        }
        FxParticleManager_SerializeByte(archive, &excluded);
        if (!excluded) {
            FxParticleManager_SerializeDword(archive, record.pad_0008);
            if (CSF_AT(int, record.pad_0008, 0) != -1) {
                if (FxParticleManager_PrepareArchiveField(archive, 0))
                    Archive_SerializeString(
                        reinterpret_cast<int>(&stringVtable), archive);
                FxParticleManager_SerializeDword(archive, &entryId);
                if (!FxParticleManager_IsSaving(archive) && !archive->pad_0000[0]) {
                    CSF_AT(uint32_t, record.pad_0000, 0) =
                        FxParticleManager_CreateArchivedParticle(
                            CSF_AT(int, record.pad_0008, 0));
                    if (CSF_AT(uint32_t, self, 0x60) >= CSF_AT(uint32_t, self, 0x5C)) {
                        // The packet also identifies this address as a vtable reference.
                        (void)CSF_VTBL(0x00520AE0);
                        FxParticleManager_GrowNamedEffectCache(
                            static_cast<uint8_t*>(self) + 0x58, nullptr,
                            CSF_AT(int, self, 0x64));
                    }
                    uint32_t size = CSF_AT(uint32_t, self, 0x60);
                    uint8_t* entry = CSF_AT(uint8_t*, self, 0x58) + 36 * size;
                    CSF_AT(uint32_t, self, 0x60) = size + 1;
                    CSF_AT(int, entry, 0) = owner;
                    CSF_AT(int, entry, 8) = stringFirst;
                    CSF_AT(int, entry, 0xC) = stringSecond;
                    CSF_AT(int, entry, 0x10) = entryId;
                    CSF_AT(uint32_t, entry, 0x14) = CSF_AT(uint32_t, record.pad_0000, 0);
                    CSF_AT(uint32_t, entry, 0x18) = record.field_0004;
                    CSF_AT(int, entry, 0x1C) = CSF_AT(int, record.pad_0008, 0);
                    CSF_AT(uint32_t, entry, 0x20) = record.field_000c;
                }
                FxParticleManager_SerializeReference(archive, &record);
            }
        }
        stringVtable = CSF_VTBL(0x0082D410);
        ++record.field_0014;
        offset += 36;
    }

    if (!FxParticleManager_IsSaving(archive) && !archive->pad_0000[0])
        SceneLoader_RegisterArchivedObject(
            CSF_AT(void*, archive->pad_0010, 0), nullptr,
            reinterpret_cast<uint32_t>(self) != 4 ? reinterpret_cast<int>(self) : 0);

    const bool hadError = archive->pad_0000[0] != 0;
    CSF_AT(uint32_t, archive, 0x1C) = CSF_AT(uint32_t, record.pad_0010, 0);
    if (!hadError) {
        void* stream = FxParticleManager_AsPointer(archive->field_0004);
        int result = ((int(__fastcall*)(void*, void*, S_00691860*))
            CSF_AT(void**, stream, 0)[0x7C / 4])(stream, nullptr, archive);
        if (static_cast<uint8_t>(result)) {
            archive->pad_0000[0] = 0;
            return result;
        }
    }
    archive->pad_0000[0] = 1;
    return 1;
}

HOOK(0x005211E0, FxParticleManager_SerializeCaches, "auto");
