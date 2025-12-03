/**
 * VoidExtractor - universal game archive unpacker
 *
 * Copyright (C) 2025  valsaven
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include "../extractor.h"

/**
 * Unpacks VFS v1 archives - version with LP1C + 0x00000000
 *
 * Examples of games:
 *   - Pathologic Classic HD (2015)
 */
ExtractorResult vfs_v1_extract(const char *input_path, const char *output_dir);
