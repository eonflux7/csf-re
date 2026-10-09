#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(int, __fastcall, AudioSoundManager_GrowPrimaryEntries, 0x00522BA0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AudioSoundManager_GrowTertiaryEntries, 0x00522C40, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, AudioSoundManager_GrowSecondaryEntries, 0x00523660, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AudioSoundManager_SerializePrimaryEntry, 0x00523D90, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AudioSoundManager_SerializeSettings, 0x00523E30, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AudioSoundManager_SerializeSecondaryEntry, 0x005248E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AudioSoundManager_SerializeTertiaryEntry, 0x00524970, (void* self, void* edx, int a2));
CSF_FUNC(int, __stdcall, AudioSoundManager_SerializeAssociations, 0x00524A00, (int* a1, int a2));

static bool AudioSoundManager_BeginArchiveField(void* archive)
{
    const bool ready = !CSF_AT(bool, archive, 0);
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
    }
    return ready;
}

static int __fastcall AudioSoundManager_Serialize(
    void* self, void* /*edx*/, unsigned int archiveArgument)
{
    // These packet annotations identify the two container growth bindings.
    (void)CSF_VTBL(0x00522BA0);
    (void)CSF_VTBL(0x00523660);

    auto* initialOwner = static_cast<std::uint8_t*>(self);
    void* archive = reinterpret_cast<void*>(archiveArgument);
    unsigned int primaryCount = archiveArgument;
    unsigned int tertiaryCount = 0;

    // The count's address has eight bytes of writable stack storage.
    struct CountAndOwner
    {
        unsigned int count;
        std::uint8_t* owner;
    } secondary = {0, initialOwner};

    bool failed = false;
    if (!CSF_AT(bool, archive, 0))
    {
        void* stream = CSF_AT(void*, archive, 4);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, stream, 0)[128 / 4])(
                stream, nullptr, 0, archive);
    }

    const std::uint32_t savedContext = CSF_AT(std::uint32_t, archive, 0x1C);
    CSF_AT(bool, archive, 0) = failed;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (!failed)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(initialOwner + 64, static_cast<bool*>(archive));
    }

    if (AudioSoundManager_BeginArchiveField(archive))
        Archive_SerializeObjectRef(initialOwner + 68, static_cast<bool*>(archive));

    if (AudioSoundManager_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 4);
        CSF_AT(bool, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, stream, 0)[68 / 4])(
                    stream, nullptr, initialOwner + 116, archive) == -1;
    }

    if (CSF_AT(bool, archive, 20) && !CSF_AT(bool, archive, 0))
    {
        primaryCount = CSF_AT(unsigned int, initialOwner, 20);
        secondary.count = CSF_AT(unsigned int, initialOwner, 36);
        tertiaryCount = CSF_AT(unsigned int, initialOwner, 52);
    }

    if (AudioSoundManager_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 4);
        CSF_AT(bool, archive, 0) =
            ((int(__fastcall*)(void*, void*, unsigned int*, void*))
                CSF_AT(void**, stream, 0)[72 / 4])(
                    stream, nullptr, &primaryCount, archive) == -1;
    }

    if (AudioSoundManager_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 4);
        CSF_AT(bool, archive, 0) =
            ((int(__fastcall*)(void*, void*, unsigned int*, void*))
                CSF_AT(void**, stream, 0)[72 / 4])(
                    stream, nullptr, &secondary.count, archive) == -1;
    }

    if (AudioSoundManager_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 4);
        CSF_AT(bool, archive, 0) =
            ((int(__fastcall*)(void*, void*, unsigned int*, void*))
                CSF_AT(void**, stream, 0)[72 / 4])(
                    stream, nullptr, &tertiaryCount, archive) == -1;
    }

    auto* primaryEntries = reinterpret_cast<S_00522ba0*>(initialOwner + 12);
    unsigned int capacity = CSF_AT(unsigned int, initialOwner, 16);
    if (primaryCount > capacity)
        AudioSoundManager_GrowPrimaryEntries(
            primaryEntries, nullptr, static_cast<int>(primaryCount - capacity));

    capacity = CSF_AT(unsigned int, secondary.owner, 48);
    if (tertiaryCount > capacity)
        AudioSoundManager_GrowTertiaryEntries(
            secondary.owner + 44, nullptr, static_cast<int>(tertiaryCount - capacity));

    capacity = CSF_AT(unsigned int, secondary.owner, 32);
    if (secondary.count > capacity)
        AudioSoundManager_GrowSecondaryEntries(
            secondary.owner + 28, nullptr, static_cast<int>(secondary.count - capacity));

    if (AudioSoundManager_BeginArchiveField(archive))
        AudioSoundManager_SerializeSettings(
            secondary.owner + 92, nullptr, reinterpret_cast<int>(archive));

    if (AudioSoundManager_BeginArchiveField(archive))
    {
        void* stream = CSF_AT(void*, archive, 4);
        CSF_AT(bool, archive, 0) =
            ((int(__fastcall*)(void*, void*, void*, void*))
                CSF_AT(void**, stream, 0)[56 / 4])(
                    stream, nullptr, secondary.owner + 100, archive) == -1;
    }

    unsigned int offset = 0;
    for (unsigned int index = 0; index < primaryCount; ++index, offset += 12)
    {
        if (!CSF_AT(bool, archive, 20) && !CSF_AT(bool, archive, 0))
        {
            if (primaryEntries->field_0008 >= primaryEntries->field_0004)
                AudioSoundManager_GrowPrimaryEntries(
                    primaryEntries, nullptr, static_cast<int>(primaryEntries->field_000c));

            const unsigned int count = primaryEntries->field_0008;
            const unsigned int data = primaryEntries->field_0000;
            primaryEntries->field_0008 = count + 1;
            void* entry = reinterpret_cast<void*>(data + 12 * count);
            CSF_AT(std::uint32_t, entry, 0) = 0;
            CSF_AT(std::uint32_t, entry, 4) = 0;
            CSF_AT(std::int32_t, entry, 8) = -1082130432;
        }

        void* entry = reinterpret_cast<void*>(primaryEntries->field_0000 + offset);
        if (AudioSoundManager_BeginArchiveField(archive))
            AudioSoundManager_SerializePrimaryEntry(
                entry, nullptr, reinterpret_cast<int>(archive));
    }

    offset = 0;
    for (unsigned int index = 0; index < secondary.count; ++index, offset += 24)
    {
        if (!CSF_AT(bool, archive, 20) && !CSF_AT(bool, archive, 0))
        {
            if (CSF_AT(unsigned int, secondary.owner, 36) >=
                CSF_AT(unsigned int, secondary.owner, 32))
            {
                AudioSoundManager_GrowSecondaryEntries(
                    secondary.owner + 28, nullptr,
                    CSF_AT(int, secondary.owner, 40));
            }

            const unsigned int count = CSF_AT(unsigned int, secondary.owner, 36);
            void* entry = reinterpret_cast<void*>(
                CSF_AT(unsigned int, secondary.owner, 28) + 24 * count);
            CSF_AT(unsigned int, secondary.owner, 36) = count + 1;
            CSF_AT(std::uint32_t, entry, 4) = 0;
            CSF_AT(std::uint32_t, entry, 0) = 0;
            CSF_AT(std::int32_t, entry, 12) = -1;
            CSF_AT(std::int32_t, entry, 16) = -1;
            CSF_AT(std::int32_t, entry, 20) = -1082130432;
        }

        void* entry = reinterpret_cast<void*>(
            CSF_AT(unsigned int, secondary.owner, 28) + offset);
        if (AudioSoundManager_BeginArchiveField(archive))
            AudioSoundManager_SerializeSecondaryEntry(
                entry, nullptr, reinterpret_cast<int>(archive));
    }

    offset = 0;
    for (unsigned int index = 0; index < tertiaryCount; ++index, offset += 24)
    {
        if (!CSF_AT(bool, archive, 20) && !CSF_AT(bool, archive, 0))
        {
            if (CSF_AT(unsigned int, secondary.owner, 52) >=
                CSF_AT(unsigned int, secondary.owner, 48))
            {
                AudioSoundManager_GrowTertiaryEntries(
                    secondary.owner + 44, nullptr,
                    CSF_AT(int, secondary.owner, 56));
            }

            const unsigned int count = CSF_AT(unsigned int, secondary.owner, 52);
            void* entry = reinterpret_cast<void*>(
                CSF_AT(unsigned int, secondary.owner, 44) + 24 * count);
            CSF_AT(unsigned int, secondary.owner, 52) = count + 1;
            CSF_AT(std::uint32_t, entry, 0) = 0;
            CSF_AT(std::uint32_t, entry, 4) = 0;
            CSF_AT(std::uint32_t, entry, 8) = 0;
            CSF_AT(std::uint32_t, entry, 12) = 0;
            CSF_AT(std::uint32_t, entry, 16) = 0;
            CSF_AT(std::int32_t, entry, 20) = -1082130432;
        }

        void* entry = reinterpret_cast<void*>(
            CSF_AT(unsigned int, secondary.owner, 44) + offset);
        if (AudioSoundManager_BeginArchiveField(archive))
            AudioSoundManager_SerializeTertiaryEntry(
                entry, nullptr, reinterpret_cast<int>(archive));
    }

    if (AudioSoundManager_BeginArchiveField(archive))
        AudioSoundManager_SerializeAssociations(
            reinterpret_cast<int*>(secondary.owner + 72),
            reinterpret_cast<int>(archive));

    const bool readyToFinish = !CSF_AT(bool, archive, 0);
    CSF_AT(std::uint32_t, archive, 0x1C) = savedContext;
    if (readyToFinish)
    {
        void* stream = CSF_AT(void*, archive, 4);
        const bool finished =
            ((bool(__fastcall*)(void*, void*, void*))
                CSF_AT(void**, stream, 0)[124 / 4])(
                    stream, nullptr, archive);
        if (finished)
        {
            CSF_AT(bool, archive, 0) = false;
            return finished;
        }
    }

    CSF_AT(bool, archive, 0) = true;
    return 1;
}

HOOK(0x00524BF0, AudioSoundManager_Serialize, "auto");
