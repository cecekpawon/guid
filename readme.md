# guid

> Original source: https://github.com/firmwhere/guid **(current mod for personal use only)**
MinGW compatible libuuid : https://github.com/cloudbase/libuuid

```
guid (mod) - generate guid for uefi development. @efikarl@yeah.net

options:
    -g, --guid <guid>      generate guid from <guid>

flags:
    -h, --help             output help info
    -l, --lowercase        output lowercase result
    -s, --standard         output guid standard text result only
```

## build

```sh
# macos
$ clang guid.c -oguid
# or
$ gcc   guid.c -oguid

# linux
$ clang guid.c -oguid -luuid
# or
$ gcc   guid.c -oguid -luuid

# windows
$ clang guid.c -oguid.exe -luuid
# or
$ gcc   guid.c -oguid.exe -luuid
```

## examples

```
$ ./guid

[[ DSC, DEC, INF ]]

BE21692E-AC35-11EB-AA1F-1FEA6618DD97

[[ DEC ]]

## Include/Pkg.h
gNameGuid = { 0xBE21692E, 0xAC35, 0x11EB, { 0xAA, 0x1F, 0x1F, 0xEA, 0x66, 0x18, 0xDD, 0x97 } }

[[ HEADER ]]

#define NAME_GUID \
  { 0xBE21692E, 0xAC35, 0x11EB, { 0xAA, 0x1F, 0x1F, 0xEA, 0x66, 0x18, 0xDD, 0x97 } }

extern NAME_GUID gNameGuid;
```
