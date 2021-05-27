# Stereopticon
A Linux guest-side compositor that transfers over Wayland window contents to a host system, similar to WSLg.


### Build the source
Building the source code is done automatically by the `build.sh` script.

Depending on whether you want to build the guest or the host side of the project, just add the corresponding `--guest` or `--host` argument.

For first time builds please run the build script using the `-d` argument, for example:

```
./build.sh -d --guest
```

This builds the source tree including dependencies. If building dependencies is not requested you can simply omit `-d` from the command.

```
./build.sh --guest
```
