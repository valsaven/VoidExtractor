# VoidExtractor - universal game archive unpacker
#
# Copyright (C) 2025 valsaven
#
# SPDX-License-Identifier: GPL-3.0-or-later
#
# This file is part of VoidExtractor.
# See LICENSE file for full copyright and licensing details.

project(
    'VoidExtractor',
    'c',
    version: '1.0.0',
    license: 'GPLv3',
    default_options: [
        'warning_level=3',
        'werror=true',          # Treat all warnings as errors
        'c_std=c2x',            # Use latest C standard
        'b_ndebug=if-release',  # Strip assert() in release builds
    ],
)

# All source files from src/
src = [
    'src/main.c',
    'src/detect.c',

    # VFS
    'src/vfs/vfs_v1.c',
    'src/vfs/vfs_v2.c',
]

# Build the executable
VoidExtractor = executable(
    'VoidExtractor',
    src,
    install: true,

    c_args : [
        '-Wall',
        '-Wextra',
        '-Wpedantic',
        '-Wshadow',
        '-Wconversion',
        '-Wsign-conversion',
        '-Wnull-dereference',
        '-Wdouble-promotion',
        '-Wformat=2',
        '-Wformat-overflow=2',
        '-Wformat-truncation=2',
        '-Wundef',
        '-Wswitch-default',
        '-Wswitch-enum',

        # Security hardening (requires -O1 or higher)
        '-D_FORTIFY_SOURCE=2',
        '-fstack-protector-strong',

        # Optimization
        '-O2',              # Or -O3 for even more speed if desired
        # '-march=native',  # Uncomment if building only for this machine
    ],

    link_args : [
        '-s',                # Strip debugging symbols for smaller binary
        '-Wl,--gc-sections', # Remove unused code and data
    ]
)

# Basic smoke test - just runs the binary
test('smoke test', VoidExtractor)
