#include <Library/HookLib.hpp>

EFI_STATUS
EFIAPI
Hook::HookJump(
    _In_ void* Address,
    _In_ void* JumpTo,
    _In_ bool IsReadOnly
)
{
    if (!Address || !JumpTo)
    {
        return EFI_INVALID_PARAMETER;
    }

    uint8_t MachineCode[] =
    {
        //
        // MOV RAX, Pointer
        //
        0x48, 0xB8,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        //
        // JMP RAX
        //
        0xFF, 0xE0
    };

    *reinterpret_cast<uint64_t*>(&MachineCode[2]) = reinterpret_cast<uint64_t>(JumpTo);

    if (IsReadOnly)
    {
        Util::CopyToReadOnly(Address, MachineCode, sizeof(MachineCode));
    }
    else
    {
        CopyMem(Address, MachineCode, sizeof(MachineCode));
    }

    if (CompareMem(Address, MachineCode, sizeof(MachineCode)) != 0)
    {
        return EFI_WRITE_PROTECTED;
    }

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
Hook::HookJumpBackup(
    _In_ void* Address,
    _In_ uint8_t (&Backup)[12]
)
{
    if (!Address || !Backup)
    {
        return EFI_INVALID_PARAMETER;
    }

    CopyMem(Backup, Address, sizeof(Backup));

    if (CompareMem(Address, Backup, sizeof(Backup)) != 0)
    {
        return EFI_WRITE_PROTECTED;
    }

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
Hook::UnhookJump(
    _In_ void* Address,
    _In_ uint8_t* Backup,
    _In_ bool IsReadOnly
)
{
    if (!Address || !Backup)
    {
        return EFI_INVALID_PARAMETER;
    }

    if (IsReadOnly)
    {
        Util::CopyToReadOnly(Address, Backup, 12);
    }
    else
    {
        CopyMem(Address, Backup, 12);
    }

    if (CompareMem(Address, Backup, 12) != 0)
    {
        return EFI_WRITE_PROTECTED;
    }

    return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
Hook::HookCall(
    _In_ void* Address,
    _In_ void* CallAddress,
    _In_ bool IsReadOnly
)
{
    if (!Address || !CallAddress)
    {
        return EFI_INVALID_PARAMETER;
    }

    uint8_t* TempAddress = reinterpret_cast<uint8_t*>(Address);
    int32_t RelativeOffset = static_cast<int32_t>(reinterpret_cast<uint8_t*>(CallAddress) - (TempAddress + 5));

    if (RelativeOffset < INT32_MIN || RelativeOffset > INT32_MAX)
    {
        return EFI_UNSUPPORTED;
    }

    uint8_t MachineCode[5] = { 0xE8 };
    *reinterpret_cast<int32_t*>(&MachineCode[1]) = static_cast<int32_t>(RelativeOffset);

    if (IsReadOnly)
    {
        Util::CopyToReadOnly(TempAddress, MachineCode, sizeof(MachineCode));
    }
    else
    {
        CopyMem(TempAddress, MachineCode, sizeof(MachineCode));
    }
    
    if (CompareMem(TempAddress, MachineCode, sizeof(MachineCode)) != 0)
    {
        return EFI_WRITE_PROTECTED;
    }
    
    return EFI_SUCCESS;
}