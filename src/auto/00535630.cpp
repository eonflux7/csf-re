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

// Byte-wise string equality as the original inlines it (strcmp-like).
// Returns 0 when both strings are identical, nonzero otherwise.
static int CompareIntroVideoNames(const char* left, const char* right)
{
    const unsigned char* a = reinterpret_cast<const unsigned char*>(left);
    const unsigned char* b = reinterpret_cast<const unsigned char*>(right);
    for (;;)
    {
        unsigned char ca = *a;
        unsigned char cb = *b;
        if (ca != cb)
            return 1;
        if (ca == 0)
            return 0;
        ++a;
        ++b;
    }
}

CSF_GLOBAL(const char*, g_pszMenu_P_Intro, 0x00899B2C);

// Original: __thiscall, one stack argument (ret 4).
// If the event tag is 14 and the intro video's stored target matches the event's
// second dword, the player's virtual slot 22 is invoked with the intro name; then if
// the name does not match the global intro name (or was absent), the intro slot is cleared.
static int __fastcall CMenuIntroVideo_ClearIntroIfNameMismatch(CMenuIntroVideo* self, void* /*edx*/, uint32_t* a2)
{
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(a2));

    if (a2[0] != 14)
        return result;

    void* player = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0010));
    if (player == nullptr)
        return result;

    if (self->field_0018 != a2[1])
        return result;

    void* introObj = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0024));
    if (introObj == nullptr)
        return result;

    uint32_t namePtr = CSF_AT(uint32_t, introObj, 0x0C);
    bool clearIntro = false;

    if (namePtr == 0)
    {
        clearIntro = true;
    }
    else
    {
        // Virtual slot 0x58 on the player object, receiving the intro name.
        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, player, 0)[0x58 / 4])(player, nullptr, namePtr);

        // Re-read the intro object after the call; the callee may have changed it.
        void* introObjAfter = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0024));
        const char* storedName = reinterpret_cast<const char*>(
            static_cast<uintptr_t>(CSF_AT(uint32_t, introObjAfter, 0x0C)));

        if (CompareIntroVideoNames(g_pszMenu_P_Intro, storedName) != 0)
            clearIntro = true;
    }

    if (clearIntro)
        self->field_0024 = 0;

    return result;
}

HOOK(0x00535630, CMenuIntroVideo_ClearIntroIfNameMismatch, "auto");
