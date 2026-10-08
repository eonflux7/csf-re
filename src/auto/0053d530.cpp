#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// C runtime _sprintf (cdecl). Called with (buffer, format, index).
CSF_FUNC(int, __cdecl, LibSprintf, 0x7AF9BB, (char* buffer, const char* format, int value));

// Virtual lookup by name: vtable slot 0xFC. Returns the child control, or null.
using PanelChildLookupFn = void* (__fastcall*)(void* self, void* edx, const char* childName);

// Virtual setter: vtable slot 0xD8. Takes one int argument and returns eax.
using PanelChildSetFn = int (__fastcall*)(void* self, void* edx, int value);

// Sets the state of the three "FotoN" and "FotoNTachada" children of PanelBichos to 0.
// Returns the eax value left by the last child setter call (0 if that child was missing).
static int __fastcall CMenuMissionInfo_ResetBichosPhotoFlags(CMenuMissionInfo* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_001c);
    void* panel = ((PanelChildLookupFn)CSF_AT(void**, owner, 0)[63])(owner, nullptr, CSF_STR(0x00842CD4, "PanelBichos"));

    int result = 0;
    if (panel != nullptr)
    {
        for (int photoIndex = 1; photoIndex <= 3; ++photoIndex)
        {
            char buffer[32];

            LibSprintf(buffer, CSF_STR(0x00842CCC, "Foto%d"), photoIndex);
            void* photo = ((PanelChildLookupFn)CSF_AT(void**, panel, 0)[63])(panel, nullptr, buffer);
            if (photo != nullptr)
            {
                ((PanelChildSetFn)CSF_AT(void**, photo, 0)[54])(photo, nullptr, 0);
            }

            LibSprintf(buffer, CSF_STR(0x00842CBC, "Foto%dTachada"), photoIndex);
            photo = ((PanelChildLookupFn)CSF_AT(void**, panel, 0)[63])(panel, nullptr, buffer);
            result = 0;
            if (photo != nullptr)
            {
                result = ((PanelChildSetFn)CSF_AT(void**, photo, 0)[54])(photo, nullptr, 0);
            }
        }
    }
    return result;
}

HOOK(0x0053D530, CMenuMissionInfo_ResetBichosPhotoFlags, "auto");
