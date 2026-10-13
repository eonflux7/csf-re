#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall S_00427bf0_UpdateHighlightRect(S_00427bf0* self, void* /*edx*/)
{
    int rect[4] = {0, 0, 0, 0};

    void* originProvider = reinterpret_cast<void*>(self->field_0018);
    int* originPair = ((int* (__fastcall*)(void*, void*))CSF_AT(void**, originProvider, 0)[8])(originProvider, nullptr);
    rect[0] = originPair[0];
    rect[1] = originPair[1];

    void* sizeProvider = reinterpret_cast<void*>(self->field_0018);
    int* sizePair = ((int* (__fastcall*)(void*, void*))CSF_AT(void**, sizeProvider, 0)[10])(sizeProvider, nullptr);
    rect[2] = rect[0] + sizePair[0];
    rect[3] = rect[1] + sizePair[1];

    void* highlightObj = reinterpret_cast<void*>(self->field_001c);
    ((void (__fastcall*)(void*, void*, int*, uint8_t*))CSF_AT(void**, highlightObj, 0)[19])(
        highlightObj, nullptr, &rect[0], &g_byte_82E65C);

    void* highlightObjAgain = reinterpret_cast<void*>(self->field_001c);
    return ((int (__fastcall*)(void*, void*, float))CSF_AT(void**, highlightObjAgain, 0)[32])(
        highlightObjAgain, nullptr, 1.0f);
}

HOOK(0x00427DA0, S_00427bf0_UpdateHighlightRect, "auto");
