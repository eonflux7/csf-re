#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));

// Reads a block of twelve fixed-size (52-byte) mission-script entries.
// Returns 1 when the block was fully consumed and the trailer check failed/was skipped,
// 0 when the trailer query on the stream reports success.
static int __stdcall MissionScript_ParseEntryBlock(int entryBase, S_00691860* reader)
{
    int headerProbe = 0xC;

    bool readerFlag = false;
    if (reader->pad_0000[0] == 0) {
        void* stream = reinterpret_cast<void*>(reader->field_0004);
        auto beginQuery = reinterpret_cast<uint8_t(__fastcall*)(void*, void*, int*, S_00691860*)>(
            CSF_AT(void**, stream, 0)[30]);
        readerFlag = !beginQuery(stream, nullptr, &headerProbe, reader);
    }
    reader->pad_0000[0] = readerFlag;

    int entryCursor = entryBase;
    unsigned int entryIndex = 0;
    while (true) {
        Archive_SerializeString(entryCursor, reader);
        if (reader->pad_0000[0] != 0) {
            break;
        }
        ++entryIndex;
        entryCursor += 0x34;
        if (entryIndex >= 0xC) {
            void* stream = reinterpret_cast<void*>(reader->field_0004);
            auto endQuery = reinterpret_cast<uint8_t(__fastcall*)(void*, void*, S_00691860*)>(
                CSF_AT(void**, stream, 0)[31]);
            if (endQuery(stream, nullptr, reader)) {
                reader->pad_0000[0] = 0;
                return 0;
            }
            break;
        }
    }

    reader->pad_0000[0] = 1;
    return 1;
}

HOOK(0x00468F50, MissionScript_ParseEntryBlock, "auto");
