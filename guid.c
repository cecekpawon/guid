/*++ @file

  Copyright ©2021 Liu Yi, efikarl@yeah.net

  This program is just made available under the terms and conditions of the
  MIT license: http://www.efikarl.com/mit-license.html

  THE PROGRAM IS DISTRIBUTED UNDER THE MIT LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
--*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <getopt.h>
#include <uuid/uuid.h>

#ifdef __GNUC__
#include <stdint.h>
#endif

#define UEFI_GUID_STD_TXT_SIZE      0x25
#define UEFI_GUID_STD_MEMBER_SIZE   0x50
#define UEFI_GUID_STR_MAX_SIZE      0x96
#define UEFI_GUID_NAME_DEFINED      "NAME_GUID"
#define UEFI_GUID_NAME_STR          "gNameGuid"
#define UEFI_HEADER_NAME_STR        "## Include/Pkg.h"
#define UEFI_GUID_FORMAT_STD        "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x"
#define UEFI_GUID_FORMAT_HEADER     "0x%02x%02x%02x%02x, 0x%02x%02x, 0x%02x%02x, { 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x }"

bool is_guid_stdtxt_format(char *string) {
    if ((string == NULL) || (strlen(string) != (UEFI_GUID_STD_TXT_SIZE - 1))) {
        printf("ERR: invalid arguments in %s\n", __func__);
        return false;
    }

    for (int i = 0; i < UEFI_GUID_STD_TXT_SIZE - 1; ++i) {
        switch (i) {
            case 8: case 13: case 18: case 23:
                if (string[i] != '-') {
                    return false;
                }
                break;
            default:
                if (!isxdigit(string[i])) {
                    return false;
                }
        }
    }

    return true;
}

void guid_hex_to_case(char *string, bool lower) {
    if (string == NULL) {
        printf("ERR: invalid arguments in %s\n", __func__);
        return;
    }

    for (int i = 0; string[i] != '\0'; i++) {
        if (lower) {
            if (string[i] >= 'A' && string[i] <= 'F') {
                string[i] = tolower(string[i]);
            }
        } else {
            if (string[i] >= 'a' && string[i] <= 'f') {
                string[i] = toupper(string[i]);
            }
        }
    }
}

void uefi_guid_stdtxt_unparse(const char *format, const uuid_t uuid, char *string, bool lower)
{
    if ((format == NULL) || (string == NULL)) {
        printf("ERR: invalid arguments in %s\n", __func__);
        return;
    }

    sprintf(string,
        format,
        uuid[0], uuid[1], uuid[2], uuid[3],
        uuid[4], uuid[5],
        uuid[6], uuid[7],
        uuid[8], uuid[9], uuid[10], uuid[11], uuid[12], uuid[13], uuid[14], uuid[15]
    );

    guid_hex_to_case(string, lower);
}

void uefi_guid_struct_unparse(const uuid_t uuid, char *string, bool lower)
{
    char string_member[UEFI_GUID_STD_MEMBER_SIZE];

    uefi_guid_stdtxt_unparse(UEFI_GUID_FORMAT_HEADER, uuid, string_member, lower);

    sprintf(string, "{ %s }", string_member);
}

void uefi_guid_define_unparse(const uuid_t uuid, char *string, bool lower)
{
    char string_member[UEFI_GUID_STD_MEMBER_SIZE];

    uefi_guid_stdtxt_unparse(UEFI_GUID_FORMAT_HEADER, uuid, string_member, lower);

    sprintf(string,
        "#define %s \\\n"
        "  { %s }\n\n"
        "extern %s %s;",
    UEFI_GUID_NAME_DEFINED, string_member, UEFI_GUID_NAME_DEFINED, UEFI_GUID_NAME_STR);
}

typedef struct {
    bool                flag;
    union {
        uint64_t        number;
        char           *string;
    }                   value;
} argument_t;

typedef struct {
    argument_t          guid;
    bool                lowercase;
    bool                standard;
} arguments_t;

void help()
{
    printf(
        "\n"
        "guid (mod) - generate guid for uefi development. @efikarl@yeah.net\n"
        "\n"
        "options:\n"
        "    -g, --guid <guid>      generate guid from <guid>\n"
        "\n"
        "flags:\n"
        "    -h, --help             output help info\n"
        "    -l, --lowercase        output lowercase result\n"
        "    -s, --standard         output guid standard text result only\n"
        "\n"
        );
}

typedef struct {
  uint32_t  data1;
  uint16_t  data2;
  uint16_t  data3;
  uint8_t   data4[8];
} efi_guid_t;

int arguments_init(int argc, char **argv, arguments_t *options)
{
    int                 opt;
    int                 option_index = 0;
    struct option       long_options[] = {
        { "guid"      , 1, 0, 'g' },
        { "lowercase" , 0, 0, 'l' },
        { "standard"  , 0, 0, 's' },
        { "help"      , 0, 0, 'h' }
    };

    while ((opt = getopt_long(argc, argv, "g:lsh", long_options, &option_index)) != EOF) {
        switch (opt) {
        case 'g':
            if (!is_guid_stdtxt_format(optarg)) {
                printf("ERR: invalid guid format: %s\n", optarg);
                return false;
            }
            options->guid.value.string  = optarg;
            options->guid.flag          = true;
            break;
        case 'l':
            options->lowercase = true;
            break;
        case 's':
            options->standard  = true;
            break;
        default:
            return false;
        }
    }

    return true;
}

typedef struct {
    char guid_stdtxt[UEFI_GUID_STD_TXT_SIZE];
    char uefi_guid_struct_string[UEFI_GUID_STR_MAX_SIZE];
    char uefi_guid_define_string[UEFI_GUID_STR_MAX_SIZE];
} guids_t;

void guid_result_output_handler(
    const uuid_t        uuid,
    const arguments_t   options
    )
{
    guids_t guids = { 0 };
    uefi_guid_stdtxt_unparse(UEFI_GUID_FORMAT_STD, uuid, guids.guid_stdtxt, options.lowercase);
    if (options.standard) {
        printf("%s\n", guids.guid_stdtxt);
        return;
    };
    uefi_guid_struct_unparse(uuid, guids.uefi_guid_struct_string, options.lowercase);
    uefi_guid_define_unparse(uuid, guids.uefi_guid_define_string, options.lowercase);
    printf(
        "\n"
        "[[ DSC, DEC, INF ]]\n\n"
        "%s\n\n"
        "[[ DEC ]]\n\n"
        "%s\n"
        "%s = %s\n\n"
        "[[ HEADER ]]\n\n"
        "%s\n\n",
        guids.guid_stdtxt, UEFI_HEADER_NAME_STR, UEFI_GUID_NAME_STR, guids.uefi_guid_struct_string, guids.uefi_guid_define_string
    );
}

int main(int argc, char **argv)
{
    arguments_t     options = { 0 };
    if (!arguments_init(argc, argv, &options)) {
        help();
        return 1;
    }

    uuid_t          uuid    = { 0 };

    if (options.guid.flag) {
        uuid_parse(options.guid.value.string, uuid);
    } else {
        uuid_generate(uuid);
    }

    guid_result_output_handler(uuid, options);

    return 0;
}
