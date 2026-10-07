#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __cdecl, ConvertByteTextToWide, 0x00695590,
         (uint8_t* source, uint16_t* destination, int limit));

static int __fastcall CGuiListBox_ForwardConvertedText(
    CGuiListBox* self, void* /*edx*/,
    int firstArgument, int secondArgument, uint8_t* text, int lastArgument)
{
    uint16_t wideText[1000];
    ConvertByteTextToWide(text, wideText, 999);

    return reinterpret_cast<
        int (__fastcall*)(CGuiListBox*, void*, int, int, uint16_t*, int)
    >(CSF_AT(void**, self, 0)[0x294 / 4])(
        self, nullptr, firstArgument, secondArgument, wideText, lastArgument);
}

HOOK(0x00722D20, CGuiListBox_ForwardConvertedText, "auto");
