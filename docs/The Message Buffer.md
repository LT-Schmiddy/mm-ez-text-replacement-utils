# The Message Buffer {#the_message_buffer}

This document provides high-level information about text handling in Majora's Mask and the text manipulation tools provided by EZTR. This is not meant to be an in-depth guide to everything related to EZTR and Majora's Mask text, but a quick overview to get you started with EZTR.

If you want a more in-depth look at Majora's Mask text handling, I recommend this page: [https://wiki.cloudmodding.com/mm/Text_Format](https://wiki.cloudmodding.com/mm/Text_Format)

## Text in Majora's Mask

EZTR makes no changes to how the messages and text data in Majora's Mask are structured, stored, and processed. Therefore, any information relevant to text in vanilla Majora's Mask should be applicable to EZTR.

In brief, the first 11 bytes of a message are a header, with the content of the message itself following after. The header is laid out in the following way:

* (1 byte) The text box type/style. [EZTR_TextBoxType](@ref EZTR_TextBoxType) provides readable names for each textbox type.
* (1 byte) The text box's Y position on screen.
* (1 byte) What icon (if any) to display with the text. [EZTR_TextBoxIcon](@ref EZTR_TextBoxIcon) provides readable names for each textbox UI icon.
* (2 bytes) The ID of the next message to display. If there is no next message, or the next message is determined by code, use `0xFFFF` or the `EZTR_NO_VALUE` macro.
* (2 bytes) The cost of the first item being offered for sale, in rupees. If there is no item, use `0xFFFF` or the `EZTR_NO_VALUE` macro.
* (2 bytes) The cost of the second item being offered for sale, in rupees. If there is no item, use `0xFFFF` or the `EZTR_NO_VALUE` macro.
* 2 bytes of padding.

All data after the header is considered part of the message content.

The text encoding of the message content is non-standard: while all the printable characters are standard ASCII, the control codes are not. Of particular note, the termination character is `\xBF` rather than `\x00` (also written as `\0`). Most of the control codes are documented on the [Control_Code_Macros](@ref Control_Code_Macros) page.

The header and content regions combined cannot be more than 1280 bytes, as that is the maximum size of the vanilla buffer. Since the header is 11 bytes, that leaves 1269 bytes for the content region.

## When to Work with Message Buffer

Depending on what you're doing, you may not need to work with message buffers directly. For example, while EZTR does store messages as preconstructed message buffers, the functions [EZTR_Basic_ReplaceText](@ref EZTR_Basic_ReplaceText), [EZTR_Basic_AddCustomText](@ref EZTR_Basic_AddCustomText), and [EZTR_Basic_ReplaceCustomText](@ref EZTR_Basic_ReplaceCustomText) all construct and store the message buffer for you. The only consideration you'll need to make is the maximum size of the `content` variable in each case, since that region cannot be larger than 1269 bytes.

Some cases where you'd work with the message buffer directly include:

* Dynamically generating messages using EZTR's message callback system.
* Loading custom messages from some external source.
* Simple text processing using Majora's Mask's encoding.

## Working with the Message Buffer

To begin with, EZTR uses the type [EZTR_MsgBuffer](@ref EZTR_MsgBuffer) to represent the Majora's Mask message buffer. It's a union between a couple different ways to represent the message buffer information. Of particular note is the `data` member, which lets you assign to each part of the header as well as the content region by treating the buffer as a packed struct. Read the type's own documentation for more details.

EZTR also provides many functions for operating on message buffers (too many to list here). These functions are broken up into two categories:

[MsgBuffer](@ref MsgBuffer) provides general operations such as creating and destroying buffers, copying from one buffer to another, and getting the size of the data on the buffer. It also provides getters and setters for the buffer's header data (as an alternative to the packed struct).

[MsgSContent](@ref MsgSContent) provides string operations designed to work with the unique encoding of Majora's Mask message data. It includes reimplemented versions of standard C functions such as `str[n]cpy`, `str[n]cmp`, `str[n]len`, `str[n]cat`, and even `[s[n]]printf`. Note that these functions are designed to operate on any `\xBF`-terminated `char` array, not just the content region of message buffers. As a result, if you want to operate on a message buffer's content, you'll need a pointer to the content region. You can get this using the [EZTR_MsgBuffer_GetContentPtr](@ref EZTR_MsgBuffer_GetContentPtr) function, or the `data.content` member of [EZTR_MsgBuffer](@ref EZTR_MsgBuffer).

## The Message Buffer and Callbacks {#message_buffer_and_callbacks}

The most common time you'll use a message buffer directly is when using EZTR's message callbacks to create dynamically generated messages.

EZTR makes message callbacks easy to set up, as EZTR provides a macro that simplifies declaring callback functions. See [EZTR_MSG_CALLBACK](@ref EZTR_MSG_CALLBACK) for more information.

When run, the callback will receive an `EZTR_MsgBuffer` pointer. The lifetime of this buffer is managed by EZTR internally, and therefore you should NOT try to destroy it.

The buffer will be prepopulated with a copy of the message data EZTR has stored, which will then be passed on to the Majora's Mask message handler to be displayed.

For example, consider this code used by the *Forms Use More Items* mod. We define the message replacement with the header options we want and a default message. Then, when the message is loaded, `day_1_and_2_callback` is called, letting us overwrite the content of the message if appropriate.

```C
EZTR_MSG_CALLBACK(day_1_and_2_callback) {
    // The function this macro expands to has the arguments `(EZTR_MsgBuffer* buf, u16 textId, PlayState* play)`.
    Player* player = GET_PLAYER(play);

    if (player->transformation == PLAYER_FORM_GORON || player->transformation == PLAYER_FORM_FIERCE_DEITY) {
        // If we're Goron Link or Fierce Deity Link, use this dialog instead.
        EZTR_MsgSContent_Sprintf(buf->data.content, EZTR_CC_SFX_ARGW "What shall we do, Honey?" EZTR_CC_NEWLINE "From what I can see, he's too" EZTR_CC_NEWLINE "heavy for the platform to support." EZTR_CC_EVENT EZTR_CC_END, NA_SE_VO_DAVO02);

    } else if (player->transformation == PLAYER_FORM_ZORA) {
        // If we're Zora Link, use this dialog instead.
        EZTR_MsgSContent_Sprintf(buf->data.content, EZTR_CC_SFX_ARGW "He's a celebrity, Honey." EZTR_CC_NEWLINE "You wouldn't want me to think" EZTR_CC_NEWLINE "I have competition, do you?" EZTR_CC_EVENT EZTR_CC_END, NA_SE_VO_DAVO02);
    } 
    // Otherwise, use the stored (vanilla) dialog.
}

EZTR_ON_INIT void setup_messages() {

    // Replacements for Honey & Darling
    EZTR_Basic_ReplaceText(
        0x2841,
        EZTR_STANDARD_TEXT_BOX_I,
        0,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        false,
        EZTR_CC_SFX_ARGW "What shall we do, Honey?" EZTR_CC_NEWLINE "From what I can see, he doesn't" EZTR_CC_NEWLINE "seem to be able to carry " EZTR_CC_COLOR_RED "bombs" EZTR_CC_COLOR_DEFAULT "." EZTR_CC_EVENT EZTR_CC_END,
        day_1_and_2_callback,
        NA_SE_VO_DAVO02
    );
}
```

## The Independent Message Buffer

If needed, you can create your own instances of [EZTR_MsgBuffer](@ref EZTR_MsgBuffer) using [EZTR_MsgBuffer_Create](@ref EZTR_MsgBuffer_Create) or one of its related functions. This enables more advanced use cases of EZTR, such as constructing messages or replacements using data from an external source.

Keep in mind:

* You are responsible for destroying any message buffers you create to avoid memory leaks. Use [EZTR_MsgBuffer_Destroy](@ref EZTR_MsgBuffer_Destroy) for this.
* **DO NOT try to allocate an [EZTR_MsgBuffer](@ref EZTR_MsgBuffer) on the stack**. In the context of Zelda64Recompiled, this may cause a stack overflow.
* If you're only doing basic string processing (and don't need to handle the header), you may not need to allocate an entire message buffer. [MsgSContent](@ref MsgSContent) works fine with `char*`, as long as it's `\xBF`-terminated.
