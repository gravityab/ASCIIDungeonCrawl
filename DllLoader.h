///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// DllLoader.h
// Copyright (C) 2025 Andrew Bellinger (gravityab@gmail.com)
//
// Resource-extraction helpers for the self-contained build:
//   * LoadEmbeddedBearLibTerminal() pulls RCDATA #1001 (BearLibTerminal.dll) out of the .exe,
//     writes it next to the .exe, and LoadLibrary's it. Must run before any BearLibTerminal call
//     (the imports are delay-loaded, so the delay-load thunk picks up our cached module).
//   * ExtractEmbeddedFont() pulls RCDATA #1002 (Consolas-Regular.ttf) out of the .exe and writes
//     it to <exe-dir>/Resources/Consolas-Regular.ttf so terminal_set("font: ...") can find it
//     without us shipping the file separately.
//
// Both functions return true on success. On failure *outError (if non-null) is filled with a
// short human-readable string describing which step failed.

#pragma once

bool LoadEmbeddedBearLibTerminal(const char** outError = nullptr);
bool ExtractEmbeddedFont(const char** outError = nullptr);
