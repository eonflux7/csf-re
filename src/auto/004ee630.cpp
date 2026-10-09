#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, DestroySceneObjectOptionsStorage, 0x004018E0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(uint32_t, __fastcall, SceneLoader_SetSectorEnabled, 0x004ED1C0, (S_004e8e00* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, S_004e8e00_PrepareIndexedValue, 0x004EDDB0, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, GetGlobalState_50D190, 0x0050D190, ());
CSF_FUNC(char, __fastcall, GetByteAtIndex, 0x0051AEB0, (void* self, void*, int index));
CSF_FUNC(int, __fastcall, GetProviderStateIndex, 0x0051AEC0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_AssociateProviderSector, 0x0051AF30, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, GlobalInt_ResolveAccessor, 0x006B13A0, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, SceneMissionSetup_InitializeObjectOptions, 0x006B2750, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetWorldObjectCollection, 0x006B6AF0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneMissionSetup_ApplyObjectOptions, 0x006CEE30, (void* self, void* edx, char* a2));
CSF_FUNC(int, __cdecl, IsStateCodeAllowed, 0x00755810, (unsigned int a1));
CSF_FUNC(int, __cdecl, IsSubStateEnabled, 0x00760F90, (int a1));
CSF_FUNC(int, __fastcall, SceneLoader_GetSectorRecord, 0x00762470, (void* self, void* edx, int a2));

CSF_GLOBAL(float, g_DefaultObjectOptionX, 0x0083169C);
CSF_GLOBAL(float, g_DefaultObjectOptionY, 0x008316A0);
CSF_GLOBAL(float, g_DefaultObjectOptionZ, 0x008316A4);

// Document the original constant-data words; use the bound data for the copies.
static constexpr std::uint32_t k_DefaultObjectOptionDataWords[] = {
    0x3F800000, 0x3F666666, 0x3F19999A
};
static const auto k_WorldObjectCollectionTable = CSF_VTBL(0x006CEE30);

static unsigned int __fastcall S_004e8e00_SetIndexedValue(
    S_004e8e00* self, void* /*edx*/, int index, int stateCode)
{
    alignas(4) char options[0x64]{};
    alignas(4) unsigned char activation[0xC]{};

    unsigned int result = static_cast<unsigned char>(
        GetByteAtIndex(reinterpret_cast<void*>(self->field_1fc0), nullptr, index));
    activation[0xA] = static_cast<unsigned char>(result);

    if ((activation[0xA] & 1) == 0)
    {
        void* provider = reinterpret_cast<void*>(self->field_1fc0);
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, provider, 0)[0x14 / 4])(provider, nullptr, index, 1);

        provider = reinterpret_cast<void*>(self->field_1fc0);
        auto* record = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, provider, 0)[0x08 / 4])(provider, nullptr, index));

        activation[0xB] = CSF_AT(unsigned char,
            reinterpret_cast<void*>(self->field_1fdc), 0x2C);
        S_004e8e00_PrepareIndexedValue(self, nullptr, index);

        if (record[20])
        {
            void* controller = reinterpret_cast<void*>(SceneLoader_GetSceneController(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
            void* name = record[21] < 0x10u
                ? static_cast<void*>(record + 16)
                : reinterpret_cast<void*>(record[16]);
            reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, controller, 0)[0x10 / 4])(controller, nullptr, name, 1);
        }

        if (record[27])
        {
            void* controller = reinterpret_cast<void*>(SceneLoader_GetSceneController(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
            void* name = record[28] < 0x10u
                ? static_cast<void*>(record + 23)
                : reinterpret_cast<void*>(record[23]);
            reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, controller, 0)[0x10 / 4])(controller, nullptr, name, 1);

            SceneMissionSetup_InitializeObjectOptions(options, nullptr);
            if (record[124])
            {
                CSF_AT(std::uint32_t, options, 0) = record[124];
                AssignStringSubstring(options + 0x4, nullptr, record + 125, 0, -1);
                AssignStringSubstring(options + 0x20, nullptr, record + 22, 0, -1);
                CSF_AT(float, options, 0x3C) = CSF_AT(float, record, 0x1D8);
                CSF_AT(float, options, 0x40) = CSF_AT(float, record, 0x1DC);
                CSF_AT(float, options, 0x44) = CSF_AT(float, record, 0x1E0);
                CSF_AT(float, options, 0x48) = CSF_AT(float, record, 0x1E4);
                CSF_AT(float, options, 0x4C) = CSF_AT(float, record, 0x1E8);
                CSF_AT(float, options, 0x50) = CSF_AT(float, record, 0x1EC);
                CSF_AT(float, options, 0x54) = CSF_AT(float, record, 0x21C);
                CSF_AT(float, options, 0x58) = CSF_AT(float, record, 0x220);
                CSF_AT(float, options, 0x5C) = CSF_AT(float, record, 0x224);
            }
            else
            {
                CSF_AT(std::uint32_t, options, 0) = 2;
                AssignStringSubstring(options + 0x20, nullptr, record + 22, 0, -1);
                CSF_AT(float, options, 0x54) = g_DefaultObjectOptionX;
                CSF_AT(float, options, 0x58) = g_DefaultObjectOptionY;
                CSF_AT(float, options, 0x5C) = g_DefaultObjectOptionZ;
            }

            void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
            void* collection = reinterpret_cast<void*>(
                SceneLoader_GetWorldObjectCollection(accessor, nullptr));
            SceneMissionSetup_ApplyObjectOptions(collection, nullptr, options);
            DestroySceneObjectOptionsStorage(options, nullptr);
        }

        if (record[39])
        {
            void* controller = reinterpret_cast<void*>(SceneLoader_GetSceneController(
                reinterpret_cast<void*>(GetGlobalInt()), nullptr));
            void* name = record[40] < 0x10u
                ? static_cast<void*>(record + 35)
                : reinterpret_cast<void*>(record[35]);
            reinterpret_cast<void(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, controller, 0)[0x10 / 4])(controller, nullptr, name, 1);
        }

        void* accessor = reinterpret_cast<void*>(GlobalInt_ResolveAccessor(
            reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        void* destination = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, accessor, 0)[0x10 / 4])(accessor, nullptr);
        void* destinationName = record[47] < 0x10u
            ? static_cast<void*>(record + 42)
            : reinterpret_cast<void*>(record[42]);
        reinterpret_cast<void(__fastcall*)(void*, void*, void*)>(
            CSF_AT(void**, destination, 0)[0x1C / 4])(destination, nullptr, destinationName);

        std::uint32_t subState = record[79];
        if (subState == static_cast<std::uint32_t>(-1))
            IsSubStateEnabled(record[29]);
        else
        {
            void* states = reinterpret_cast<void*>(self->field_1fd0);
            reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
                CSF_AT(void**, states, 0)[0x1C / 4])(states, nullptr, subState);
        }

        subState = record[76];
        if (subState == static_cast<std::uint32_t>(-1))
            IsSubStateEnabled(record[29]);
        else
        {
            void* states = reinterpret_cast<void*>(self->field_1fd0);
            reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
                CSF_AT(void**, states, 0)[0x1C / 4])(states, nullptr, subState);
        }

        subState = record[77];
        if (subState != static_cast<std::uint32_t>(-1))
        {
            void* states = reinterpret_cast<void*>(self->field_1fd0);
            reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
                CSF_AT(void**, states, 0)[0x1C / 4])(states, nullptr, subState);
        }

        subState = record[78];
        if (subState != static_cast<std::uint32_t>(-1))
        {
            void* states = reinterpret_cast<void*>(self->field_1fd0);
            reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t)>(
                CSF_AT(void**, states, 0)[0x1C / 4])(states, nullptr, subState);
        }

        activation[0] = 0;
        activation[1] = 1;
        CSF_AT(std::uint32_t, activation, 4) = 0;
        std::uint32_t offset = 0;
        for (;;)
        {
            std::uint32_t begin = record[86];
            if (!begin || CSF_AT(std::uint32_t, activation, 4) >=
                static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(record[87] - begin) / 12))
                break;

            void* states = reinterpret_cast<void*>(self->field_1fc4);
            std::uint32_t value = CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(record[86] + offset), 8);
            reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t, void*)>(
                CSF_AT(void**, states, 0)[0x1C / 4])(states, nullptr, value, activation);
            ++CSF_AT(std::uint32_t, activation, 4);
            offset += 12;
        }

        offset = 0;
        CSF_AT(std::uint32_t, activation, 4) = 0;
        for (;;)
        {
            std::uint32_t begin = record[90];
            if (!begin || CSF_AT(std::uint32_t, activation, 4) >=
                static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(record[91] - begin) / 12))
                break;

            void* entry = reinterpret_cast<void*>(record[90] + offset);
            int type = CSF_AT(int, entry, 4);
            if ((type != 32 && type != 33) || activation[0xB])
            {
                void* states = reinterpret_cast<void*>(self->field_1fc4);
                std::uint32_t value = CSF_AT(std::uint32_t, entry, 8);
                reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t, void*)>(
                    CSF_AT(void**, states, 0)[0x1C / 4])(states, nullptr, value, activation);
            }
            ++CSF_AT(std::uint32_t, activation, 4);
            offset += 12;
        }

        if (CSF_AT(unsigned char, reinterpret_cast<void*>(GetGlobalState_50D190()), 208))
        {
            activation[1] = 0;
            activation[0] = 1;
            CSF_AT(std::uint32_t, activation, 4) = 0;
            offset = 0;
            for (;;)
            {
                std::uint32_t begin = record[98];
                if (!begin || CSF_AT(std::uint32_t, activation, 4) >=
                    static_cast<std::uint32_t>(
                        static_cast<std::int32_t>(record[99] - begin) / 12))
                    break;

                void* states = reinterpret_cast<void*>(self->field_1fc4);
                std::uint32_t value = CSF_AT(std::uint32_t,
                    reinterpret_cast<void*>(record[98] + offset), 8);
                reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t, void*)>(
                    CSF_AT(void**, states, 0)[0x1C / 4])(states, nullptr, value, activation);
                ++CSF_AT(std::uint32_t, activation, 4);
                offset += 12;
            }
        }

        activation[1] = 0;
        activation[0] = 1;
        CSF_AT(std::uint32_t, activation, 4) = 0;
        offset = 0;
        for (;;)
        {
            std::uint32_t begin = record[94];
            if (!begin || CSF_AT(std::uint32_t, activation, 4) >=
                static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(record[95] - begin) / 12))
                break;

            void* states = reinterpret_cast<void*>(self->field_1fc4);
            std::uint32_t value = CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(record[94] + offset), 8);
            reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t, void*)>(
                CSF_AT(void**, states, 0)[0x1C / 4])(states, nullptr, value, activation);
            ++CSF_AT(std::uint32_t, activation, 4);
            offset += 12;
        }

        result = GetProviderStateIndex(
            reinterpret_cast<void*>(self->field_1fc0), nullptr, index);
        if (result == static_cast<unsigned int>(-1))
        {
            void* sectors = reinterpret_cast<void*>(self->field_1fc8);
            void* storage = reinterpret_cast<void*>(CSF_AT(std::uint32_t, sectors, 8));
            std::uint32_t begin = CSF_AT(std::uint32_t, storage, 4);
            std::uint32_t count = begin
                ? static_cast<std::uint32_t>(static_cast<std::int32_t>(
                    CSF_AT(std::uint32_t, storage, 8) - begin) / 556)
                : 0;
            result = count;

            for (std::uint32_t sectorIndex = 0; sectorIndex < count; ++sectorIndex)
            {
                void* sector = reinterpret_cast<void*>(SceneLoader_GetSectorRecord(
                    reinterpret_cast<void*>(self->field_1fc8), nullptr, sectorIndex));
                if (CSF_AT(int, sector, 180) == 22 &&
                    CSF_AT(std::uint32_t, sector, 192) == record[0])
                {
                    sectors = reinterpret_cast<void*>(self->field_1fc8);
                    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(
                        CSF_AT(void**, sectors, 0)[0x18 / 4])(sectors, nullptr, sectorIndex);
                    SceneLoader_SetSectorEnabled(self, nullptr, sectorIndex, 1);
                    result = reinterpret_cast<std::uintptr_t>(
                        SceneMissionSetup_AssociateProviderSector(
                            reinterpret_cast<void*>(self->field_1fc0), nullptr,
                            index, CSF_AT(int, sector, 0)));
                    break;
                }
                result = count;
            }
        }
    }

    if ((activation[0xA] & 2) == 0)
    {
        void* provider = reinterpret_cast<void*>(self->field_1fc0);
        auto* record = reinterpret_cast<std::uint32_t*>(
            reinterpret_cast<void*(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, provider, 0)[0x08 / 4])(provider, nullptr, index));

        bool allowed = static_cast<unsigned char>(IsStateCodeAllowed(stateCode)) != 0;
        bool activate = allowed && stateCode != 0;
        if (!activate)
        {
            result = record[29];
            activate = result == 6 || result == 16 || result == 17;
        }

        if (activate)
        {
            provider = reinterpret_cast<void*>(self->field_1fc0);
            reinterpret_cast<void(__fastcall*)(void*, void*, int, int)>(
                CSF_AT(void**, provider, 0)[0x14 / 4])(provider, nullptr, index, 2);

            stateCode = static_cast<int>(
                (static_cast<std::uint32_t>(stateCode) & 0xFFFF0000u) | 1u);
            std::uint32_t offset = 0;
            for (std::uint32_t entryIndex = 0;; ++entryIndex)
            {
                result = record[102];
                if (!result)
                    break;
                result = static_cast<std::int32_t>(record[103] - result) / 12;
                if (entryIndex >= result)
                    break;

                void* states = reinterpret_cast<void*>(self->field_1fc4);
                std::uint32_t value = CSF_AT(std::uint32_t,
                    reinterpret_cast<void*>(record[102] + offset), 8);
                reinterpret_cast<void(__fastcall*)(void*, void*, std::uint32_t, void*)>(
                    CSF_AT(void**, states, 0)[0x1C / 4])(states, nullptr, value, &stateCode);
                offset += 12;
            }
        }
    }

    return result;
}

HOOK(0x004EE630, S_004e8e00_SetIndexedValue, "auto");
