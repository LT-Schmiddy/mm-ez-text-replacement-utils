# EZTR Basic API

Currently, the basic API is the only implemented method of replacing text. It's very simple, but it should be able to a lot situations.

The basic API exposes exactly one function and one callback. All components to the basic API are built included in `eztr_api.h`, so there's
no need to declare them yourself.

## `EZTR_ON_INIT` and `RECOMP_CALLBACK("MM_EZ_Text_Replacer_API", EZTR_OnInit)`

This `EZTR_OnInit` callback runs immediately after EZTR finished its own startup. If you want to declare text replacements that aren't going to change,
this is the recommended place to do it. You should not try to declare any text replacements in `RECOMP_CALLBACK("*", recomp_on_init)`, since
there's no guarantee that EZTR will be initialized before your mod, and attempting to declare a text replacement before EZTR is initialized
will cause a crash.

`EZTR_ON_INIT` is a macro that expands to `RECOMP_CALLBACK("MM_EZ_Text_Replacer_API", EZTR_OnInit)`, in order to make declaring your callback
function easier. Something like `EZTR_ON_INIT void my_callback()` works perfectly.

## `RECOMP_EXPORT void EZTR_Basic_ReplaceText(...)`

This function is how you actually declare a text replacement. The full signature of the function is:

```C
RECOMP_EXPORT void EZTR_Basic_ReplaceText(
    u16 textId,
    u8 text_box_type, 
    u8 text_box_y_pos, 
    u8 display_icon, 
    u16 next_message_id, 
    u16 first_item_rupees, 
    u16 second_item_rupees,
    bool pipe_escape_bytes,
    char* content
) 
```

If you don't know how Majora's Mask handles it's message data internally, I recommend going to read the documentation on Cloud Modding [here](https://wiki.cloudmodding.com/mm/Text_Format) before continuing.

The first 6 arguments of the function are used to compose the message's header, which comprises the first 11 bytes of the message data. The last 2 arguments are used to compose the message content. Let's look at the arguments individually:

| Argument | Description |
| -------- | ----------- |
| `u8 text_box_type` | The style of textbox to render. `eztr_api.h` has offers the enum `EztrTextBoxType` to make this more readable. |
| `u8 text_box_y_pos` | The vertical position of the textbox on the screen. |
| `u8 display_icon` | An icon id to display in the textbox. `eztr_api.h` has offers the enum `EztrTextBoxIcon` to make this more readable. |
| `u16 next_message_id` | The id of the next message to load. This value should be `0xFFFF` if there is no next message, or use `EZTR_NO_VALUE` for better readability |
| `u16 first_item_rupees` | The cost (in rupees) of the first item being offered, if items are being offered for sale. If no items are being offered, this should be `0xFFFF` or `EZTR_NO_VALUE`. |
| `u16 second_item_rupees` | The cost (in rupees) of the second item being offered, if items are being offered for sale. If no items (or only one item) are being offered, this should be `0xFFFF` or `EZTR_NO_VALUE`. |
| `bool pipe_escape_bytes` | Enables a special syntax for the `content` argument, where non-ascii bytes can be represented be escaped with the pipe (`\|`) character. For example, you can write `\|00` instead of `0x00`. The pipe symbol itself can be entered as `\|\|`. Useful if your compiler complains about invalid bytes. |
| `char* content` | The actual text of the message. Remember that Majora's Mask uses completely different control bytes than standard ASCII. Consult the Cloud Modding page for information about how these work. |

For information on how to get this information for any given piece of text, see the page on text dumping (coming soon).
