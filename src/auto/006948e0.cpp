#include "hook.h"

// Copies the file-name part of a path (text after the last '/' or '\\') into a2.
// If no separator exists, the whole path is copied when it fits.
// Returns the length of the input path, except when the whole-path copy runs (no separator, fits in a3):
// then the original returns its source pointer after the copy loop, one past the terminator (a1 + length + 1),
// because the copy loop keeps its pointer in eax. The copy is skipped when the destination
// buffer (capacity a3) is too small. The original inlines strlen and strcpy, so no calls are made here.
static unsigned int __cdecl SceneLoader_CopyMapFilename(const char* a1, char* a2, unsigned int a3)
{
    // strlen(a1), inlined
    unsigned int pathLength = 0;
    unsigned int result = 0;  // value returned: pathLength, or the copy's end pointer (see above)
    while (a1[pathLength] != '\0')
    {
        ++pathLength;
    }

    // Scan backwards for the last path separator.
    unsigned int separatorIndex = pathLength - 1;
    bool separatorFound = false;
    if (pathLength != 0)
    {
        while (true)
        {
            char ch = a1[separatorIndex];
            if (ch == '/' || ch == '\\')
            {
                separatorFound = true;
                break;
            }
            if (--separatorIndex == 0xFFFFFFFFu)
            {
                break;
            }
        }
    }

    result = pathLength;
    if (separatorFound)
    {
        // Copy the name after the separator if it fits in the buffer.
        if (pathLength - (separatorIndex + 1) < a3)
        {
            const char* src = a1 + separatorIndex + 1;
            char* dst = a2;
            char ch;
            do
            {
                ch = *src;
                *dst = ch;
                ++dst;
                ++src;
            } while (ch != '\0');
        }
    }
    else
    {
        // No separator: copy the whole path if it fits (strcpy, inlined).
        if (pathLength < a3)
        {
            const char* src = a1;
            char* dst = a2;
            char ch;
            do
            {
                ch = *src;
                *dst = ch;
                ++dst;
                ++src;
            } while (ch != '\0');
            // Original: 00694949 mov eax, edi ... 00694958 jne -> eax ends one past the NUL.
            result = reinterpret_cast<unsigned int>(src);
        }
    }

    return result;
}

HOOK(0x006948E0, SceneLoader_CopyMapFilename, "auto");
