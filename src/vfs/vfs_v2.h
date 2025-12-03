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
 * Unpacks VFS v2 archives - version with LP2C
 *
 * Examples of games:
 *   - The Void (2016)
 */
ExtractorResult vfs_v2_extract(const char *input_path, const char *output_dir);
