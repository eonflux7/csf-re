#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __fastcall, SceneTrack_FindIndexForValue, 0x00468E20, (void* self, void* edx, int a2));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GlobalTable_GetEntryByIndex, 0x004FC140, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, ObjectTable_FindIndexById, 0x007686E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ObjectTable_GetEntryByIndex, 0x00768740, (void* self, void* edx, int a2));

// sub_4F53D0 is __usercall: provider in ecx, a2 in edi, returns al, plain ret (no stack args).
// The thunk is a pure tail jump: it loads ecx/edi from its stack arguments and jumps through
// the function pointer slot. sub_4F53D0's ret returns straight to the wrapper's call site,
// and the caller cleans the two __cdecl arguments.
static void* g_CachedProviderRunEntry = reinterpret_cast<void*>(CSF_ADDR_FN(0x004F53D0));

static __declspec(naked) char __cdecl CachedObjectProvider_RunWithEdi(void* provider, int a2)
{
    __asm {
        mov ecx, dword ptr [esp+4]
        mov edi, dword ptr [esp+8]
        jmp dword ptr ds:[g_CachedProviderRunEntry]
    }
}

static void* AsPointer(uint32_t value)
{
    return reinterpret_cast<void*>(static_cast<uintptr_t>(value));
}

static C_0085e7b8* AsResult(int value)
{
    return reinterpret_cast<C_0085e7b8*>(static_cast<intptr_t>(value));
}

// a3 is an array of three record references; the function links two keyed
// objects in the global table and runs the cached provider on them.
static C_0085e7b8* __stdcall Scene_LinkObjectPairAndRun(int /*unusedA1*/, int /*unusedA2*/, uint32_t* recordRefs)
{
    // recordRefs[0] -> record whose +4 points at a payload holding a float at +4.
    void* payloadA = CSF_AT(void*, AsPointer(recordRefs[0]), 4);
    int scalarKey = static_cast<int>(CSF_AT(float, payloadA, 4));

    // recordRefs[1] -> record whose +4 is an owner object; the owner's +4 is a key id.
    void* ownerA = CSF_AT(void*, AsPointer(recordRefs[1]), 4);
    void* sceneSubsystem = CSF_AT(void*, g_pCurrentScene, 0xBC);

    void* tableA = AsPointer(static_cast<uint32_t>(GetValue8B0554()));
    int idA = ObjectTable_FindIndexById(CSF_AT(void*, tableA, 0x1F90), nullptr,
                                        static_cast<int>(CSF_AT(uint32_t, ownerA, 4)));

    // recordRefs[2] -> record whose +4 is the second owner object.
    void* ownerB = CSF_AT(void*, AsPointer(recordRefs[2]), 4);

    void* tableB = AsPointer(static_cast<uint32_t>(GetValue8B0554()));
    int idB = ObjectTable_FindIndexById(CSF_AT(void*, tableB, 0x1F90), nullptr,
                                        static_cast<int>(CSF_AT(uint32_t, ownerB, 4)));

    C_0085e7b8* result = AsResult(idB);
    if (idA == -1 || idB == -1)
        return result;

    void* tableC = AsPointer(static_cast<uint32_t>(GetValue8B0554()));
    int entryA = ObjectTable_GetEntryByIndex(CSF_AT(void*, tableC, 0x1F90), nullptr, idA);

    void* tableD = AsPointer(static_cast<uint32_t>(GetValue8B0554()));
    int entryB = ObjectTable_GetEntryByIndex(CSF_AT(void*, tableD, 0x1F90), nullptr, idB);

    int subsystemIndex = SceneTrack_FindIndexForValue(static_cast<uint8_t*>(sceneSubsystem) + 0x94,
                                                      nullptr, scalarKey);
    result = AsResult(subsystemIndex);
    if (subsystemIndex == -1)
        return result;

    void* globalTable = AsPointer(static_cast<uint32_t>(GetGlobalValue_4FBDE0()));
    int globalEntry = GlobalTable_GetEntryByIndex(globalTable, nullptr, static_cast<unsigned int>(subsystemIndex));
    result = AsResult(globalEntry);
    if (globalEntry == 0)
        return result;

    void* provider = GetCachedObjectProvider(AsPointer(static_cast<uint32_t>(globalEntry)), nullptr);

    void* refA = GetReferencedObject(provider, nullptr);
    void* refAChild = reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, refA, 0)[4])(refA, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, refAChild, 0)[5])(
        refAChild, nullptr, entryA + 4);

    void* refB = GetReferencedObject(provider, nullptr);
    void* refBChild = reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, refB, 0)[4])(refB, nullptr);
    reinterpret_cast<void(__fastcall*)(void*, void*, int)>(CSF_AT(void**, refBChild, 0)[21])(
        refBChild, nullptr, entryB + 4);

    char providerResult = CachedObjectProvider_RunWithEdi(provider, entryA + 4);
    return AsResult(providerResult);
}

HOOK(0x0045DE60, Scene_LinkObjectPairAndRun, "auto");
