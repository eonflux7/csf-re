#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwStreamRead, 0x006293E0, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(int, __cdecl, RwStreamSkip, 0x00629700, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwStreamReadChunkHeaderInfo, 0x00629CE0, (uint32_t* a1, uint32_t* a2));

typedef void(__cdecl* AudioEffectEnumCallback)(uint32_t*);

// Opens an RW stream from the given file name, walks its chunks and invokes the
// callback for every audio-effect (0x802) chunk with a block {name, field, field, flag, index}.
static uint8_t* __stdcall AudioEffects_EnumerateDictionary(uint8_t* fileName, AudioEffectEnumCallback callback)
{
    if (!fileName)
        return fileName;
    if (*fileName == 0)
        return fileName;
    if (!callback)
        return fileName;

    uint32_t* stream = RwStreamOpen(2, 1, (int)(intptr_t)fileName);
    if (!stream)
        return nullptr;

    // entry[4] is the running effect index, shared with the callback's view of the block.
    uint32_t entry[5];
    entry[4] = 0;

    uint32_t chunkInfo[5];
    if (RwStreamReadChunkHeaderInfo(stream, chunkInfo))
    {
        do
        {
            if ((int)chunkInfo[0] == 2050)
            {
                char name[100];
                uint8_t header[28];

                memset(name, 0, sizeof(name));
                RwStreamSkip((int)(intptr_t)stream, 16);
                RwStreamRead(stream, header, 28);
                RwStreamSkip((int)(intptr_t)stream, 80);

                int idx = 0;
                do
                {
                    RwStreamRead(stream, &name[idx], 1);
                } while (name[idx++]);
                name[idx] = 0;

                RwStreamSkip((int)(intptr_t)stream, (int)chunkInfo[1] - (int)strlen(name) - 125);

                entry[0] = (uint32_t)(uintptr_t)name;
                entry[1] = CSF_AT(uint32_t, header, 8);
                entry[2] = CSF_AT(uint32_t, header, 0);
                entry[3] = CSF_AT(uint8_t, header, 0x0D);
                callback(entry);
                entry[4] = entry[4] + 1;
            }
            else
            {
                RwStreamSkip((int)(intptr_t)stream, (int)chunkInfo[1]);
            }
        } while (RwStreamReadChunkHeaderInfo(stream, chunkInfo));
    }
    return (uint8_t*)(intptr_t)RwStreamClose(stream, nullptr);
}

HOOK(0x0070FC30, AudioEffects_EnumerateDictionary, "auto");
