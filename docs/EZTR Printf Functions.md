# EZTR Printf Functions {#prinf_functions}

Because the message content are terminated.

## Differences from Standard `[s[n]]printf`

There are a few major differences to note between the standard `printf` functions and EZTR's implementations:

* \ref bf_termination
* \ref pipe_escaped_bytes
* \ref m_type_specifier
* \ref non_printable_bytes

### `\xBF` Termination {#bf_termination}

Message strings do not use the `\0` null character to terminate, as is done in standard ASCII. Instead, strings are terminated with `\xBF`, and `\0`
is used as the control code to set the text color back to normal. As a result of this, EZTR cannot assume your string arguments will be `\0` terminated either.
It is expected that any message data argument of a `EZTR_MsgSContent_*` function will manually terminated with `\xBF` by the programmer.

See \ref the_message_buffer for more information on message control codes and massage termination.

### Pipe-Escaped Byte Syntax {#pipe_escaped_bytes}

Because of the non-standard encoding that Majora's Mask uses for control characters, EZTR offers an optional special syntax for handling these in the form of
"Pipe-escaped bytes". When used, single bytes can be written in your string using the syntax `|XX`, where each `X` is a hexidecimal character. Note that this
only does a single byte at a time, so the correct way to write 2 bytes would be `|XX|XX` and not `|XXXX` (unlike the `\x` escape sequence used by C, where `\xXXXX` would be a valid expression of two bytes). This is a useful feature if you need to load message data from an external, ASCII or Unicode-based source.

Pipe-escaped bytes are used in 3 places:

* The `format` argument of any of the normal `EZTR_MsgSContent_*Printf` functions (the ones with `NoPipe` in the name are alternate versions without this behavior).
* Passing message content to any `EZTR_MsgSContent_*Printf` (including the `NoPipe` versions) using the `%m` type specifier.
* The content `content` argument of `EZTR_Basic_ReplaceText`, `EZTR_Basic_AddCustomText`, and `EZTR_Basic_ReplaceCustomText`, provided the `pipe_escaped_bytes` argument is set to true.

Note that this feature is entirely optional. EZTR is fully capable of processing control code bytes directly (such as using C escape sequences in string literals).

### The `%%m` and `%%n` Type Specifiers {#m_type_specifier}

Because the `%s` string specifier in C expects the argument to be `\0` terminated, the specifier cannot be easily used to pass in control codes and
formatting data. To solve this, `EZTR_MsgSContent_*Printf` have two unique specifiers: `%%m` and `%%n`. These specifiers function identically to `%s`
except they assume that the corresponding argument is Majora's Mask message data and thus `\xBF` terminated.

That is to say, when using `%%m` and `%%n` to pass in message data, terminate your argument with `\xBF`. This argument termination will not be copied into the output string
(much like how `*printf` doesn't copy in the `\0` null terminator from `%s` arguments).

The only difference between `%%m` and `%%n` is that `%%m` processes pipe-escaped bytes, whereas `%%n` does not.

### Non-Printable Bytes {#non_printable_bytes}

When printing to the console using `EZTR_MsgSContent_Printf`, `EZTR_MsgSContent_PrintfLine`, or the `NoPipe` variants thereof, all control codes are displayed according to EZTR's `text_dumping_byte_format` and `text_dumping_cc_macros` options. This does not apply to any other printf functions, which preserves the control bytes.

## Format Specification

A format specifier follows this prototype: `%[flags][width][.precision][length]type`
The following format specifiers are supported:

### Supported Types

| Type   | Output |
|--------|--------|
| d or i | Signed decimal integer |
| u      | Unsigned decimal integer |
| b      | Unsigned binary |
| o      | Unsigned octal |
| x      | Unsigned hexadecimal integer (lowercase) |
| X      | Unsigned hexadecimal integer (uppercase) |
| f or F | Decimal floating point |
| e or E | Scientific-notation (exponential) floating point |
| g or G | Scientific or decimal floating point |
| c      | Single character |
| s      | ASCII string of characters. To offer parity with standard C, it is assumed that this string is null-terminated. |
| S      | ASCII string of characters. Like `%s`, it is assumed that this string is null-terminated, but pipe-escaped bytes will still be evaluated. |
| m      | Message content string using pipe-escaped bytes. This is a unique type implemented by EZTR. |
| M      | Message content string NOT using pipe-escaped bytes. This is a unique type implemented by EZTR. |
| w      | Unsigned 16-bit value to be inserted into the byte string raw. Useful for pass argument bytes following `EZTR_CC_SFX`, `EZTR_CC_DELAY`, etc. |
| p      | Pointer address |
| %      | A % followed by another % character will write a single % |

### Supported Flags

| Flags | Description |
|-------|-------------|
| -     | Left-justify within the given field width; Right justification is the default. |
| +     | Forces to precede the result with a plus or minus sign (+ or -) even for positive numbers. By default, only negative numbers are preceded with a - sign. |
| (space) | If no sign is going to be written, a blank space is inserted before the value. |
| #     | Used with o, b, x or X specifiers the value is preceded with 0, 0b, 0x or 0X respectively for values different than zero.Used with f, F it forces the written output to contain a decimal point even if no more digits follow. By default, if no digits follow, no decimal point is written. |
| 0     | Left-pads the number with zeros (0) instead of spaces when padding is specified (see width sub-specifier). |

### Supported Width

| Width    | Description |
|----------|-------------|
| (number) | Minimum number of characters to be printed. If the value to be printed is shorter than this number, the result is padded with blank spaces. The value is not truncated even if the result is larger. |
| *        | The width is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted. |

### Supported Precision

| Precision | Description |
|-----------|-------------|
| .number   | For integer specifiers (d, i, o, u, x, X): precision specifies the minimum number of digits to be written. If the value to be written is shorter than this number, the result is padded with leading zeros. The value is not truncated even if the result is longer. A precision of 0 means that no character is written for the value 0.For f and F specifiers: this is the number of digits to be printed after the decimal point. **By default, this is 6, maximum is 9**.For s: this is the maximum number of characters to be printed. By default all characters are printed until the ending null character is encountered.If the period is specified without an explicit value for precision, 0 is assumed. |
| .*        | The precision is not specified in the format string, but as an additional integer value argument preceding the argument that has to be formatted. |

### Supported Length

The length sub-specifier modifies the length of the data type.

| Length | d i  | u o x X |
|--------|------|---------|
| (none) | int  | unsigned int |
| hh     | char | unsigned char |
| h      | short int | unsigned short int |
| l      | long int | unsigned long int |
| ll     | long long int | unsigned long long int (if PRINTF_SUPPORT_LONG_LONG is defined) |
| j      | intmax_t | uintmax_t |
| z      | size_t | size_t |
| t      | ptrdiff_t | ptrdiff_t (if PRINTF_SUPPORT_PTRDIFF_T is defined) |

### Return Value

Upon successful return, all functions return the number of characters written, _excluding_ the terminating null character used to end the string.
Functions `snprintf()` and `vsnprintf()` don't write more than `count` bytes, _including_ the terminating null byte ('\xBF').
Anyway, if the output was truncated due to this limit, the return value is the number of characters that _could_ have been written.
Notice that a value equal or larger than `count` indicates a truncation. Only when the returned value is non-negative and less than `count`,
the string has been completely written.
If any error is encountered, `-1` is returned.

If `buffer` is set to `NULL` (`nullptr`) nothing is written and just the formatted length is returned.

```C
int length = sprintf(NULL, "Hello, world"); // length is set to 12
```

## Credits

Special thanks to [mpaland on GitHub](https://github.com/mpaland/) for creating this [embedded printf library](https://github.com/mpaland/printf), as EZTR's printf logic is a modified version of this project. The documentation on this page is partially copied from there as well.

([https://github.com/mpaland/printf](https://github.com/mpaland/printf))
