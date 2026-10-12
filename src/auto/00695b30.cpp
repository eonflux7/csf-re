#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, InsertWideCharacters, 0x006959F0, (void* self, void* edx, unsigned int a2, unsigned int a3, int16_t a4));

// __thiscall InsertWideCharacter(S_004f21d0* this, uint32_t* outPtr, int charAddress, int16_t ch), ret 12
static uint32_t* __fastcall S_004f21d0_InsertWideCharacter(S_004f21d0* self, void* /*edx*/, uint32_t* outPtr, int charAddress, int16_t ch)
{
    // Inline (small) string buffer lives at +4; heap buffer pointer is stored there when capacity >= 8.
    uint16_t* bufferStart = reinterpret_cast<uint16_t*>(&self->field_0004);
    uint16_t* dataBase;
    if (self->field_0018 < 8)
        dataBase = bufferStart;
    else
        dataBase = *reinterpret_cast<uint16_t**>(bufferStart);

    unsigned int insertIndex;
    if (charAddress)
        insertIndex = static_cast<unsigned int>((charAddress - static_cast<int>(reinterpret_cast<uintptr_t>(dataBase))) >> 1);
    else
        insertIndex = 0;

    InsertWideCharacters(self, nullptr, insertIndex, 1u, ch);

    if (self->field_0018 >= 8)
        bufferStart = *reinterpret_cast<uint16_t**>(bufferStart);

    *outPtr = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(&bufferStart[insertIndex]));
    return outPtr;
}

HOOK(0x695B30, S_004f21d0_InsertWideCharacter, "auto");
