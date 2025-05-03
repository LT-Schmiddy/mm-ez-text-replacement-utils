# Changelog

## 2.0.1

* No code changes. Removes version numbers from mod filename.

## v2.0.0

Added:

* Dynamically generates messages using a callback system.
* Support for adding new message entries. Use EZTR to quickly implement additional dialog for your mods.
* Tools and functions for and generating manipulating message strings, including a specialized version of the `printf` family of functions.
* (BREAKING) All replacement functions now include a field for callback. Set to NULL if no callback is used.

## 1.1.0

Initial Public Release
Only supports basic replacements.
