/**
 * VoidExtractor - universal game archive unpacker
 *
 * Copyright (C) 2025  valsaven
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#include "vfs_v1.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#include <unistd.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

/* Creates all subfolders along a path (similar to os.MkdirAll) */
static int mkdir_recursive(const char *path) {
    if (!path || !*path) {
        return 0;
    }

    char tmp[1024];
    strncpy(tmp, path, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    char *p = tmp;
    /* Skip ./ or C:\ */
    if (tmp[0] == '.' && (tmp[1] == '\\' || tmp[1] == '/')) {
        p += 2;
    } else if (tmp[1] == ':') {
        p += 3;
    }

    while ((p = strchr(p, '\\')) != NULL) {
        *p = '\0';
        if (MKDIR(tmp) == -1 && errno != EEXIST) {
            return -1;
        }
        *p++ = '\\'; // use \ under Windows
    }

    return (MKDIR(tmp) == -1 && errno != EEXIST) ? -1 : 0;
}

// Main function for extracting from vfs v1
ExtractorResult vfs_v1_extract(const char *input_path, const char *output_dir) {
    FILE *f = fopen(input_path, "rb");
    if (!f) {
        fprintf(stderr, "Error: cannot open file '%s'\n", input_path);
        return EXTRACTOR_ERROR_OPEN;
    }

    // Checking the size
    fseek(f, 0, SEEK_END);
    long vfs_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (vfs_size < 12) {
        fclose(f);
        return EXTRACTOR_ERROR_CORRUPTED;
    }

    /* === Read the header === */
    uint8_t magic[4];
    if (fread(magic, 1, 4, f) != 4 || memcmp(magic, "LP1C", 4) != 0) {
        fclose(f);
        fprintf(stderr, "Error: Invalid signature (expected LP1C)\n");
        return EXTRACTOR_ERROR_UNSUPPORTED;
    }

    uint8_t version[4];
    if (fread(version, 1, 4, f) != 4 || memcmp(version, "\0\0\0\0", 4) != 0) {
        fclose(f);
        fprintf(stderr, "Error: Unsupported VFS version (expected 0.0.0.0)\n");
        return EXTRACTOR_ERROR_UNSUPPORTED;
    }

    uint32_t file_count = 0;
    if (fread(&file_count, sizeof(uint32_t), 1, f) != 1) {
        fclose(f);
        return EXTRACTOR_ERROR_IO;
    }

    printf("VFS v1 detected: %u files\n", file_count);

    if (file_count == 0) {
        printf("No files to extract.\n");
        fclose(f);
        return EXTRACTOR_SUCCESS;
    }

    // Create output folder
    if (mkdir_recursive(output_dir) != 0) {
        fprintf(stderr, "Error: cannot create directory '%s'\n", output_dir);
        fclose(f);
        return EXTRACTOR_ERROR_IO;
    }

    /* === Extract files === */
    for (uint32_t i = 0; i < file_count; ++i) {
        long entry_start = ftell(f);

        // 1. Name length
        uint8_t name_len;
        if (fread(&name_len, 1, 1, f) != 1 || name_len == 0 || name_len > 250) {
            fclose(f);
            fprintf(stderr, "Error writing %u: invalid name length\n", i + 1);
            return EXTRACTOR_ERROR_CORRUPTED;
        }

        // 2. File name
        char name[256] = {0};
        if (fread(name, 1, name_len, f) != name_len) {
            fclose(f);
            return EXTRACTOR_ERROR_IO;
        }

        // Path normalization: '/' → '\'
        for (int j = 0; j < name_len; ++j) {
            if (name[j] == '/') {
                name[j] = '\\';
            }
        }

        // 3. Size and offset
        uint32_t size, offset;
        if (fread(&size, 4, 1, f) != 1 || fread(&offset, 4, 1, f) != 1) {
            fclose(f);
            return EXTRACTOR_ERROR_IO;
        }

        // Skip 8 bytes of padding
        fseek(f, 8, SEEK_CUR);

        // Bounds check
        if (offset + size > (uint32_t)vfs_size) {
            fclose(f);
            fprintf(stderr, "Error writing file '%s': data exceeds archive bounds\n", name);
            return EXTRACTOR_ERROR_CORRUPTED;
        }

        //  === Extract  ===
        char out_path[1024];
        snprintf(out_path, sizeof(out_path), "%s\\%s", output_dir, name);

        // Create subfolders
        char *slash = strrchr(out_path, '\\');
        if (slash) {
            *slash = '\0';
            mkdir_recursive(out_path);
            *slash = '\\';
        }

        FILE *out = fopen(out_path, "wb");
        if (!out) {
            fprintf(stderr, "Failed to create file: %s\n", out_path);
            fclose(f);
            return EXTRACTOR_ERROR_IO;
        }

        fseek(f, (long)offset, SEEK_SET);

        uint8_t *buffer = malloc(size);
        if (!buffer) {
            fclose(out);
            fclose(f);
            return EXTRACTOR_ERROR_NOMEM;
        }

        fread(buffer, 1, size, f);
        fwrite(buffer, 1, size, out);
        free(buffer);
        fclose(out);

        printf("Extracted (%u/%u): %s (%u bytes)\n", i+1, file_count, name, size);

        // ← RETURN TO NEXT ENTRY
        long next_entry_pos = entry_start + 1 + name_len + 4 + 4 + 8;  // 1 (len) + name_len + size(4) + offset(4) + padding(8)
        fseek(f, next_entry_pos, SEEK_SET);
    }

    fclose(f);
    printf("Unpacking VFS v1 completed successfully.\n");

    return EXTRACTOR_SUCCESS;
}
