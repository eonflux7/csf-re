#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, MissionScript_FindDefinitionByName, 0x0046BE10, (void* self, void* edx, char* String1));
CSF_FUNC(unsigned int, __stdcall, MissionScript_FindDefinitionById, 0x0046BE40, (int a1));
CSF_FUNC(int, __fastcall, MissionScript_GetDefinition, 0x0046BE70, (void* self, void* edx, unsigned int a1));
CSF_FUNC(int, __fastcall, MissionScript_BeginActions, 0x0046C540, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MissionScript_BeginConditions, 0x0046C570, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MissionScript_BeginScript, 0x0046E170, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MissionScript_BeginVariables, 0x0046E1E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, MissionScript_RegisterNamedScript, 0x0046E410, (void* self, void* edx, char* String1, int a3));
CSF_FUNC(int, __fastcall, MissionScript_FindNamedScript, 0x0046E4E0, (void* self, void* edx, char* String1));
CSF_FUNC(uint32_t*, __fastcall, MissionScript_SetOpcode, 0x0046F2B0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, MissionScript_AppendScopeReference, 0x0046F370, (void* self, void* edx, int a2, int a3));
CSF_FUNC(uint8_t*, __fastcall, ReserveScratchEntries, 0x004725C0, (void* self, void* edx, int a2));
CSF_FUNC(void, __fastcall, MissionScript_CopyComponent, 0x004734F0, (void* self, void* edx, void* a2));
CSF_FUNC(int, __fastcall, MissionScript_GetComponentType, 0x006927F0, (void* self, void* edx));
CSF_FUNC(std::uint32_t, __fastcall, GetComponent, 0x00692BD0, (unsigned char* node, void* edx, unsigned int index));
CSF_FUNC(int, __fastcall, ParseIntegerComponent, 0x00692C40, (std::uint32_t component));
CSF_FUNC(char, __fastcall, CopyAttributeText, 0x00692D40, (std::uint8_t* attribute, void* edx, char* buffer, int capacity));

static int __fastcall S_0046c260_ParseScriptEntry(
    S_0046c260* self, void* /*edx*/, S_00692b80* node,
    std::uint32_t* entry, int outputAddress)
{
    char buffer[256] = {};
    void* output = reinterpret_cast<void*>(outputAddress);
    const std::uint32_t opcode = entry[1];

    if (entry[0])
    {
        int scopeIndex;
        switch (opcode)
        {
        case 3001:
        {
            auto component = GetComponent(
                reinterpret_cast<unsigned char*>(node), nullptr, 1u);
            component = GetComponent(
                reinterpret_cast<unsigned char*>(component), nullptr, 1u);
            CopyAttributeText(reinterpret_cast<std::uint8_t*>(component),
                              nullptr, buffer, 256);
            MissionScript_RegisterNamedScript(
                self, nullptr, buffer, CSF_AT(int, self, 0x00));
            MissionScript_SetOpcode(output, nullptr, 0);
            return reinterpret_cast<int>(ReserveScratchEntries(
                CSF_AT(void*, output, 0x04), nullptr, 0));
        }
        case 3003:
            scopeIndex = MissionScript_BeginScript(
                self, nullptr, CSF_AT(int, self, 0x00));
            MissionScript_SetOpcode(output, nullptr, entry[1]);
            break;
        case 3004:
        {
            scopeIndex = CSF_AT(
                int, reinterpret_cast<void*>(self->field_0024),
                4 * self->field_002c - 4);
            void* scopeData = CSF_AT(
                void*, reinterpret_cast<void*>(self->field_0044), 0x50);
            CSF_AT(int, scopeData, 8 * static_cast<std::uint32_t>(scopeIndex)) =
                CSF_AT(int, self, 0x00);
            MissionScript_SetOpcode(output, nullptr, entry[1]);
            break;
        }
        case 3005:
            MissionScript_SetOpcode(output, nullptr, 3005);
            return MissionScript_BeginActions(
                self, nullptr, CSF_AT(int, self, 0x00));
        case 3006:
        case 3013:
            scopeIndex = MissionScript_BeginVariables(
                self, nullptr, CSF_AT(int, self, 0x00));
            MissionScript_SetOpcode(output, nullptr, entry[1]);
            break;
        case 3007:
            scopeIndex = CSF_AT(
                int, reinterpret_cast<void*>(self->field_0034),
                4 * self->field_003c - 4);
            MissionScript_SetOpcode(output, nullptr, 3007);
            break;
        case 3008:
            scopeIndex = CSF_AT(
                int, reinterpret_cast<void*>(self->field_0034),
                4 * self->field_003c - 4);
            MissionScript_SetOpcode(output, nullptr, 3008);
            break;
        case 3009:
        case 3014:
            scopeIndex = MissionScript_BeginConditions(
                self, nullptr, CSF_AT(int, self, 0x00));
            MissionScript_SetOpcode(output, nullptr, entry[1]);
            break;
        default:
            return reinterpret_cast<int>(
                MissionScript_SetOpcode(output, nullptr, entry[1]));
        }

        return reinterpret_cast<int>(MissionScript_AppendScopeReference(
            reinterpret_cast<void*>(self->field_0044), nullptr,
            CSF_AT(int, self, 0x00), scopeIndex));
    }

    switch (opcode)
    {
    case 21:
    {
        auto component = GetComponent(
            reinterpret_cast<unsigned char*>(node), nullptr, 1u);
        CopyAttributeText(reinterpret_cast<std::uint8_t*>(component),
                          nullptr, buffer, 256);
        MissionScript_SetOpcode(output, nullptr, 0);
        ReserveScratchEntries(CSF_AT(void*, output, 0x04), nullptr, entry[2]);
        void* destination = CSF_AT(
            void*, CSF_AT(void*, output, 0x04), 0x04);
        int scriptIndex = MissionScript_FindNamedScript(self, nullptr, buffer);
        CSF_AT(int, destination, 0x04) = scriptIndex;
        return scriptIndex;
    }
    case 22:
    {
        auto component = GetComponent(
            reinterpret_cast<unsigned char*>(node), nullptr, 1u);
        CopyAttributeText(reinterpret_cast<std::uint8_t*>(component),
                          nullptr, buffer, 256);
        int scriptIndex =
            MissionScript_RegisterNamedScript(self, nullptr, buffer, -1);
        MissionScript_SetOpcode(output, nullptr, 0);
        auto result = ReserveScratchEntries(
            CSF_AT(void*, output, 0x04), nullptr, entry[2]);
        CSF_AT(int, CSF_AT(void*, CSF_AT(void*, output, 0x04), 0x04), 0x04) =
            scriptIndex;
        return reinterpret_cast<int>(result);
    }
    case 10:
    case 32:
    {
        auto component = GetComponent(
            reinterpret_cast<unsigned char*>(node), nullptr, 1u);
        unsigned int definitionIndex;
        if (MissionScript_GetComponentType(
                reinterpret_cast<void*>(component), nullptr) == 5)
        {
            component = GetComponent(
                reinterpret_cast<unsigned char*>(node), nullptr, 1u);
            CopyAttributeText(reinterpret_cast<std::uint8_t*>(component),
                              nullptr, buffer, 256);
            definitionIndex =
                MissionScript_FindDefinitionByName(self, nullptr, buffer);
        }
        else
        {
            component = GetComponent(
                reinterpret_cast<unsigned char*>(node), nullptr, 1u);
            int id = ParseIntegerComponent(component);
            definitionIndex = MissionScript_FindDefinitionById(id);
        }

        MissionScript_SetOpcode(output, nullptr, 0);
        void* scratch = CSF_AT(void*, output, 0x04);
        int definition =
            MissionScript_GetDefinition(self, nullptr, definitionIndex);
        ReserveScratchEntries(
            scratch, nullptr,
            CSF_AT(int, reinterpret_cast<void*>(definition), 0x04));
        void* result = CSF_AT(void*, output, 0x04);
        CSF_AT(unsigned int, result, 0x08) = definitionIndex;
        return reinterpret_cast<int>(result);
    }
    default:
        MissionScript_SetOpcode(output, nullptr, 0);
        ReserveScratchEntries(CSF_AT(void*, output, 0x04), nullptr, entry[2]);
        MissionScript_CopyComponent(
            CSF_AT(void*, output, 0x04), nullptr, node);
        return 0;
    }
}

HOOK(0x0046E590, S_0046c260_ParseScriptEntry, "auto");
