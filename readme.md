## guid

```shell
guid - generate guid for uefi development. @efikarl@yeah.net

options:
    -g, --guid <guid>      generate guid from <guid>

flags:
    -h  --help             output help info
    -u, --upper            output uppercase result, or lowercase
```

### build

```shell
# macos
$ clang guid.c -oguid
# or
$ gcc   guid.c -oguid

# linux
$ clang guid.c -oguid -luuid
# or
$ gcc   guid.c -oguid -luuid

# windows
$ clang guid.c -oguid.exe -luuid_mingw -lrpcrt4
# or
$ gcc   guid.c -oguid.exe -luuid_mingw -lrpcrt4
```

### examples

```shell
$ ./guid -g 00112233-4455-6677-8899-AABBCCDDEEFF

[guid.std.text] 33221100-5544-7766-8899-aabbccddeeff
  [uefi.memmap] 00112233-4455-6677-8899-aabbccddeeff
  [ipmi.memmap] aabbccddeeff-8899-6677-4455-00112233

[uefi.guid.struct]
{ 0x33221100, 0x5544, 0x7766, { 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff } }

[uefi.guid.define]
#define GUID \
  { \
    0x33221100, 0x5544, 0x7766, { 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff } \
  }
extern EFI_GUID gGuid;

$ ./guid --guid 00112233-4455-6677-8899-AABBCCDDEEFF

[guid.std.text] 33221100-5544-7766-8899-aabbccddeeff
  [uefi.memmap] 00112233-4455-6677-8899-aabbccddeeff
  [ipmi.memmap] aabbccddeeff-8899-6677-4455-00112233

[uefi.guid.struct]
{ 0x33221100, 0x5544, 0x7766, { 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff } }

[uefi.guid.define]
#define GUID \
  { \
    0x33221100, 0x5544, 0x7766, { 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff } \
  }
extern EFI_GUID gGuid;

```

```shell
$ ./guid

[guid.std.text] 6bf30348-d2e4-1744-81a1-53c3d00b690b
  [uefi.memmap] 4803f36b-e4d2-4417-81a1-53c3d00b690b
  [ipmi.memmap] 53c3d00b690b-81a1-4417-e4d2-4803f36b

[uefi.guid.struct]
{ 0x6bf30348, 0xd2e4, 0x1744, { 0x81, 0xa1, 0x53, 0xc3, 0xd0, 0x0b, 0x69, 0x0b } }

[uefi.guid.define]
#define GUID \
  { \
    0x6bf30348, 0xd2e4, 0x1744, { 0x81, 0xa1, 0x53, 0xc3, 0xd0, 0x0b, 0x69, 0x0b } \
  }
extern EFI_GUID gGuid;

$ ./guid --upper

[guid.std.text] C02AE3B1-DB8F-7B42-A4ED-9F57714B705F
  [uefi.memmap] B1E32AC0-8FDB-427B-A4ED-9F57714B705F
  [ipmi.memmap] 9F57714B705F-A4ED-427B-8FDB-B1E32AC0

[uefi.guid.struct]
{ 0xC02AE3B1, 0xDB8F, 0x7B42, { 0xA4, 0xED, 0x9F, 0x57, 0x71, 0x4B, 0x70, 0x5F } }

[uefi.guid.define]
#define GUID \
  { \
    0xC02AE3B1, 0xDB8F, 0x7B42, { 0xA4, 0xED, 0x9F, 0x57, 0x71, 0x4B, 0x70, 0x5F } \
  }
extern EFI_GUID gGuid;

```
