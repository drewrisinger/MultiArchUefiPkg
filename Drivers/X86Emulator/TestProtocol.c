/** @file

    Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

**/

#include "X86Emulator.h"
#include "TestProtocol.h"

#ifndef NDEBUG

#ifdef MDE_CPU_AARCH64
#define HOST_MACHINE_TYPE EFI_IMAGE_MACHINE_AARCH64
#elif defined (MDE_CPU_RISCV64)
#define HOST_MACHINE_TYPE EFI_IMAGE_MACHINE_RISCV64
#else
#error
#endif

STATIC
UINT64
EFIAPI
TestRet (
  VOID
  )
{
  return RET_VAL;
}

RET16
EFIAPI
TestRet16 (
  IN  UINT64 Arg1
  )
{
  RET16 r = { 0 };

  if (Arg1 == ARG_VAL(1)) {
    r.Field1 = FIELD_VAL(1);
    r.Field2 = FIELD_VAL(2);
  }

  return r;
}

RET_LARGE
EFIAPI
TestLargeRet (
  IN  UINT64 Arg1
  )
{
  RET_LARGE r = { 0 };

  if (Arg1 == ARG_VAL(1)) {
    r.Field1 = FIELD_VAL(1);
    r.Field2 = FIELD_VAL(2);
    r.Field3 = FIELD_VAL(3);
    r.Field4 = FIELD_VAL(4);
  }

  return r;
}

STATIC
EFI_STATUS
EFIAPI
TestArgs (
  IN  UINT64 Arg1,
  IN  UINT64 Arg2,
  IN  UINT64 Arg3,
  IN  UINT64 Arg4,
  IN  UINT64 Arg5,
  IN  UINT64 Arg6,
  IN  UINT64 Arg7,
  IN  UINT64 Arg8,
  IN  UINT64 Arg9,
  IN  UINT64 Arg10,
  IN  UINT64 Arg11,
  IN  UINT64 Arg12,
  IN  UINT64 Arg13,
  IN  UINT64 Arg14,
  IN  UINT64 Arg15,
  IN  UINT64 Arg16
  )
{
  if (Arg1 == ARG_VAL(1) &&
      Arg2 == ARG_VAL(2) &&
      Arg3 == ARG_VAL(3) &&
      Arg4 == ARG_VAL(4) &&
      Arg5 == ARG_VAL(5) &&
      Arg6 == ARG_VAL(6) &&
      Arg7 == ARG_VAL(7) &&
      Arg8 == ARG_VAL(8) &&
      Arg9 == ARG_VAL(9) &&
      Arg10 == ARG_VAL(10) &&
      Arg11 == ARG_VAL(11) &&
      Arg12 == ARG_VAL(12) &&
      Arg13 == ARG_VAL(13) &&
      Arg14 == ARG_VAL(14) &&
      Arg15 == ARG_VAL(15) &&
      Arg16 == ARG_VAL(16)) {
    return EFI_SUCCESS;
  }

  return EFI_INVALID_PARAMETER;
}

STATIC
UINT64
EFIAPI
TestCb (
  IN  UINT64        Arg1,
  IN  UINT64        Arg2,
  IN  UINT64 EFIAPI (*Cb)(UINT64, UINT64)
  )
{
  return Cb(Arg1, Arg2);
}

STATIC X86_EMU_TEST_PROTOCOL mX86EmuTestProtocol = {
  HOST_MACHINE_TYPE,
  TestRet,
  TestRet16,
  TestLargeRet,
  TestArgs,
  TestCb,
};

STATIC EFI_GUID mX86EmuTestProtocolGuid = X86_EMU_TEST_PROTOCOL_GUID;

EFI_STATUS
TestProtocolInit (
  IN  EFI_HANDLE ImageHandle
  )
{
  EFI_STATUS Status;

  Status = gBS->InstallProtocolInterface (&ImageHandle,
                                          &mX86EmuTestProtocolGuid,
                                          EFI_NATIVE_INTERFACE,
                                          &mX86EmuTestProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InstallProtocolInterface failed: %r\n", Status));
  }

  return Status;
}

VOID
TestProtocolCleanup (
  IN  EFI_HANDLE ImageHandle
  )
{
  gBS->UninstallProtocolInterface (ImageHandle,
                                   &mX86EmuTestProtocolGuid,
                                   &mX86EmuTestProtocol);
}

#endif
