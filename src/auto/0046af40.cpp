#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, SceneLoader_SerializeRecordMember, 0x00445620, (void* a1, int a2));
CSF_FUNC(char, __cdecl, MissionScript_SerializeEventList, 0x00468500, (void* a3, bool* a2));
CSF_FUNC(int, __stdcall, MissionScript_SerializeScriptList, 0x0046A440, (int a1, int a2));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));
CSF_FUNC(char, __cdecl, Archive_SerializeObjectRef, 0x004ADF10, (void* a3, bool* a2));
CSF_FUNC(unsigned int, __stdcall, Scene_SerializeNameList, 0x004E8060, (void* a1, int a2));

static int __fastcall MissionScript_SerializeMembers(
    void* self, void* /*edx*/, void* archive)
{
    auto* members = static_cast<std::uint8_t*>(self);

    bool failed = false;
    if (CSF_AT(std::uint8_t, archive, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, void*))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, archive);
    }

    const std::uint32_t savedContext =
        CSF_AT(std::uint32_t, archive, 0x1C);
    CSF_AT(std::uint8_t, archive, 0x00) = failed;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;

    if (!failed)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Scene_SerializeNameList(members + 0x24, reinterpret_cast<int>(archive));
    }

    bool ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        MissionScript_SerializeScriptList(
            reinterpret_cast<int>(members + 0x60),
            reinterpret_cast<int>(archive));
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        MissionScript_SerializeScriptList(
            reinterpret_cast<int>(members + 0x70),
            reinterpret_cast<int>(archive));
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        SceneLoader_SerializeRecordMember(
            members + 0x80, reinterpret_cast<int>(archive));
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 1;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeObjectRef(
            members + 0x34, static_cast<bool*>(archive));
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 1;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        MissionScript_SerializeEventList(
            members + 0xA4, static_cast<bool*>(archive));
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, archive, 0x15) = 0;
        CSF_AT(std::uint8_t, archive, 0x16) = 0;
        Archive_SerializeString(
            reinterpret_cast<int>(members + 0x90), archive);
    }

    ready = CSF_AT(std::uint8_t, archive, 0x00) == 0;
    CSF_AT(std::uint32_t, archive, 0x1C) = savedContext;
    if (ready)
    {
        void* serializer = CSF_AT(void*, archive, 0x04);
        // Original tests only al (test al, al) but returns the whole eax.
        const int result = ((int(__fastcall*)(void*, void*, void*))
            CSF_AT(void**, serializer, 0)[0x7C / 4])(
                serializer, nullptr, archive);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, archive, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x0046AF40, MissionScript_SerializeMembers, "auto");
