#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwStreamSkip, 0x00629700, (int a1, int a2));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwStreamReadChunkHeaderInfo, 0x00629CE0, (uint32_t* a1, uint32_t* a2));
CSF_FUNC(int, __stdcall, AudioEffects_MergeMissingDictEntries, 0x0070F500, (int a1, int a2));
CSF_FUNC(uint32_t*, __cdecl, AudioEffects_CreateDictionary, 0x007DBBA0, (int a1));
CSF_FUNC(int, __cdecl, AudioEffects_DestroyDictionary, 0x007DBC20, (int a1));
CSF_FUNC(int, __cdecl, RtDictGetNumEntries, 0x007DBD80, (int a1));
CSF_FUNC(int, __cdecl, RpMTEffectDictSetCurrent, 0x007DBE80, (int a1, int a2));
CSF_FUNC(int, __cdecl, AudioEffects_GetCurrentDictionary, 0x007DBE90, ());
CSF_FUNC(int, __cdecl, AudioEffects_ReadDictionary, 0x007E0540, (uint32_t* a1, int arg4, int a3, int a4, unsigned int a5));

static char __fastcall C_00863ba8_LoadEffectsDictionary(
    C_00863ba8* self, void* /*edx*/, uint8_t* filename, char preserveExisting)
{
    if (!filename || !*filename)
        return 0;

    alignas(8) uint8_t chunkBytes[0x18];
    int& savedDictionary = CSF_AT(int, chunkBytes, 0x14);
    savedDictionary = 0;

    int previousDictionary = 0;
    char success = 0;

    if (preserveExisting)
    {
        savedDictionary = AudioEffects_GetCurrentDictionary();
        uint32_t* dictionary = AudioEffects_CreateDictionary(0);
        RpMTEffectDictSetCurrent(
            static_cast<int>(reinterpret_cast<uintptr_t>(dictionary)), 0);
        previousDictionary = savedDictionary;
    }
    else
    {
        if (AudioEffects_GetCurrentDictionary())
        {
            int dictionary = AudioEffects_GetCurrentDictionary();
            AudioEffects_DestroyDictionary(dictionary);
        }

        uint32_t* dictionary = AudioEffects_CreateDictionary(0);
        RpMTEffectDictSetCurrent(
            static_cast<int>(reinterpret_cast<uintptr_t>(dictionary)), 0);
    }

    uint32_t* stream = RwStreamOpen(
        2, 1, static_cast<int>(reinterpret_cast<uintptr_t>(filename)));

    if (stream)
    {
        int failures = 0;
        success = 1;

        do
        {
            if (!RwStreamReadChunkHeaderInfo(
                    stream, reinterpret_cast<uint32_t*>(chunkBytes)))
                break;

            if (CSF_AT(int, chunkBytes, 0) == 2050)
            {
                bool accepted =
                    ((bool(__fastcall*)(void*, void*, int))
                        self->vtbl[0xCC / 4])(
                            self, nullptr, CSF_AT(int, chunkBytes, 0x4));

                if (!accepted ||
                    !AudioEffects_ReadDictionary(
                        stream, 0, static_cast<int>(self->field_0004), 0, 0))
                {
                    ++failures;
                }
            }
            else
            {
                RwStreamSkip(
                    static_cast<int>(reinterpret_cast<uintptr_t>(stream)),
                    CSF_AT(int, chunkBytes, 0x4));
            }
        } while (!failures);

        RwStreamClose(stream, nullptr);
        previousDictionary = savedDictionary;

        if (failures > 0)
        {
            self->field_0031 = 1;
            success = 0;
        }
    }

    self->field_0030 = 1;

    if (previousDictionary)
    {
        if (static_cast<uint32_t>(RtDictGetNumEntries(previousDictionary)) > 0)
        {
            int currentDictionary = AudioEffects_GetCurrentDictionary();
            AudioEffects_MergeMissingDictEntries(
                previousDictionary, currentDictionary);

            currentDictionary = AudioEffects_GetCurrentDictionary();
            AudioEffects_DestroyDictionary(currentDictionary);

            // The destroy argument remains on the original caller's stack.
            RpMTEffectDictSetCurrent(previousDictionary, currentDictionary);
            return success;
        }

        AudioEffects_DestroyDictionary(previousDictionary);
    }

    return success;
}

HOOK(0x00710BA0, C_00863ba8_LoadEffectsDictionary, "auto");
