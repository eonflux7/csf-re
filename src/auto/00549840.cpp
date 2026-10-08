#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// CRT atol (shown as j__atol in the decompilation). cdecl, one argument, caller cleans the stack.
CSF_FUNC(int, __cdecl, ParseThumbnailIndex, 0x007AFD89, (const char* text));

// Text-source reader: cdecl (source, buffer, maxLength). Copies up to 256 bytes into the buffer.
// Its return value is not used by this function.
CSF_FUNC(unsigned int, __cdecl, ConvertWideTextToNarrow, 0x00695620, (int source, char* buffer, unsigned int maxLength));

// Signature of the vtable slot 8 accessor on the resolution source object (__thiscall, so __fastcall here).
using GetCurrentModePairFn = int* (__fastcall*)(void* self, void* edx);

// Copies exactly `count` bytes forward, byte by byte, like the original rep movsd + rep movsb pair.
static void CopyTextBytes(char* destination, const char* source, unsigned int count)
{
    for (unsigned int i = 0; i < count; ++i)
        destination[i] = source[i];
}

// Writes the current mode pair from the resolution source object to *outWidth / *outHeight, then reads
// a text source and parses the "A x B" text into the two outputs.
static void __fastcall CMenuVideoOptions_ParseResolutionPair(CMenuVideoOptions* self, void* /*edx*/,
                                                             int textSource, int* outWidth, int* outHeight)
{
    if (textSource == 0)
        return;

    // 384-byte scratch frame laid out like the original locals, contiguous:
    //   rawText = frame[0x000..0x0FF] (256-byte read buffer; the 253-byte view overlaps the following locals)
    //   token   = frame[0x100..0x17C] (125-byte token buffer)
    char frame[0x180] = {};
    char* rawText = frame;
    char* token = frame + 0x100;

    // Query the current mode pair from the resolution source object (vtable slot 8, byte offset 0x20).
    void* sourceObject = reinterpret_cast<void*>(self->field_0024);
    int* modePair = ((GetCurrentModePairFn)CSF_AT(void**, sourceObject, 0)[8])(sourceObject, nullptr);
    const int modeWidth = modePair[0];
    const int modeHeight = modePair[1];
    *outWidth = modeWidth;
    *outHeight = modeHeight;

    ConvertWideTextToNarrow(textSource, rawText, 256);

    // Length of the text (strlen equivalent).
    unsigned int textLength = 0;
    while (rawText[textLength] != '\0')
        ++textLength;
    if (textLength == 0)
        return;

    // Find the first 'x' separator. If none is found, the function returns without further output.
    bool separatorFound = false;
    unsigned int separatorIndex = 0;
    while (!separatorFound)
    {
        if (rawText[separatorIndex] == 'x')
            separatorFound = true;
        else
            ++separatorIndex;

        if (separatorIndex >= textLength)
        {
            if (!separatorFound)
                return;
            break;
        }
    }

    // First token: copies separatorIndex - 1 bytes (as in the original), terminates at separatorIndex, parses.
    CopyTextBytes(token, rawText, separatorIndex - 1);
    token[separatorIndex] = '\0';
    *outWidth = ParseThumbnailIndex(token);

    // Second token: copies textLength - separatorIndex bytes starting at separatorIndex + 2, parses.
    CopyTextBytes(token, rawText + separatorIndex + 2, textLength - separatorIndex);
    *outHeight = ParseThumbnailIndex(token);
}

HOOK(0x00549840, CMenuVideoOptions_ParseResolutionPair, "auto");
