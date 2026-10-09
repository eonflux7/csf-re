#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, sub_51BA50, 0x0051BA50, (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());
CSF_FUNC(double, __cdecl, sub_70E600, 0x0070E600, (char* a1));
CSF_FUNC(int, __fastcall, sub_75A480, 0x0075A480, (void* self, void* edx, int a2));

static double __stdcall ConvertValueToScalar(int a1)
{
    double result = 0.0;

    // Sound table lives at +0x1FD0 of the object returned by GetValue8B0554.
    void* managerBase = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
    C_0085e7b8* soundTable = reinterpret_cast<C_0085e7b8*>(
        static_cast<uintptr_t>(CSF_AT(uint32_t, managerBase, 0x1FD0)));

    // Virtual slot 0xC -> index 3: resolve a handle from the name/ID argument.
    int handle = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, soundTable, 0)[3])(
        soundTable, nullptr, a1);
    if (handle == -1)
        return 0.0;

    int entryValue = sub_75A480(soundTable, nullptr, handle);
    void* entry = reinterpret_cast<void*>(static_cast<uintptr_t>(entryValue));

    void* nameNode = sub_51BA50(soundTable, nullptr, static_cast<unsigned int>(handle));

    // Strings with capacity >= 0x10 are heap-allocated (pointer at +4); smaller ones are inline at +4.
    char* text;
    if (CSF_AT(uint32_t, nameNode, 0x18) < 0x10u)
        text = reinterpret_cast<char*>(nameNode) + 4;
    else
        text = CSF_AT(char*, nameNode, 4);

    if (CSF_AT(uint8_t, entry, 0x22) != 0)
        return sub_70E600(text);

    // Otherwise forward the name to the global object's slot 0x88 (index 34).
    void* globalObject = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue_70DDE0()));
    ((void(__fastcall*)(void*, void*, char*))CSF_AT(void**, globalObject, 0)[34])(
        globalObject, nullptr, text);

    // The original returns whatever is left on the FPU stack here; it is uninitialized, so pass 0.
    return result;
}

HOOK(0x00523140, ConvertValueToScalar, "auto");
