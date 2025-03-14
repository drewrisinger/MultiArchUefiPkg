# MultiArchUefiPkg - Multi-Architecture UEFI Environment Driver

This repo implements a UEFI driver that allows non-native UEFI boot
service drivers and applications to be executed in 64-bit UEFI environments.

The primary motivation is bringing together the IHV device and
standardized AARCH64 and RISCV64 ecosystems. This driver allows plugging
off the shelf storage, network and video cards into non-x86 machines
and using them with UEFI for OS boot.

Today, AArch64 and RISC-V are supported as host environments.
In AArch64 environments, EmulatorDxe supports X64 (aka x86_64, AMD64)
UEFI binaries. On RISCV64, EmulatorDxe can be build with X64 and AARCH64
UEFI binary support.

It's derived from [X86EmulatorPkg](https://github.com/ardbiesheuvel/X86EmulatorPkg),
yet is otherwise a reimplementation using a [UEFI-enabled version](https://github.com/intel/unicorn-for-efi)
of the [Unicorn Engine](https://www.unicorn-engine.org/) - a flexible
CPU emulator framework.

MultiArchUefiPkg has competitive performance, portability and size
(2/3rds the binary size on AArch64), support for multiple emulated ISAs,
a regression test suite, and improved (but not perfect) correctness in
modeling the emulated UEFI Boot Service environment.

[See a demo](https://youtu.be/ntZ177E3lRY).

## Theory Of Operation

UEFI code uses a pretty narrowly-defined ABI, which makes it
easy to thunk x64/AArch64 client code making EFIAPI calls to
native code: no FP/SIMD, no returning large values, etc. E.g. calls like:

        UINT64
        EFIAPI
        Fn(UINT64, UINT64, UINT64, UINT64,
           UINT64, UINT64, UINT64, UINT64,
           UINT64, UINT64, UINT64, UINT64,
           UINT64, UINT64, UINT64, UINT64);

...with up to 16 arguments are supported both client -> native
and native -> client, which covers all UEFI needs.

Seamless thunking from native code to emulated code relies on MMU
support in the UEFI firmware, specifically the ability to mark a
range non-executable.

MultiArchUefiPkg won't work with any UEFI implementations, but only
with implementations that provide the EDKII_PECOFF_IMAGE_EMULATOR_PROTOCOL
interface, which is the magic that enables loading foreign ISA binaries.
Today this means you must use TianoCore EDK2 or a derived implementation.

The emulator presents an x64 and/or AArch64 UEFI Boot Services
environment appropriate for running Boot Service drivers (e.g. OpRom
drivers for video cards, NICs) and UEFI applications that aren't OS
loaders. Certain Boot, Runtime and DXE services are filtered or disabled.

## Quick Start

To quickly build a RISC-V version:

        $ git clone https://github.com/tianocore/edk2.git
        $ cd edk2
        $ git submodule https://github.com/intel/unicorn-for-efi.git unicorn
        $ git submodule add https://github.com/intel/MultiArchUefiPkg.git
        $ git submodule update --init
        $ export GCC5_RISCV64_PREFIX=... (if you are on a non-RISCV64 system)
        $ build -a RISCV64 -t GCC5 -p MultiArchUefiPkg/Emulator.dsc -b RELEASE

This will produce Build/MultiArchUefiPkg/RELEASE_GCC5/RISCV64/EmulatorDxe.efi

Also see the [document on building](Docs/Building.md).

## Modeled Environment

The emulator presents UEFI Boot Services environment appropriate
for running Boot Service drivers (e.g. OpRom drivers for video cards, NICs)
and UEFI applications that aren't OS loaders.

The details are described by [a separate document](Docs/EmulatedEnvironment.md).

# License

MultiArchUefiPkg is approved for release under GPLv2 and LGPLv2.1+ for the respective components under those licenses. EmulatorDxe itself is covered by the [LGPL v2.1+](LICENSE), but it is statically linked to the [Unicorn Engine](https://www.unicorn-engine.org/) library, which has a mix of LGPL and GPLv2 code.

# Security Policy

Intel is committed to rapidly addressing security vulnerabilities affecting our customers and providing clear guidance on the solution, impact, severity and mitigation. 

## Reporting A Vulnerability
Please report any security vulnerabilities in this project utilizing the guidelines [here](https://www.intel.com/content/www/us/en/security-center/vulnerability-handling-guidelines.html).

# Contribute

This is a [RISE Project](https://riseproject.dev) under the Firmware WG.

Contributions are welcome. Please raise issues and pull requests.

Please see the [policy on contributions](CONTRIBUTING.md) and our [Code of Conduct](CODE_OF_CONDUCT.md).
