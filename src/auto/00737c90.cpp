#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00867260_PrepareQuad, 0x00737260, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00867260_CommitQuad, 0x007377F0, (void* self, void* edx));

static char __fastcall C_00867260_UpdateQuadVertexData(
    C_00867260* self, void* /*edx*/)
{
    const char dirty = CSF_AT(char, self, 0x20);
    if (!dirty)
        return dirty;

    CSF_AT(char, self, 0x20) = 0;
    C_00867260_PrepareQuad(self, nullptr);

    const uint32_t firstCoordinate = CSF_AT(uint32_t, self, 0x44);
    const uint32_t secondCoordinate = CSF_AT(uint32_t, self, 0x48);
    CSF_AT(uint32_t, self, 0x4C) = firstCoordinate;
    CSF_AT(uint32_t, self, 0x50) = secondCoordinate;

    for (int offset = 0; offset < 112; offset += 28)
    {
        CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x14), offset + 8) = 0;
        CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x14), offset + 12) =
            1065353216;

        const uint8_t alpha =
            static_cast<uint8_t>(static_cast<int64_t>(CSF_AT(float, self, 0x74)));
        const uint8_t red =
            static_cast<uint8_t>(static_cast<int64_t>(CSF_AT(float, self, 0x68)));
        const uint8_t green =
            static_cast<uint8_t>(static_cast<int64_t>(CSF_AT(float, self, 0x6C)));
        const uint8_t blue =
            static_cast<uint8_t>(static_cast<int64_t>(CSF_AT(float, self, 0x70)));

        CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x14), offset + 16) =
            (static_cast<uint32_t>(alpha) << 24) |
            (static_cast<uint32_t>(red) << 16) |
            (static_cast<uint32_t>(green) << 8) |
            static_cast<uint32_t>(blue);
    }

    CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x14), 20) =
        CSF_AT(uint32_t, self, 0x58);
    CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x14), 24) =
        CSF_AT(uint32_t, self, 0x5C);
    CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x14), 48) =
        CSF_AT(uint32_t, self, 0x58);
    CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x14), 52) =
        CSF_AT(uint32_t, self, 0x64);
    CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x14), 76) =
        CSF_AT(uint32_t, self, 0x60);
    CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x14), 80) =
        CSF_AT(uint32_t, self, 0x5C);
    CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x14), 104) =
        CSF_AT(uint32_t, self, 0x60);
    CSF_AT(uint32_t, CSF_AT(uint8_t*, self, 0x14), 108) =
        CSF_AT(uint32_t, self, 0x64);

    return static_cast<char>(C_00867260_CommitQuad(self, nullptr));
}

HOOK(0x00737C90, C_00867260_UpdateQuadVertexData, "auto");
