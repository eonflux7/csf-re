#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, MissionScript_ResizeActionIndices, 0x0046D800,
         (void* self, void* edx, unsigned int a2, int a4));
CSF_FUNC(int*, __fastcall, MissionScript_ResizeEvents, 0x0046D890,
         (void* self, void* edx, unsigned int a2, int a3, int a4));

// The callee takes the address of this complete packed stack argument.
CSF_FUNC(int*, __fastcall, MissionScript_ResizeEntries, 0x0046D930,
         (S_0046c710* self, void* edx, unsigned int count,
          std::int32_t entry, void* owner));
CSF_FUNC(int, __cdecl, MissionScript_ResetParseState, 0x0046E120, (int* a1));
CSF_FUNC(int, __fastcall, MissionScript_ParseEntry, 0x0046E910,
         (void* self, void* edx, void* a2, int a3));
CSF_FUNC(unsigned int, __fastcall, MissionScript_ParseEvent, 0x0046EA00,
         (void* self, void* edx, void* a2, int* a3));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(std::uint32_t, __fastcall, MenuExtras_GetChildCount, 0x00692B80,
         (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0,
         (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40,
         (std::uint32_t component));
CSF_FUNC(std::uint8_t*, __fastcall, FindProperty, 0x006932B0,
         (std::uint8_t* self, void* edx, char* keyword));
CSF_FUNC(int, __fastcall, ReadIntegerProperty, 0x00693910,
         (std::uint8_t* self, void* edx, char* name));
CSF_FUNC(char, __fastcall, MenuExtras_ReadText, 0x006939C0,
         (std::uint8_t* self, void* edx, char* keyword, char* buffer, int capacity));
CSF_FUNC(char, __fastcall, MissionScript_HasPendingState, 0x0069AF10,
         (void* self, void* edx));
CSF_FUNC(int*, __fastcall, MissionScript_ConstructVariables, 0x00770440,
         (void* self, void* edx, int a2, void* a3));

static bool __fastcall S_0046c260_ParseScript(
    S_0046c260* self, void* /*edx*/, int /*a2*/, S_00692b80* source, int destination)
{
    if (!source)
        return false;

    char nameBuffer[0x10C];
    std::memset(nameBuffer, 0, 256);

    void* pendingState = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x4C));
    if (pendingState && MissionScript_HasPendingState(pendingState, nullptr))
    {
        pendingState = reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x4C));
        reinterpret_cast<void(__fastcall*)(void*, void*)>(
            CSF_AT(void**, pendingState, 0)[1])(pendingState, nullptr);
    }

    self->field_0044 = static_cast<std::uint32_t>(destination);
    MissionScript_ResetParseState(
        reinterpret_cast<int*>(reinterpret_cast<std::uint8_t*>(self) + 4));

    auto* node = reinterpret_cast<std::uint8_t*>(source);
    bool valid = false;
    auto* flags = FindProperty(
        node, nullptr, const_cast<char*>(CSF_STR(0x008315D4, ".FLAGS")));
    if (flags)
    {
        int trigger = ReadIntegerProperty(
            flags, nullptr, const_cast<char*>(CSF_STR(0x008315C8, ".TRIGGER")));
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_0044), 0) =
            trigger == 1;

        int enabled = ReadIntegerProperty(
            flags, nullptr, const_cast<char*>(CSF_STR(0x008315BC, ".ENABLED")));
        CSF_AT(std::uint8_t, reinterpret_cast<void*>(self->field_0044), 1) =
            enabled == 1;

        valid = ReadIntegerProperty(
            flags, nullptr, const_cast<char*>(CSF_STR(0x008315B4, ".VALIDO"))) == 1;
    }

    auto* integerProperty = FindProperty(
        node, nullptr, reinterpret_cast<char*>(CSF_ADDR_DATA(0x008305F4)));
    if (integerProperty)
    {
        int value = ParseIntegerComponent(
            reinterpret_cast<std::uint32_t>(integerProperty));
        CSF_AT(int, reinterpret_cast<void*>(self->field_0044), 4) = value;
    }

    MenuExtras_ReadText(
        node, nullptr, const_cast<char*>(CSF_STR(0x00830F8C, ".NOMBRE")),
        nameBuffer, 256);
    if (!valid)
    {
        MenuExtras_ReadText(
            node, nullptr, const_cast<char*>(CSF_STR(0x00830F8C, ".NOMBRE")),
            nameBuffer, 256);
        return valid;
    }

    auto* variablesProperty = FindProperty(
        node, nullptr, const_cast<char*>(CSF_STR(0x00831574, ".VARIABLES")));
    void* variables = Mem_Alloc(20);
    if (variables)
        MissionScript_ConstructVariables(variables, nullptr, 1, variablesProperty);

    CSF_AT(std::uint32_t, reinterpret_cast<void*>(self->field_0044), 40) =
        reinterpret_cast<std::uint32_t>(variables);

    // The packet also lists this direct callee address as a vtable reference.
    (void)CSF_VTBL(0x00692B80);

    auto* events = FindProperty(
        node, nullptr, const_cast<char*>(CSF_STR(0x008315A8, ".EVENTOS")));
    if (events)
    {
        unsigned int count = MenuExtras_GetChildCount(events, nullptr);
        if (count)
        {
            MissionScript_ResizeEvents(
                reinterpret_cast<std::uint8_t*>(
                    static_cast<std::uint32_t>(destination)) + 8,
                nullptr, count, -1, 0);

            for (unsigned int index = 0; index < count; ++index)
            {
                std::uint32_t component = GetComponent(events, nullptr, index);
                std::uint32_t entries = CSF_AT(
                    std::uint32_t, reinterpret_cast<void*>(self->field_0044), 12);
                MissionScript_ParseEvent(
                    self, nullptr, reinterpret_cast<void*>(component),
                    reinterpret_cast<int*>(entries + 8 * index));
            }
        }
    }

    auto* conditions = FindProperty(
        node, nullptr, const_cast<char*>(CSF_STR(0x00831598, ".CONDICIONES")));
    if (conditions)
    {
        unsigned int count = MenuExtras_GetChildCount(conditions, nullptr);
        if (count)
        {
            std::uint16_t entryParts[2] = {0, static_cast<std::uint16_t>(-1)};
            std::int32_t emptyEntry;
            std::memcpy(&emptyEntry, entryParts, sizeof(emptyEntry));

            MissionScript_ResizeEntries(
                reinterpret_cast<S_0046c710*>(
                    reinterpret_cast<std::uint8_t*>(self->field_0044) + 24),
                nullptr, count, emptyEntry, nullptr);

            for (unsigned int index = 0; index < count; ++index)
            {
                std::uint32_t component = GetComponent(conditions, nullptr, index);
                std::uint32_t entries = CSF_AT(
                    std::uint32_t, reinterpret_cast<void*>(self->field_0044), 28);
                MissionScript_ParseEntry(
                    self, nullptr, reinterpret_cast<void*>(component),
                    static_cast<int>(entries + 8 * index));
            }
        }
    }

    auto* actions = FindProperty(
        node, nullptr, const_cast<char*>(CSF_STR(0x0083158C, ".ACCIONES")));
    if (!actions)
        return valid;

    unsigned int actionCount = MenuExtras_GetChildCount(actions, nullptr);
    if (!actionCount)
        return valid;

    MissionScript_ResizeActionIndices(
        reinterpret_cast<std::uint8_t*>(self->field_0044) + 112,
        nullptr, actionCount, 0);

    std::uint16_t entryParts[2] = {0, static_cast<std::uint16_t>(-1)};
    std::int32_t emptyEntry;
    std::memcpy(&emptyEntry, entryParts, sizeof(emptyEntry));

    MissionScript_ResizeEntries(
        reinterpret_cast<S_0046c710*>(
            reinterpret_cast<std::uint8_t*>(self->field_0044) + 44),
        nullptr, actionCount, emptyEntry, nullptr);

    CSF_AT(std::uint32_t, self, 0) = 0;
    std::uint32_t nextIndex;
    do
    {
        std::uint32_t component = GetComponent(
            actions, nullptr, CSF_AT(std::uint32_t, self, 0));
        std::uint32_t entries = CSF_AT(
            std::uint32_t, reinterpret_cast<void*>(self->field_0044), 48);
        MissionScript_ParseEntry(
            self, nullptr, reinterpret_cast<void*>(component),
            static_cast<int>(entries + 8 * CSF_AT(std::uint32_t, self, 0)));

        nextIndex = CSF_AT(std::uint32_t, self, 0) + 1;
        CSF_AT(std::uint32_t, self, 0) = nextIndex;
    } while (nextIndex < actionCount);

    return valid;
}

HOOK(0x0046EAD0, S_0046c260_ParseScript, "auto");
