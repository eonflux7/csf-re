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

// Callee bindings (addresses from the disassembly).
CSF_FUNC(int, __fastcall, LoadGuiBuilderFromResource, 0x71F6E0, (void* self, void* edx, const char* resourcePath));
CSF_FUNC(int, __cdecl, GetGlobalInt, 0x6B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x652160, (int objectIndex, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x6910F0, (size_t byteCount));
CSF_FUNC(int, __fastcall, C_00864508_Initialize, 0x71D6D0, (void* self, void* edx, void* owner, int a3, int a4));
CSF_FUNC(int, __fastcall, MenuSystem_AssignDialogPanel, 0x712E80, (void* self, void* edx, int panel));
CSF_FUNC(int, __cdecl, GetValue_8B0574, 0x518980, ());

static int __fastcall MenuSystem_InitCommonMenuBuilder(void* self, void* /*edx*/)
{
    // Virtual slot 0x94 on self (returns the object that receives the builder result).
    void* targetObject = reinterpret_cast<void*>(
        ((int(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x94 / 4])(self, nullptr));

    LoadGuiBuilderFromResource(targetObject, nullptr, CSF_STR(0x840544, "Menus/P_Common.fbs"));

    // First global object: read int at [obj+0] from virtual slot 0x18 result.
    int firstGlobalIndex = GetGlobalInt();
    void* firstGlobalObject = LookupObjectById(firstGlobalIndex, nullptr);
    void* firstResult = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, firstGlobalObject, 0)[0x18 / 4])(firstGlobalObject, nullptr);
    int ownerValue = CSF_AT(int, firstResult, 0);

    // Second global object: read int at [result+4].
    int secondGlobalIndex = GetGlobalInt();
    void* secondGlobalObject = LookupObjectById(secondGlobalIndex, nullptr);
    void* secondResult = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, secondGlobalObject, 0)[0x18 / 4])(secondGlobalObject, nullptr);
    int secondaryValue = CSF_AT(int, secondResult, 4);

    int builtObject = 0;
    void* allocated = Mem_Alloc(0xA8);
    if (allocated)
    {
        builtObject = C_00864508_Initialize(allocated, nullptr, targetObject, ownerValue, secondaryValue);
    }
    else
    {
        builtObject = 0;
    }

    CSF_AT(int, self, 0x14) = builtObject;
    MenuSystem_AssignDialogPanel(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x10)), nullptr, builtObject);

    int result = GetValue_8B0574();

    CSF_AT(uint32_t, targetObject, 0x80) = CSF_AT(uint32_t, self, 0x10);
    return result;
}

HOOK(0x0050D270, MenuSystem_InitCommonMenuBuilder, "auto");
