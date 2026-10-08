#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Format-argument frame laid out in the same memory order as the original stack
// frame: 0x1c-byte formatter object, then the two counters, the number-format mode,
// and the saved caller esi.
struct CountTextFrame {
    uint8_t formatter[0x1C];
    uint32_t countAt2C;    // pushed as edx = [a1+0x2C]
    uint32_t countAt30;    // pushed as ecx = [a1+0x30]
    int32_t numberMode;    // g_NumberFormattingMode
    uint32_t savedEsi;     // caller's esi (output string pointer)
};

CSF_GLOBAL(int, g_NumberFormattingMode, 0x0085E684);

// __thiscall int FormatterSetPattern(this, const char* pattern)
CSF_FUNC(int, __fastcall, FormatterSetPattern, 0x00697100, (void* self, void* edx, const char* pattern));

// __cdecl formatter producing the result string from the frame
CSF_FUNC(void*, __cdecl, FormatCountTextBuffer, 0x00696A80, (void* outString));

// __thiscall string assignment: (this=dest, src, pos=0, count=-1)
CSF_FUNC(void*, __fastcall, StringAssignRange, 0x00488830, (void* self, void* edx, void* src, uint32_t pos, uint32_t count));

// Builds "{d0}/{d1}" from two counters of the info object into outString.
static void* __stdcall FormatCountText(int infoObject, void* outString)
{
    uint8_t* info = reinterpret_cast<uint8_t*>(infoObject);

    CountTextFrame frame;
    frame.numberMode = g_NumberFormattingMode;
    frame.countAt30 = CSF_AT(uint32_t, info, 0x30);
    frame.countAt2C = CSF_AT(uint32_t, info, 0x2C);
    frame.savedEsi = reinterpret_cast<uint32_t>(outString);

    FormatterSetPattern(frame.formatter, nullptr, CSF_STR(0x00841B30, "{d0}/{d1}"));

    void* formattedText = FormatCountTextBuffer(outString);
    StringAssignRange(outString, nullptr, formattedText, 0, 0xFFFFFFFFu);

    return outString;
}

HOOK(0x00530CD0, FormatCountText, "auto");
