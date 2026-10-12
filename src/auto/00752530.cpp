#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0, (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0, (void* self, void* edx));
CSF_FUNC(char*, __cdecl, FormatValueString, 0x0064A8F0, (void** a1, char* Format, ...));
CSF_FUNC(char, __fastcall, NameMatchesCString, 0x0064A960, (void* self, void* edx, char* a1));
CSF_FUNC(char, __fastcall, NameMatchesString, 0x0064A9B0, (void* self, void* edx, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatUnsignedValue, 0x0066C010, (uint32_t* a1, void** a2));
CSF_FUNC(char, __cdecl, NetSpawnMessages_FormatFloatValue, 0x0068E670, (float* a1, void** a2));

static char __fastcall NetSpawnMessages_FormatNetworkParameter(
    void* self, void* /*edx*/, void* nameOwner, void** output)
{
    auto* parameters = static_cast<std::uint8_t*>(self);
    void* name = static_cast<std::uint8_t*>(nameOwner) + 8;

    // The original reuses its first argument slot for this one-word string.
    void* temporaryString = nameOwner;
    InitializeTemporaryString(
        &temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(CSF_STR(0x00868D20, "bHuboCambios")));
    char matches = NameMatchesString(name, nullptr, &temporaryString);
    DestroyTemporaryString(&temporaryString, nullptr);
    if (matches)
    {
        FormatValueString(
            output, const_cast<char*>(CSF_STR(0x0083090C, "%d")),
            static_cast<int>(CSF_AT(std::uint8_t, self, 0)));
        return 1;
    }

    InitializeTemporaryString(
        &temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x00868D04, "fParRed_ConstanteErrorDir")));
    matches = NameMatchesString(name, nullptr, &temporaryString);
    DestroyTemporaryString(&temporaryString, nullptr);
    if (matches)
    {
        FormatValueString(
            output, const_cast<char*>(CSF_STR(0x00840C3C, "%f")),
            static_cast<double>(CSF_AT(float, self, 4)));
        return 1;
    }

    InitializeTemporaryString(
        &temporaryString, nullptr,
        reinterpret_cast<std::int32_t>(
            CSF_STR(0x00868CE8, "fParRed_ConstanteErrorPos")));
    matches = NameMatchesString(name, nullptr, &temporaryString);
    DestroyTemporaryString(&temporaryString, nullptr);
    if (matches)
    {
        FormatValueString(
            output, const_cast<char*>(CSF_STR(0x00840C3C, "%f")),
            static_cast<double>(CSF_AT(float, self, 8)));
        return 1;
    }

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868CC8, "fParRed_CuadraticoDistanciaDir"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 12), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868CA8, "fParRed_CuadraticoDistanciaPos"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 16), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868C8C, "fParRed_CuadraticoMaximoDir"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 20), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868C70, "fParRed_CuadraticoMaximoPos"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 24), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868C54, "fParRed_CuadraticoMinimoDir"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 28), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868C38, "fParRed_CuadraticoMinimoPos"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 32), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868C1C, "fParRed_LinealDistanciaDir"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 36), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868C00, "fParRed_LinealDistanciaPos"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 40), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868BE8, "fParRed_LinealMaximoDir"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 44), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868BD0, "fParRed_LinealMaximoPos"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 48), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868BB8, "fParRed_LinealMinimoDir"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 52), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868BA0, "fParRed_LinealMinimoPos"))))
        return NetSpawnMessages_FormatFloatValue(
            reinterpret_cast<float*>(parameters + 56), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868B80, "uParRed_FrecuenciadoActivoDir"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<uint32_t*>(parameters + 60), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868B60, "uParRed_FrecuenciadoActivoPos"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<uint32_t*>(parameters + 64), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868B3C, "uParRed_FrecuenciadoSeleccionadoDir"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<uint32_t*>(parameters + 68), output);

    if (NameMatchesCString(name, nullptr, const_cast<char*>(
            CSF_STR(0x00868B18, "uParRed_FrecuenciadoSeleccionadoPos"))))
        return NetSpawnMessages_FormatUnsignedValue(
            reinterpret_cast<uint32_t*>(parameters + 72), output);

    return 0;
}

HOOK(0x00752530, NetSpawnMessages_FormatNetworkParameter, "auto");
