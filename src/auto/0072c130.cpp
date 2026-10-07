#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "legacy/string.h"
#include "types/auto_classes.h"

CSF_FUNC(legacy::string*, __fastcall, AssignStringBuffer, 0x00429B60,
         (legacy::string* self, void* edx, char* text, unsigned int length));

static legacy::string* __fastcall CGuiPanel_SetStoredText(
    CGuiPanel* self, void* /*edx*/, char* text)
{
    if (text == nullptr)
        text = const_cast<char*>(CSF_STR(0x0082E65C, ""));

    char* cursor = text;
    char character;
    do {
        character = *cursor++;
    } while (character != '\0');

    const unsigned int length =
        static_cast<unsigned int>(cursor - (text + 1));

    return AssignStringBuffer(&self->m_str15C, nullptr, text, length);
}

HOOK(0x0072C130, CGuiPanel_SetStoredText, "auto");
