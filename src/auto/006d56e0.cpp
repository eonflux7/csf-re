#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(unsigned int, __fastcall, MissionScript_BitArraySetCount, 0x00470F50, (S_00470440* self, void* , unsigned int newCount, char fillValue));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(std::uint32_t, __cdecl, CloseSceneFile, 0x00697980, (void* file));
CSF_FUNC(void*, __cdecl, CoreFiles_OpenSceneFile, 0x00698350, (const char* filename, char* mode, int openFlags));
CSF_FUNC(int, __fastcall, SceneLoader_FinalizeObjectProvider, 0x006D4450, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, S_006d4450_FindObjectIndex, 0x006D46A0, (void* self, void* edx, char* String2));
CSF_FUNC(void*, __cdecl, GfxClumpDict_MoveObjectRange, 0x006D4A70, (void* a1, void* a2, void* a3));
CSF_FUNC(char, __cdecl, GfxClumpDict_DestroyObjectRange, 0x006D4B40, (int a1, int a2));
CSF_FUNC(int*, __fastcall, GfxClumpDict_EraseBit, 0x006D4EC0, (void* self, void* edx, int* a2, unsigned int a3, int* a4));
CSF_FUNC(int, __fastcall, S_006d4450_AddCachedObject, 0x006D5560, (void* self, void* edx, char* String2, int a3));

static char __fastcall S_006d4450_ProcessCachedObjectPath(
    S_006d4450* self, void* /*edx*/, const char* filename)
{
    alignas(8) std::uint8_t bitArrayBytes[0x14];
    S_00470440& seen = *reinterpret_cast<S_00470440*>(bitArrayBytes);
    seen.field_0000 = 0;
    seen.field_0008 = 0;
    CSF_AT(std::uint32_t, bitArrayBytes, 0x0C) = 0;
    CSF_AT(std::uint32_t, bitArrayBytes, 0x10) = 0;

    const std::uint32_t begin = self->field_0008;
    const unsigned int count = begin
        ? static_cast<unsigned int>(
            static_cast<std::int32_t>(
                CSF_AT(std::uint32_t, self, 0x0C) - begin) / 60)
        : 0;
    MissionScript_BitArraySetCount(&seen, nullptr, count, 0);

    void* file = CoreFiles_OpenSceneFile(
        filename, const_cast<char*>(CSF_STR(0x0083E130, "r")), 0);
    if (!file) {
        if (seen.field_0008)
            Mem_FreeDebug(reinterpret_cast<void*>(seen.field_0008));
        return 0;
    }

    char objectName[256];
    int objectValue = 0;
    int nameLength = 0;

    ((void(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, file, 0)[8 / 4])(
            file, nullptr, &nameLength, 4);

    while (nameLength) {
        ((void(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, file, 0)[8 / 4])(
                file, nullptr, objectName, nameLength);
        objectName[nameLength] = 0;

        ((void(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, file, 0)[8 / 4])(
                file, nullptr, &objectValue, 4);

        const unsigned int index =
            S_006d4450_FindObjectIndex(self, nullptr, objectName);
        if (index == static_cast<unsigned int>(-1)) {
            S_006d4450_AddCachedObject(
                self, nullptr, objectName, objectValue);
        } else {
            CSF_AT(std::uint32_t,
                reinterpret_cast<void*>(seen.field_0008),
                4 * (index >> 5)) |= 1u << (index & 0x1F);
        }

        ((void(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, file, 0)[8 / 4])(
                file, nullptr, &nameLength, 4);
    }

    CloseSceneFile(file);

    alignas(8) std::uint8_t eraseStateBytes[0x8];
    int* eraseState = reinterpret_cast<int*>(eraseStateBytes);
    eraseState[0] = 0; // Originally an uninitialized argument.

    std::uint32_t index = seen.field_0000 - 1;
    if (static_cast<std::int32_t>(index) >= 0) {
        std::uint32_t objectOffset = 60 * index;
        do {
            eraseState[1] = 1;
            if ((CSF_AT(std::uint32_t,
                    reinterpret_cast<void*>(seen.field_0008),
                    4 * (index >> 5)) & (1u << (index & 0x1F))) == 0) {
                ((void(__fastcall*)(void*, void*, std::uint32_t))
                    CSF_AT(void**, self, 0)[20 / 4])(
                        self, nullptr, objectOffset + self->field_0008);

                GfxClumpDict_MoveObjectRange(
                    reinterpret_cast<void*>(objectOffset + self->field_0008 + 60),
                    reinterpret_cast<void*>(CSF_AT(std::uint32_t, self, 0x0C)),
                    reinterpret_cast<void*>(objectOffset + self->field_0008));

                const std::uint32_t end =
                    CSF_AT(std::uint32_t, self, 0x0C);
                GfxClumpDict_DestroyObjectRange(
                    static_cast<int>(end - 60), static_cast<int>(end));

                CSF_AT(std::uint32_t, self, 0x0C) -= 60;
                GfxClumpDict_EraseBit(
                    &seen, nullptr, eraseState, index & 0x1F,
                    reinterpret_cast<int*>(seen.field_0008 + 4 * (index >> 5)));
            }
            --index;
            objectOffset -= 60;
        } while (static_cast<std::int32_t>(index) >= 0);
    }

    SceneLoader_FinalizeObjectProvider(self, nullptr);
    if (seen.field_0008)
        Mem_FreeDebug(reinterpret_cast<void*>(seen.field_0008));
    return 1;
}

HOOK(0x006D56E0, S_006d4450_ProcessCachedObjectPath, "auto");
