#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, sub_799AA0, 0x00799AA0, (void* a1, int a2));
CSF_FUNC(void*, __fastcall, WriteConfigFlag, 0x00656380, (void* self, void* edx, char a2));
CSF_FUNC(void*, __fastcall, GameConfig_WriteByteField, 0x006563B0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, GameConfig_WriteDwordField, 0x006563F0, (void* self, void* edx, char a2));

static int __cdecl NetGatheringDdl_WriteConfigSection(C_008622e8* writer, int settingsBlock) {
    uint8_t* src = reinterpret_cast<uint8_t*>(settingsBlock);
    void* out = reinterpret_cast<void*>(writer);

    sub_799AA0(writer, settingsBlock);

    GameConfig_WriteByteField(out, nullptr, CSF_AT(uint8_t, src, 0x30));
    GameConfig_WriteByteField(out, nullptr, CSF_AT(uint8_t, src, 0x31));
    WriteConfigFlag(out, nullptr, CSF_AT(uint8_t, src, 0x32));
    WriteConfigFlag(out, nullptr, CSF_AT(uint8_t, src, 0x33));

    for (unsigned int i = 0; i < 0x20; ++i) {
        GameConfig_WriteByteField(out, nullptr, CSF_AT(uint8_t, src, 0x34 + i));
    }

    for (unsigned int j = 0; j < 0x200; ++j) {
        GameConfig_WriteByteField(out, nullptr, CSF_AT(uint8_t, src, 0x54 + j));
    }

    return GameConfig_WriteDwordField(out, nullptr, CSF_AT(uint32_t, src, 0x254));
}

HOOK(0x006AB250, NetGatheringDdl_WriteConfigSection, "auto");
