#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Converts both text arguments through the display-encoding helper, then passes
// them to the localized message routine. The original leaves the first two
// converter calls' arguments on the stack and cleans them all up with one
// `add esp, 0x18` after the final call, so the third argument of the final call
// is the raw second parameter `a2` (its low byte as char).
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int a1, int a2));
CSF_FUNC(char, __cdecl, Dialog_ShowLocalizedMessage, 0x0069CE50,
         (const wchar_t* lpCaption, wchar_t* text, char a3));

static int __cdecl ParticleSystemDefs_ConvertAndShowMessage(int a1, int a2)
{
    const wchar_t* caption =
        reinterpret_cast<const wchar_t*>(Text_ConvertToDisplayEncoding(a1, 8));
    wchar_t* text =
        reinterpret_cast<wchar_t*>(Text_ConvertToDisplayEncoding(a2, 8));
    return static_cast<int>(
        Dialog_ShowLocalizedMessage(caption, text, static_cast<char>(a2)));
}

HOOK(0x00701EC0, ParticleSystemDefs_ConvertAndShowMessage, "auto");
