#include <cstdint>
#include <cstring>

#include "globals.h"
#include "hook.h"

// Lookup table of 6-byte rows indexed by 6 * row. Four words per row.
CSF_GLOBAL(uint16_t, g_RowWordA_ECD8, 0x0085ECD8);
CSF_GLOBAL(uint16_t, g_RowWordB_ECDA, 0x0085ECDA);
CSF_GLOBAL(uint16_t, g_RowWordC_ECDC, 0x0085ECDC);
CSF_GLOBAL(uint16_t, g_RowWordD_ECDE, 0x0085ECDE);

static uint16_t ReadRowWord(const uint16_t& base_word, int byte_offset) {
    return *reinterpret_cast<const uint16_t*>(
        reinterpret_cast<const uint8_t*>(&base_word) + byte_offset);
}

// Resets the decode/output state of an inflate-style context and loads the
// per-row parameters selected by field +0x7C. Returns 2.
static int __fastcall ResetInflateStateAndLoadTableRow(void* /*ecx: unused*/, uint8_t* self) {
    int char_count = CSF_AT(int, self, 0x44);
    CSF_AT(int, self, 0x34) = 2 * CSF_AT(int, self, 0x24);

    uint16_t* buffer = reinterpret_cast<uint16_t*>(CSF_AT(uint8_t*, self, 0x3C));
    buffer[char_count - 1] = 0;
    std::memset(buffer, 0, static_cast<size_t>(2 * char_count - 2));

    int row_offset = 6 * CSF_AT(int, self, 0x7C);
    CSF_AT(int, self, 0x78) = ReadRowWord(g_RowWordB_ECDA, row_offset);
    CSF_AT(int, self, 0x84) = ReadRowWord(g_RowWordA_ECD8, row_offset);
    CSF_AT(int, self, 0x88) = ReadRowWord(g_RowWordC_ECDC, row_offset);
    CSF_AT(int, self, 0x74) = ReadRowWord(g_RowWordD_ECDE, row_offset);

    CSF_AT(int, self, 0x64) = 0;
    CSF_AT(int, self, 0x54) = 0;
    CSF_AT(int, self, 0x6C) = 0;
    CSF_AT(int, self, 0x60) = 0;
    CSF_AT(int, self, 0x40) = 0;
    CSF_AT(int, self, 0x70) = 2;
    CSF_AT(int, self, 0x58) = 2;
    return 2;
}

HOOK(0x6A08B0, ResetInflateStateAndLoadTableRow, "auto");
