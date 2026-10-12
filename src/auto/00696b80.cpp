#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(size_t, __cdecl, _wcslen, 0x007B0499, (const wchar_t* str));
CSF_FUNC(int, __fastcall, MenuMpAccounts_CompareNames, 0x005158F0,
         (void* self, void* edx, unsigned int offset, unsigned int length,
          const uint16_t* text, unsigned int textLength));

// Original: BOOL __thiscall MenuProgressInput_InitializeText(std::uint32_t* this, wchar_t* String)
// Returns 1 when MenuMpAccounts_CompareNames reports a non-zero match, 0 otherwise.
static int __fastcall MenuProgressInput_InitializeText(void* self, void* /*edx*/, wchar_t* String)
{
    size_t textLength = _wcslen(String);
    uint32_t storedValue = CSF_AT(uint32_t, self, 0x14);
    int compareResult = MenuMpAccounts_CompareNames(self, nullptr, 0, storedValue,
                                                    reinterpret_cast<const uint16_t*>(String),
                                                    static_cast<unsigned int>(textLength));
    return (compareResult != 0) ? 1 : 0;
}

HOOK(0x696B80, MenuProgressInput_InitializeText, "auto");
