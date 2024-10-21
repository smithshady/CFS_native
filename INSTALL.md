Scripts should be run from inside the scripts directory:

```bash
$ cd scripts/
```

Initialize all the submodules:

```bash
$ source setup_repository.sh
```

Set up environment variables:

```bash
$ source setup_cfs_env_<target>.sh
```

Available targets are
- `freertos`
- `posix`
- `rtems`

Clean and build cFS from source:

```bash
$ source build_cfs.sh
```

After this, you can find the executable and other build artefacts in
`build/exe/<target>/`.

---

## Building for RTEMS 6 on the ATSAMv71Q21B

First, clone and build the Lunar Outpost fork of the RTEMS source and the
official upstream RTEMS Source Builder (RSB):

```bash
# Get the sources
$ git clone ssh://git@bitbucket.lunaroutpost.com:7999/rov/rtems.git
$ git clone ssh://git@gitlab.rtems.org:2222/rtems/tools/rtems-source-builder.git

# Follow installation instructions in rtems/INSTALL.md
```

Then, simply follow the installation instructions above for cFS. You'll need to
perform one additional step. Before running `build_cfs.sh`, you should run
`rtems_tarballer.sh`. This will initially only include the startup script in
the base filesystem. To get the app table files as well, you'll have to run
`rtems_tarballer.sh` again after a successful table build (so that the staging
files exist).

The process for subsequent builds will be to run the tarballer script (iff the
startup script or app tables change), then the build script.

## cFS Changes

Adding the new `arm-rtems6-atsamv` target mostly involved changes in the
`sample_defs/` directory, where a number of files were added:

`sample_defs/`
- `targets.cmake`
    defines the target name, and lists of apps and files to build and link

- `toolchain-arm-rtems6-atsamv.cmake`
    configures the build environment for the target (OSAL/PSP to use,
    compiler/linker flags, tools locations, etc.)

- `bsp_rtems_cfg.h`
    defines configuration options for RTEMS itself, including clock
    frequencies, driver selection, and resource allocation

- `default_cfe_es_internal_cfg.h`
    overrides a config file for the cFE core app ES (Executive Services) - all
    options match the defaults except `CFE_PLATFORM_ES_RAM_DISK_SECTOR_SIZE`,
    `CFE_PLATFORM_ES_RAM_DISK_NUM_SECTORS`, and
    `CFE_PLATFORM_ES_USER_RESERVED_SIZE`

- `arm-rtems6-atsamv_osconfig.cmake`
    configures the OSAL by setting CMake variables, the full enumeration of
    which can be found in `osal/default_config.cmake`

- `arm-rtems6-atsamv_cfe_es_startup.scr`
    is the startup script that will be used to load apps and libraries when cFS
    starts

For a description of what every option does, read through the comments in the
above files. A few key points about how the RTEMS target works:

### Filesystem

To get the startup files (startup script, app table files) into a filesystem
for initial use, we utilize the RTEMS "tarfs" feature. This allows us to roll
the desired files and directories into a tarball, convert this to a binary
object file which is linked to the final cFS executable, and subsequently
unroll them into the RTEMS base IMFS.

### Networking

Build the RTEMS networking library using the `build_networking.sh` script in
the RTEMS repository. Then, just make sure that the RTEMS networking library
gets linked to the final cFS executable using the `-lnetworking` switch in the
toolchain file.

On the cFS side, network initialization occurs in the PSP startup code in
`psp/fsw/generic-rtems/src/cfe_psp_start.c`. The code that comes in this file
by default refers to nonexistant unit test symbols and doesn't work. It has
been commented out. The setup code put in its place simply starts up the
loopback interface (127.0.0.1), but it should be straightforward enough to use
a different (real) interface.

Using the loopback allows for very easy testing of the POSIX build using the
provided ground system simulator located in `tools/cFS-GroundSystem`. It's just
a little Python app that dumps CCSDS packets into a UDP socket, and it's a
little out of date, but it's still quite useful for confirming that things like
telemetry are being sent correctly.

If you use the ground system, make sure to set the __command header version__
to "custom", and enter `4` and `0` for the two values to the right of that on
the main screen. Leave the telemetry header version at `1`.

### PSP

A new PSP target `generic-rtems` was added, mostly an exact copy of the
existing `pc-rtems`, except for the networking code (see above).

The default PSP memory usage includes a request for 1 MiB of memory for the
"User Reserved" region. This is meant to be used by apps, but the default
allocation is too large on our tiny 4 MiB of MRAM, and doesn't leave enough
space for RTEMS to allocate task stacks for all the core apps. This value was
adjusted down to 512 KiB, and might need to be adjusted in the future for
different app configurations.

### Logging and Debug Messages

The cFS logging system includes several different methods for writing to the
console. Some are synchronous, some are asynchronous. Either way, RTEMS has a
bad habit of mangling messages printed using anything other than a simple
`printf`. This makes them unreadable. The most important of these messages have
been changed in the code, but if you see output that looks off, it probably is.
You'll have to track down the offending print statement and swap it out for a
`printf`.
