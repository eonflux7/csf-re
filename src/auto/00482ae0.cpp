#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_ConfigureFocusEntry, 0x0048E460, (void* self, void* edx, int* a2));
CSF_FUNC(int, __fastcall, GuiControl_SetInteractionState, 0x0048EEC0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Character_SetStateFlag, 0x00498F50, (void* self, void* edx, int a2, char a3));
CSF_FUNC(void, __fastcall, Character_SetActionEnabled, 0x0049AF10, (void* self, void* edx, char a3));
CSF_FUNC(char, __fastcall, ObjectProvider_SetPendingRequest, 0x004F5EF0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, ObjectProvider_CommitPositioning, 0x004F71B0, (void* self, void* edx, float a4));
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, GetFirstElement, 0x004FC1D0, (void* self, void*));
CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, Vector_ConvertDirection, 0x006ABA10, (void* self, void* edx, uint32_t* a2));

static int __fastcall CharActionFsm_PositionCharacterRelativeToOwner(
    void* self, void* /*edx*/, CPlayerCharacter* character, float distance)
{
    const int request = reinterpret_cast<int>(
        reinterpret_cast<std::uint8_t*>(CSF_AT(void*, self, 0x8)) + 28);
    int manager = GetGlobalValue_4FBDE0();
    int first = GetFirstElement(reinterpret_cast<void*>(manager), nullptr);
    void* provider = GetCachedObjectProvider(reinterpret_cast<void*>(first), nullptr);
    ObjectProvider_SetPendingRequest(provider, nullptr, request);

    Character_SetActionEnabled(CSF_AT(void*, self, 0x8), nullptr, 1);
    character->vSlot54();
    GuiControl_SetInteractionState(character, nullptr, 0);

    int characterPosition[3];
    std::memcpy(characterPosition,
        reinterpret_cast<void*>(GetObjectPositionVector(character, nullptr)), 0xC);

    void* characterController = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, character, 0)[0x28 / 4])(character, nullptr);
    if (((bool(__fastcall*)(void*, void*))
            CSF_AT(void**, characterController, 0)[0xA8 / 4])(
                characterController, nullptr))
    {
        characterController = ((void*(__fastcall*)(void*, void*))
            CSF_AT(void**, character, 0)[0x28 / 4])(character, nullptr);
        ((void(__fastcall*)(void*, void*))
            CSF_AT(void**, characterController, 0)[0xA4 / 4])(
                characterController, nullptr);
        character->field_01e4 = 0;
    }

    SceneLoader_ConfigureFocusEntry(character, nullptr, characterPosition);

    void* owner = CSF_AT(void*, self, 0x8);
    void* ownerController = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, owner, 0)[0x28 / 4])(owner, nullptr);
    void* ownerTransform = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, ownerController, 0)[0x18 / 4])(ownerController, nullptr);

    characterController = ((void*(__fastcall*)(void*, void*))
        CSF_AT(void**, character, 0)[0x28 / 4])(character, nullptr);
    C_008342e8* characterTransform = ((C_008342e8*(__fastcall*)(void*, void*))
        CSF_AT(void**, characterController, 0)[0x18 / 4])(
            characterController, nullptr);

    int ownerPosition[3];
    std::memcpy(ownerPosition,
        reinterpret_cast<void*>(GetObjectPositionVector(
            CSF_AT(void*, self, 0x8), nullptr)), 0xC);

    float* ownerCoordinates = reinterpret_cast<float*>(
        GetObjectPositionVector(CSF_AT(void*, self, 0x8), nullptr));
    float* characterCoordinates = reinterpret_cast<float*>(
        GetObjectPositionVector(character, nullptr));

    float direction[3];
    direction[2] = characterCoordinates[2] - ownerCoordinates[2];
    direction[1] = characterCoordinates[1] - ownerCoordinates[1];
    direction[0] = characterCoordinates[0] - ownerCoordinates[0];

    std::uint32_t convertedDirection[3];
    Vector_ConvertDirection(direction, nullptr, convertedDirection);
    ((void(__fastcall*)(void*, void*, std::uint32_t*))
        CSF_AT(void**, ownerTransform, 0)[0x58 / 4])(
            ownerTransform, nullptr, convertedDirection);

    // The saved scalar shares the final dword of the matrix-reference buffer.
    struct MatrixReference {
        std::uint32_t* matrix;
        std::uint8_t owned;
        std::uint8_t padding[3];
        float heading;
    };
    static_assert(sizeof(MatrixReference) == 0xC);
    MatrixReference reference{};
    reference.heading = static_cast<float>(
        ((double(__fastcall*)(void*, void*))
            CSF_AT(void**, ownerTransform, 0)[0x28 / 4])(ownerTransform, nullptr));

    reference.matrix = RwMatrixCreate();
    reference.owned = 1;
    ((void(__fastcall*)(void*, void*, MatrixReference*))
        CSF_AT(void**, ownerTransform, 0)[0x94 / 4])(
            ownerTransform, nullptr, &reference);
    if (reference.owned)
        RwMatrixDestroy(reference.matrix);

    ((void(__fastcall*)(void*, void*, float))
        CSF_AT(void**, ownerTransform, 0)[0x34 / 4])(
            ownerTransform, nullptr, reference.heading);

    reference.matrix = RwMatrixCreate();
    reference.owned = 1;
    ((void(__fastcall*)(void*, void*, MatrixReference*))
        CSF_AT(void**, characterTransform, 0)[0x94 / 4])(
            characterTransform, nullptr, &reference);
    if (reference.owned)
        RwMatrixDestroy(reference.matrix);

    std::uint32_t headingArgument;
    std::memcpy(&headingArgument, &reference.heading, sizeof(headingArgument));
    characterTransform->vSlot34(
        reinterpret_cast<std::uint32_t*>(headingArgument));

    SceneLoader_ConfigureFocusEntry(
        CSF_AT(void*, self, 0x8), nullptr, ownerPosition);

    float** matrixReference = ((float**(__fastcall*)(void*, void*, MatrixReference*))
        CSF_AT(void**, ownerTransform, 0)[0x8C / 4])(
            ownerTransform, nullptr, &reference);
    float* matrix = *matrixReference;
    direction[0] = distance * matrix[8];
    direction[1] = distance * matrix[9];
    direction[2] = distance * matrix[10];

    ownerCoordinates = reinterpret_cast<float*>(
        GetObjectPositionVector(CSF_AT(void*, self, 0x8), nullptr));
    float destination[3];
    destination[2] = direction[2] + ownerCoordinates[2];
    destination[1] = direction[1] + ownerCoordinates[1];
    destination[0] = direction[0] + ownerCoordinates[0];
    SceneLoader_ConfigureFocusEntry(
        character, nullptr, reinterpret_cast<int*>(destination));

    if (reference.owned)
        RwMatrixDestroy(reference.matrix);

    Character_SetStateFlag(CSF_AT(void*, self, 0x8), nullptr, 4096, 1);
    manager = GetGlobalValue_4FBDE0();
    first = GetFirstElement(reinterpret_cast<void*>(manager), nullptr);
    provider = GetCachedObjectProvider(reinterpret_cast<void*>(first), nullptr);

    // The original pushes integer bits 1, not floating-point 1.0f.
    const std::uint32_t commitBits = 1;
    float commitArgument;
    std::memcpy(&commitArgument, &commitBits, sizeof(commitArgument));
    return ObjectProvider_CommitPositioning(provider, nullptr, commitArgument);
}

HOOK(0x00482AE0, CharActionFsm_PositionCharacterRelativeToOwner, "auto");
