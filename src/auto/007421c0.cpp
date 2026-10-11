#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NameMatchesCString, 0x0064A960, (void* self, void* edx, char* a1));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __stdcall, NetCogible_BaseDispatchField, 0x0065E6F0, (int a1, int a2, int a3, int a4));
CSF_FUNC(char, __fastcall, ReadRespawnMember, 0x0074B220, (void* self, void* edx, int a2, void** a3));
CSF_FUNC(char, __fastcall, ReadCogibleDescription, 0x00751580, (void* self, void* edx, int a3, int a4));

// Compares the field name held by `name` against a literal using a temporary string object.
static char MatchFieldName(void* name, const char* text)
{
    alignas(8) uint8_t tmpBytes[4];
    void* tmp = tmpBytes;
    InitializeTemporaryString(tmp, nullptr, reinterpret_cast<std::int32_t>(text));
    char result = NameMatchesString(name, nullptr, reinterpret_cast<void**>(tmp));
    DestroyTemporaryString(tmp, nullptr);
    return result;
}

static char __fastcall NetCogible_DispatchFieldByName(void* self, void* /*edx*/, char* obj, int nameHolder, int arg3, int arg4)
{
    void* name = reinterpret_cast<uint8_t*>(nameHolder) + 8;

    if (MatchFieldName(name, CSF_STR(0x008679C0, "m_dsCogibleDescription"))) {
        return ReadCogibleDescription(obj + 0x44, nullptr, arg3, arg4);
    }
    if (MatchFieldName(name, CSF_STR(0x008679AC, "m_dsRespawnActual"))) {
        return ReadRespawnMember(obj + 0x6c, nullptr, arg3, reinterpret_cast<void**>(arg4));
    }
    if (MatchFieldName(name, CSF_STR(0x00867998, "m_dsRespawnFlags"))) {
        return ReadRespawnMember(obj + 0x6f, nullptr, arg3, reinterpret_cast<void**>(arg4));
    }
    if (NameMatchesCString(name, nullptr, const_cast<char*>(CSF_STR(0x00867988, "m_dsRespawnTime")))) {
        return ReadRespawnMember(obj + 0x72, nullptr, arg3, reinterpret_cast<void**>(arg4));
    }
    return NetCogible_BaseDispatchField(reinterpret_cast<int>(obj), nameHolder, arg3, arg4);
}

HOOK(0x007421C0, NetCogible_DispatchFieldByName, "auto");
