#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AiTaskStates_SerializeBaseState, 0x0041F0A0,
         (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, AiTaskStates_SerializeStateParameters, 0x0041F200,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, AiTaskStates_SerializeStateRecord, 0x0041F8C0,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, AiTaskStates_SerializeTaskState, 0x0047BCB0,
         (int a1, void* a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080,
         (int archiveAddress, S_00691860* string));

static bool AiTaskStates_PrepareArchiveField(void* archive)
{
    const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(int, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(uint8_t, archive, 0x15) = 0;
        CSF_AT(uint8_t, archive, 0x16) = 0;
    }
    return ready;
}

// The slot is fixed at compile time, never selected dynamically.
template<std::size_t Slot>
static void AiTaskStates_SerializeField(void* archive, void* field)
{
    if (AiTaskStates_PrepareArchiveField(archive))
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[Slot])(
                    serializer, nullptr, field, archive);
        CSF_AT(uint8_t, archive, 0x00) = result == -1;
    }
}

static void AiTaskStates_SerializeTaskStateField(void* archive, void* state)
{
    if (AiTaskStates_PrepareArchiveField(archive))
        AiTaskStates_SerializeTaskState(reinterpret_cast<int>(state), archive);
}

static int __fastcall S_0041f0a0_SerializeNavigationState(
    S_0041f0a0* self, void* /*edx*/, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);

    // The packet labels this call target as a vtable reference.
    // It is not a vtable store in the original instruction stream.
    (void)CSF_VTBL(0x0041F0A0);

    bool failed = false;
    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, int, void*)>(
            CSF_AT(void**, serializer, 0)[128 / 4])(
                serializer, nullptr, 0, archive);
    }

    const int savedContext = CSF_AT(int, archive, 0x1C);
    CSF_AT(uint8_t, archive, 0x00) = failed;
    AiTaskStates_SerializeBaseState(self, nullptr, archive);

    int stateType = 0;
    if (CSF_AT(uint8_t, archive, 0x14) != 0)
    {
        void* state = reinterpret_cast<void*>(self->field_000c);
        if (state)
        {
            stateType = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                CSF_AT(void**, state, 0)[128 / 4])(state, nullptr);
        }
    }

    AiTaskStates_SerializeField<88 / 4>(archive, &stateType);
    AiTaskStates_SerializeField<88 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x1F8));
    AiTaskStates_SerializeField<72 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x26C));

    if (AiTaskStates_PrepareArchiveField(archive))
    {
        Archive_SerializeString(
            reinterpret_cast<int>(&CSF_AT(uint8_t, self, 0x258)),
            reinterpret_cast<S_00691860*>(archive));
    }

    AiTaskStates_SerializeField<84 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x1FC));
    AiTaskStates_SerializeField<68 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x200));
    AiTaskStates_SerializeField<68 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x201));
    AiTaskStates_SerializeField<84 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x204));
    AiTaskStates_SerializeField<56 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x208));
    AiTaskStates_SerializeField<56 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x214));
    AiTaskStates_SerializeField<56 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x220));
    AiTaskStates_SerializeField<68 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x22C));
    AiTaskStates_SerializeField<88 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x234));
    AiTaskStates_SerializeField<84 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x238));
    AiTaskStates_SerializeField<84 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x23C));

    if (AiTaskStates_PrepareArchiveField(archive))
    {
        AiTaskStates_SerializeStateParameters(
            &CSF_AT(uint8_t, self, 0x240), nullptr, archiveAddress);
    }

    AiTaskStates_SerializeField<68 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x270));
    AiTaskStates_SerializeField<56 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x274));
    AiTaskStates_SerializeField<56 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x280));
    AiTaskStates_SerializeField<56 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x28C));

    if (AiTaskStates_PrepareArchiveField(archive))
    {
        AiTaskStates_SerializeStateRecord(
            &CSF_AT(uint8_t, self, 0x298), nullptr, archiveAddress);
    }

    AiTaskStates_SerializeField<68 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x2B4));
    AiTaskStates_SerializeField<88 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x2B8));
    AiTaskStates_SerializeField<88 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x2BC));
    AiTaskStates_SerializeField<88 / 4>(
        archive, &CSF_AT(uint8_t, self, 0x2C0));

    if (CSF_AT(int, archive, 0x18) > 50 || stateType == 0)
        AiTaskStates_SerializeTaskStateField(
            archive, &CSF_AT(uint8_t, self, 0x1C));

    if (CSF_AT(int, archive, 0x18) > 50 || stateType == 1)
        AiTaskStates_SerializeTaskStateField(
            archive, &CSF_AT(uint8_t, self, 0x30));

    if (CSF_AT(int, archive, 0x18) > 50 || stateType == 2)
        AiTaskStates_SerializeTaskStateField(
            archive, &CSF_AT(uint8_t, self, 0x44));

    if (CSF_AT(int, archive, 0x18) > 50 || stateType == 6)
        AiTaskStates_SerializeTaskStateField(
            archive, &CSF_AT(uint8_t, self, 0xFC));

    if (CSF_AT(int, archive, 0x18) > 50 || stateType == 4)
        AiTaskStates_SerializeTaskStateField(
            archive, &CSF_AT(uint8_t, self, 0x70));

    if (CSF_AT(int, archive, 0x18) > 50 || stateType == 3)
        AiTaskStates_SerializeTaskStateField(
            archive, &CSF_AT(uint8_t, self, 0x5C));

    if (CSF_AT(int, archive, 0x18) > 50 || stateType == 7)
        AiTaskStates_SerializeTaskStateField(
            archive, &CSF_AT(uint8_t, self, 0x17C));

    AiTaskStates_SerializeTaskStateField(
        archive, &CSF_AT(uint8_t, self, 0x190));
    AiTaskStates_SerializeTaskStateField(
        archive, &CSF_AT(uint8_t, self, 0x1A4));
    AiTaskStates_SerializeTaskStateField(
        archive, &CSF_AT(uint8_t, self, 0xAC));

    const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
    CSF_AT(int, archive, 0x1C) = savedContext;
    if (ready)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        // The low byte is tested, but the pseudocode returns the full result.
        const int result =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*)>(
                CSF_AT(void**, serializer, 0)[124 / 4])(
                    serializer, nullptr, archive);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x00423730, S_0041f0a0_SerializeNavigationState, "auto");
