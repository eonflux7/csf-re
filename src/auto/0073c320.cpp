#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(std::uint32_t*, __fastcall, MenuProgressInput_AssignText, 0x00696F90, (std::uint32_t* self, void* edx, std::int16_t* text));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (void* self, void* edx, uint8_t* arg));

static std::uint32_t* __fastcall NetSession_SetTextInputOrDefault(
    void* self, void* /*edx*/, std::int16_t* text)
{
    auto* input = static_cast<std::uint8_t*>(self) + 0x0C;

    if (text && *text) {
        return MenuProgressInput_AssignText(
            reinterpret_cast<std::uint32_t*>(input), nullptr, text);
    }

    return static_cast<std::uint32_t*>(TextInputSubobject_Init(
        input, nullptr,
        reinterpret_cast<uint8_t*>(
            const_cast<char*>(CSF_STR(0x0086760C, "DEFAULT")))));
}

HOOK(0x0073C320, NetSession_SetTextInputOrDefault, "auto");
