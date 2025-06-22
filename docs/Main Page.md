# EZ Text Replacer for Zelda64Recompiled {#mainpage}

EZTR is a complete in-game text, message, and dialog suite for Zelda64Recompiled modding. With it, you can:

* Replace any vanilla game dialog, item description, or other on-screen message.
* Add your own brand-new message entries for custom NPCs.
* Dynamically generate message content at runtime, using callbacks.

And more!

## Basic Replacements

Replacing dialog is straightforward.

In your mod code, create a function with the following signature:

```C
// This function can be named whatever you want.
EZTR_ON_INIT void my_initialization_function() { ... }
```

Inside this function, you will define each text replacement you want to make using EZTR's [Basic_Replacement](@ref Basic_Replacement) API. For example:

```C
#include "eztr_api.h"

EZTR_ON_INIT void init_text() {
    EZTR_Basic_ReplaceText(
        0x061B,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "Hey, apprentice! Don't just stand" EZTR_CC_NEWLINE "around lookin' at the sky all" EZTR_CC_NEWLINE "day!" EZTR_CC_EVENT2 EZTR_CC_END,
        NULL
    );
}
```

Messages can also be dynamically generated at runtime based on code. For example:

```C
// Callback used to set Bomb Shop owner dialog.
EZTR_MSG_CALLBACK(bombshop_nonhuman_callback) {
    Player* player = GET_PLAYER(play);
    if (player->transformation != PLAYER_FORM_DEKU) {
        buf->data.text_box_type = EZTR_STANDARD_TEXT_BOX_II;
        EZTR_MsgSContent_Sprintf(buf->data.content, "...Welcome." EZTR_CC_EVENT EZTR_CC_END);
    } else {
        // Vanilla Dialogue:
        EZTR_MsgSContent_Sprintf(buf->data.content, "You can't use any of them, but" EZTR_CC_NEWLINE "feel free to look around." EZTR_CC_EVENT EZTR_CC_END);
    }
}

EZTR_ON_INIT void init_text() {
    // Replacements for Bomb Shop
    EZTR_Basic_ReplaceText(
        0x064B,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        "\xBF",
        bombshop_nonhuman_callback
    );
}
```

See [Message Buffers and Callbacks](@ref message_buffer_and_callbacks) to learn more.

You can add custom messages through a similar process. For example:

```C
EZTR_DEFINE_CUSTOM_MSG_HANDLE(my_custom_message_handle);

EZTR_ON_INIT void init_text() {

    EZTR_Basic_AddCustomText(
        EZTR_HNAME(external_custom_message_handle),
        EZTR_STANDARD_TEXT_BOX_I, 
        0, 
        EZTR_ICON_NO_ICON, 
        EZTR_NO_VALUE, 
        EZTR_NO_VALUE, 
        EZTR_NO_VALUE, 
        false, 
        "HELLO WORLD AGAIN!!\xBF", 
        NULL
    );
}
```

Check out the [Custom Messages and Handles](@ref custom_messages_and_handles) page to learn more.

## Dumping Messages

To easily access the raw message data needed for replacements, go to EZTR's configuration page and enable `Text Dumping`.

![./eztr_config_page.png](./eztr_config_page.png)

When enabled, EZTR prints message data to the Zelda64Recompiled console whenever a message is loaded/displayed. There are also a couple of formatting options provided:

* `Dump Text as C Code` - When enabled, text dumps will be formatted as calls to EZTR’s text-replacement API.
* `Text Dumping Byte Format` - How non-printable bytes are dumped, with `_` representing hexadecimal digits. See the section on [pipe-escaped bytes](@ref pipe_escaped_bytes) for information on the `Pipes` option.
* Print the EZTR control code macro from [Control_Code_Macros](@ref Control_Code_Macros) (when available) when dumping a non-printable byte.

Additionally, you may want to check out the [EZTR Dump To Disk Extension](https://thunderstore.io/c/zelda-64-recompiled/p/LT_Schmiddy/EZTR_Dump_To_Disk_Extention/), which can generate a preformatted C file with all the messages you've dumped.

## More Information

The complete `eztr_api.h` header is documented on this site. For more information, visit the various topic pages for each part of the API:

* [General](@ref General) - General API Information
* [CustomMsgHandle](@ref CustomMsgHandle) - Macros and types used for working with the handles for custom messages.
* [Types](@ref Types) - Type definitions that EZTR uses for Majora's Mask messages.
* [Control_Code_Macros](@ref Control_Code_Macros) - Macros for the various control codes and non-printable bytes used in the Majora's Mask text encoding.
* [Basic_Replacement](@ref Basic_Replacement) - Functions for replacing vanilla messages.
* [Basic_CustomMessages](@ref Basic_CustomMessages) - Functions for creating and replacing new messages.
* [MsgBuffer](@ref MsgBuffer) - Functions for high-level message buffer operations.
* [MsgSContent](@ref MsgSContent) - Functions for manipulating message text content.
