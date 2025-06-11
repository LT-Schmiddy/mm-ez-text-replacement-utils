# Changelog

## 2.1.0

* All `EZTR_Basic_*` calls are required to be made during the `EZTR_ON_INIT` event.
* Added the ability to dump text in the format of an `EZTR_Basic_ReplaceText` function call.
* Improved the `\x__` byte formatting options to be usable without compilation issues.
* Non-printable bytes can now be dumped to use the corresponding `EZTR_CC_*` macro (when available).
* Added the `EZTR_ON_DUMP_BUFFER` and `EZTR_ON_DUMP_BUFFER_FULL` events.
* Adding a `Full` dumping option.

## 2.0.1

* Fixed a memory leak when replacing a message that already has an entry in the MsgTable.
* Removes version numbers from mod filename.

## v2.0.0

* Adds dynamically generates messages using a callback system.
* Adds Support for adding new message entries. Use EZTR to quickly implement additional dialog for your mods.
* Adds Tools and functions for and generating manipulating message strings, including a specialized version of the `printf` family of functions.
* (BREAKING) All replacement functions now include a field for callback. Set to NULL if no callback is used.

## 1.1.0

* Initial Public Release
* Only supports basic replacements.
