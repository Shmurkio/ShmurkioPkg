#pragma once

extern "C"
{
    #include <Base.h>
    #include <Uefi.h>
    #include <Library/UefiLib.h>
    #include <Library/UefiBootServicesTableLib.h>
    #include <Library/BaseMemoryLib.h>

    #include <sal.h>
}

#include <Library/UtilLib.hpp>

#include <cstdint>

#define HOOK_JUMP_BACKUP(BackupName) \
    uint8_t gOrig##BackupName[12]

namespace Hook
{
    EFI_STATUS
    EFIAPI
    HookJump(
        _In_ void* Address,
        _In_ void* JumpTo,
        _In_ bool IsReadOnly
    );

    EFI_STATUS
    EFIAPI
    HookJumpBackup(
        _In_ void* Address,
        _In_ uint8_t (&Backup)[12]
    );

    EFI_STATUS
    EFIAPI
    UnhookJump(
        _In_ void* Address,
        _In_ uint8_t* Backup,
        _In_ bool IsReadOnly
    );

    EFI_STATUS
    EFIAPI
    HookCall(
        _In_ void* Address,
        _In_ void* CallAddress,
        _In_ bool IsReadOnly
    );
}