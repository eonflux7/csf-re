#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Task flags kept in field_0018:
//   bit 1 (2) = a path request is active / being followed
//   bit 2 (4) = finished (together with bit 1 when the destination is already reached)

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, PathFollower_AssignPath, 0x00426310, (void* self, void* edx, float a2, void* a3));
CSF_FUNC(int, __fastcall, C_0082faf8_ResetPathProgress, 0x00424440, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalNestedField14, 0x0044F3D0, ());
CSF_FUNC(float*, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0086c12c_ReleaseOwnedBuffer, 0x0075A190, (C_0086c12c* self, void*));
CSF_FUNC(uint32_t*, __fastcall, C_0086c12c_Construct, 0x0075A360, (C_0086c12c* self, void*));
CSF_FUNC(char, __fastcall, S_0075f7c0_FindPath_75FC40, 0x0075FC40, (S_0075f7c0* self, void* , float* origin, float* destination, C_0085e7b8* path, uint32_t* destinationValue));

static int __fastcall C_0082faf8_RequestPathToPosition(C_0082faf8* self, void* /*edx*/, float* destination, int /*unused*/)
{
    self->field_0018 &= 0xFFFFFFF9;

    uint32_t destinationValue = 0;

    alignas(8) uint8_t pathBytes[0x14];
    C_0086c12c* pathContainer = reinterpret_cast<C_0086c12c*>(pathBytes);
    C_0085e7b8* path = reinterpret_cast<C_0085e7b8*>(pathBytes);
    C_0086c12c_Construct(pathContainer, nullptr);

    // The original loads the current scene into ecx for the pathfinder getter (which ignores it).
    void* currentScene = g_pCurrentScene;
    (void)currentScene;

    float* origin = GetObjectPositionVector(reinterpret_cast<void*>(self->field_0014), nullptr);
    S_0075f7c0* pathFinder = reinterpret_cast<S_0075f7c0*>(GetGlobalNestedField14());
    char found = S_0075f7c0_FindPath_75FC40(pathFinder, nullptr, origin, destination, path, &destinationValue);

    if (GetCachedObjectProvider(pathBytes, nullptr) != nullptr)
    {
        // First stack argument is the path object; the shared binding types it as float, so pass its bits.
        void* pathPointer = pathBytes;
        float pathArgument;
        std::memcpy(&pathArgument, &pathPointer, sizeof(pathArgument));
        PathFollower_AssignPath(reinterpret_cast<uint8_t*>(self) + 0x24, nullptr, pathArgument,
                                reinterpret_cast<void*>(self->field_0014));

        C_0082faf8_ResetPathProgress(self, nullptr);

        uint8_t* character = reinterpret_cast<uint8_t*>(self->field_0014);

        alignas(8) uint32_t moveEvent[7];
        moveEvent[0] = 2;
        moveEvent[1] = (uint32_t)CSF_VTBL(0x0082D448);
        moveEvent[2] = CSF_AT(uint32_t, character, 0x20);
        moveEvent[3] = CSF_AT(uint32_t, character, 0x24);
        moveEvent[4] = CSF_AT(uint32_t, destination, 0x0);
        moveEvent[5] = CSF_AT(uint32_t, destination, 0x4);
        moveEvent[6] = CSF_AT(uint32_t, destination, 0x8);

        void* gameManager = reinterpret_cast<void*>(GetValue8B0554());
        uint8_t* dispatcher = reinterpret_cast<uint8_t*>(CSF_AT(void*, gameManager, 0x1FD8)) + 0x34;
        DispatchWithArgBlock(dispatcher, nullptr, moveEvent);

        self->field_0018 |= 2u;
        return C_0086c12c_ReleaseOwnedBuffer(pathContainer, nullptr);
    }

    // No path nodes: stop the character through its secondary interface at +4.
    uint8_t* characterInterface = reinterpret_cast<uint8_t*>(self->field_0014) + 4;
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, characterInterface, 0)[4])(characterInterface, nullptr);

    if (found)
        self->field_0018 |= 6u;
    else
        self->field_0018 = 0;

    return C_0086c12c_ReleaseOwnedBuffer(pathContainer, nullptr);
}

HOOK(0x00424820, C_0082faf8_RequestPathToPosition, "auto");
