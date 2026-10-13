#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(uint32_t*, __cdecl, RwStreamOpen, 0x006298E0, (int a3, int a4, int a5));
CSF_FUNC(int, __cdecl, RwTexDictionaryGetCurrent, 0x006319B0, ());
CSF_FUNC(int, __cdecl, RwTexDictionaryStreamWrite, 0x006308E0, (int a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RwStreamClose, 0x00629810, (uint32_t* a1, uint32_t* a2));

// Writes the current texture dictionary to the stream opened on a1 (a path/name).
// Returns true only when a stream was opened and the dictionary write succeeded.
static bool __stdcall TexDict_SaveCurrentDictionaryToFile(int a1)
{
    bool wrote = false;
    if (a1 == 0)
        return false;

    uint32_t* stream = RwStreamOpen(2, 2, a1);
    if (stream)
    {
        int currentDictionary = RwTexDictionaryGetCurrent();
        wrote = RwTexDictionaryStreamWrite(currentDictionary, stream) != 0;
        RwStreamClose(stream, nullptr);
    }
    return wrote;
}

HOOK(0x6C1060, TexDict_SaveCurrentDictionaryToFile, "auto");
