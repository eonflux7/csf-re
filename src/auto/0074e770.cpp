#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __stdcall, NetCogible_BaseDispatchField, 0x0065E6F0, (int a1, int a2, int a3, int a4));
CSF_FUNC(char, __fastcall, NetSpawnMessages_FormatNetworkParameter_752530, 0x00752530, (void* self, void* , void* nameOwner, void** output));
CSF_FUNC(char, __fastcall, NetSpawnMessages_FormatNetworkParameter_752990, 0x00752990, (void* self, void* , void* nameOwner, void** output));
CSF_FUNC(char, __fastcall, NetSpawnMessages_FormatNetworkParameter_752C50, 0x00752C50, (void* self, void* , void* nameObject, void** output));

static char __stdcall NetControlDdl_DispatchNetworkParameter(
    int object, int fieldDescriptor, int nameOwner, int output)
{
    alignas(8) std::uint8_t temporaryStringBytes[0x4];
    auto* temporaryString = reinterpret_cast<void**>(temporaryStringBytes);
    auto* fieldName = reinterpret_cast<std::uint8_t*>(fieldDescriptor) + 8;

    // The initializer returns its storage address, despite its void binding.
    InitializeTemporaryString(
        temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x00868574, "m_dsParEmpaquetado")));
    char matches = NameMatchesString(fieldName, nullptr, temporaryString);
    DestroyTemporaryString(temporaryString, nullptr);
    if (matches)
    {
        return NetSpawnMessages_FormatNetworkParameter_752C50(
            reinterpret_cast<std::uint8_t*>(object) + 0x44, nullptr,
            reinterpret_cast<void*>(nameOwner),
            reinterpret_cast<void**>(output));
    }

    InitializeTemporaryString(
        temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x00868560, "m_dsParFrecuenciado")));
    matches = NameMatchesString(fieldName, nullptr, temporaryString);
    DestroyTemporaryString(temporaryString, nullptr);
    if (matches)
    {
        return NetSpawnMessages_FormatNetworkParameter_752530(
            reinterpret_cast<std::uint8_t*>(object) + 0x58, nullptr,
            reinterpret_cast<void*>(nameOwner),
            reinterpret_cast<void**>(output));
    }

    InitializeTemporaryString(
        temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x00868550, "m_dsParSimulado")));
    matches = NameMatchesString(fieldName, nullptr, temporaryString);
    DestroyTemporaryString(temporaryString, nullptr);
    if (matches)
    {
        return NetSpawnMessages_FormatNetworkParameter_752990(
            reinterpret_cast<std::uint8_t*>(object) + 0xA8, nullptr,
            reinterpret_cast<void*>(nameOwner),
            reinterpret_cast<void**>(output));
    }

    return NetCogible_BaseDispatchField(object, fieldDescriptor, nameOwner, output);
}

HOOK(0x0074E770, NetControlDdl_DispatchNetworkParameter, "auto");
