# Custom Messages and Handles {#custom_messages_and_handles}

EZTR fully supports adding new messages with unique message IDs to Majora's Mask for use in your mods. For example, if you want to create a
brand new NPC, you can use EZTR to define all of their dialog in a way that's fully compatible with standard dialog/message system in Majora's Mask.

The process is slightly different from regular text replacements, however. This page documents those differences, and the important details to
keep in mind when adding new messages.

## Quickstart

### How To Declare a Custom Message

To define a custom message, you'll need to create a handle for it. To do that, add the following code somewhere in one of your C files, outside of any functions:

```C
EZTR_DEFINE_CUSTOM_MSG_HANDLE(my_custom_message_handle);
```

To access this handle in another C file, use the following to extern it in (works in C files or headers.):

```C
EZTR_DECLARE_CUSTOM_MSG_HANDLE(my_custom_message_handle);
// Or, alternatively use:
EZTR_EXTERN_CUSTOM_MSG_HANDLE(my_custom_message_handle);
```

Then, in your `EZTR_ON_INIT` event function, you'll call `EZTR_Basic_AddCustomText`, `EZTR_Basic_AddCustomBuffer`, or `EZTR_Basic_AddCustomTextEmpty` using
`EZTR_HNAME(test_message)` in the place of the handle argument. For example:

```C
EZTR_Basic_AddCustomText(EZTR_HNAME(my_custom_message_handle), EZTR_STANDARD_TEXT_BOX_I, 0, EZTR_ICON_NO_ICON, 
    EZTR_NO_VALUE, EZTR_NO_VALUE, EZTR_NO_VALUE, false, "HELLO WORLD\xBF", NULL);
```

You can only pass a custom message handle to the `EZTR_Basic_AddCustom*` functions once time. Attempting to use the handle again cause EZTR to print an error message.

To get the actual text ID value (to display it with the Majora's Mask function `Message_StartTextbox`, for example), you can use `EZTR_GET_ID_H(my_custom_message_handle)`. For example:

```C
Message_StartTextbox(play, EZTR_GET_ID_H(my_custom_message_handle), NULL);
```

### Replacing Custom Messages

If you know the name of a custom message handle from another mod, add that mod as a dependency to yours, then use the following code to import it.

```C
EZTR_IMPORT_CUSTOM_MSG_HANDLE("other_mod_id", external_custom_message_handle)
```

During your `EZTR_ON_INIT` event function, you can use `EZTR_Basic_ReplaceCustomText`, `EZTR_Basic_ReplaceCustomBuffer`, or `EZTR_Basic_ReplaceCustomTextEmpty` using `EZTR_HNAME(my_custom_message_handle)` as the handle argument. For example:

```C
EZTR_Basic_ReplaceCustomText(EZTR_HNAME(external_custom_message_handle), EZTR_STANDARD_TEXT_BOX_I, 0, EZTR_ICON_NO_ICON, 
        EZTR_NO_VALUE, EZTR_NO_VALUE, EZTR_NO_VALUE, false, "HELLO WORLD AGAIN!!\xBF", NULL);
```

## Advanced Usage

Instructions on more advanced features, as well a technical information about how custom messages work, is found below.

### A Word on Implementation Decisions

Before we continue, I'd like to take a moment and explain some things. In order to understand the advanced usage, you first need to understand certain, 'under-the-hood' details of the custom message system. And before I divulge that information, I'd like to explain why I settled on this implementation.

There were 3 major challenges in implementing custom messages:

* Preventing message ID overlap.
* Ensuring effiecient use of the message ID space.
* Ensuring custom messages can be replaced by other mods in the load order.

The first challenge is easily addressed by having EZTR manage and assign the ID values for every new message declared, rather than allowing mods to
declare their new message at any arbitrary ID value. The second and third challenges are not so easily handled.

It's important to remember that message IDs in Majora's Mask are unsigned 16-bit integers, and EZTR is not able to change that. The highest message ID
value used by the original game is 13644 (0x354C). Since the highest possible unsigned 16-bit integer is 65535, that leaves us 51891 remaining message
IDs to work with. Sure, that's a fairly large number, but it's not impossible that problems could arise as the Recomp modding matures, especially if
a mod badly designed. Since EZTR is responsible for handling out new message IDs (and obviously should not be handing out the same ID more than once),
it's not inconcievable some inexperienced modder might try to implement dynamic text generation by declaring a new message entry for every variant of
some statement. While EZTR's new restriction on declaring new messages outside of `EZTR_ON_INIT` will help with this (preventing those new messages from
being generated during gameplay), it does not fully solve the issue. Dynamic text such as this can easily be handled with EZTR's callback system instead.

For one mod to override a custom message from another mod is actually not far-fetched a situation as one might expect. Perhaps someone wishes to create
a translation patch for a custom NPC, or alter their dialog so that is matches up with content are altered by another mod. Such patches are easy enough
with vanilla messages, where the ID is a universal, hardcoded value, but less so when the ID is assigned at runtime by EZTR.

After considering several possible implementations, I settled on the system described below because it for the following reasons:

* EZTR manages custom message ID assignments, to prevent conflict between mods.
* It encourages frugality with declaring message IDs by ensuring that the number of message IDs a mod will receive is countable at compile time, and encouraging the use of dynamic messages via callbacks where appropriate.
* The IDs of specific custom messages are easily accessable (and the content replacable) by additional mods without requiring significant performance overhead.

### What is A Custom Message Handle, anyway?

A custom message handle is actually a function, the body of which is constructed in your mod by the `EZTR_DEFINE_CUSTOM_MSG_HANDLE` macro in `eztr_api.h`.

When expanded and formatted for readability, the resulting function for `EZTR_DEFINE_CUSTOM_MSG_HANDLE(my_custom_message_handle)` looks like this:

```C
RECOMP_EXPORT u16 EZTR_CustomMsgHandle_my_custom_message_handle(_EZTR_CustomMsgHandleSetter* setter) {
    static u16 id;
    static u8 is_set = 0;
    if (setter != NULL) {
        if (is_set) {
            _EZTR_ReportErrorMessage("The textId of EZTR_CustomMsgHandle 'my_custom_message_handle' has already been set and will not be updated.");
            setter->out_success = 0;
        } else { 
            id = setter->new_id;
            is_set = 1;
            setter->out_success = 1;
        }
    } 
    return id; 
}
```

First of all, you'll notice that the full function name has the prefix `EZTR_CustomMsgHandle_`. This is meant to distinguish it as a custom message handle, and not an ordinary function. The `HNAME(my_custom_message_handle)` macro expands to full function name, and used for readability. You'll also notice that the function is marked as `RECOMP_EXPORT` automatically.

As you can see, the message id is stored in a static variable within the function. There's also a different static variable that records whether the handle has been assigned to before.

When being assigned, EZTR calls the handle function with a pointer to a configuration struct. If the handle hasn't been set before, the ID is read and the operation is marked as a success in the struct (so EZTR knows that the provided ID was accepted). If the handle was assigned to before, an error message is printed (using a private function in EZTR's API), and the operation is marked as a failure in the struct (so EZTR knows it can use that ID value again). In either case, the function returns the message ID. Is assignment was reported as a success, EZTR will check the returned ID against the value in the configuration struct to ensure that the handle function is working correctly.

If the function is called with the configuration pointer set to `NULL`, no assignment operation is attempted. In all cases, the function will return the message ID value. That means that a handle's ID can be accessed by calling `EZTR_CustomMsgHandle_my_custom_message_handle(NULL)`. In fact, this is exactly
how the various `GET_ID` macros work.

### Disabling the Handle Prefix

If you want to disable use of the `EZTR_CustomMsgHandle_` prefix, you can use declaring `#define EZTR_NO_HANDLE_PREFIX` before including `eztr_api.h`,
or by adding `-DEZTR_NO_HANDLE_PREFIX` to your compilation flags. If you do this, you won't need to use `HNAME` when referencing your handle by name, but
it's recommended that you then add your own prefix instead.

If another mod has the handle prefix disabled, you can import it's handles by using:

```C
EZTR_IMPORT_CUSTOM_MSG_HANDLE_NO_PREFIX("other_mod_id", external_custom_message_handle);
```

### Disabling Handle Exports

If, for some reason, you don't want to export a particular message handle, you can use the macro:

```C
EZTR_DEFINE_CUSTOM_MSG_HANDLE_NO_EXPORT(my_custom_message_handle);
```

This macro functions the same as `EZTR_DEFINE_CUSTOM_MSG_HANDLE`, but doesn't add `RECOMP_EXPORT`.

This is not recommended.

### Using Handles as Variables and Arguments

EZTR provided a type definition for custom message handles called `EZTR_CustomMsgHandle`, which is defined as

```C
typedef u16 (*EZTR_CustomMsgHandle)(_EZTR_CustomMsgHandleSetter* setter);
```

This will allow you do use a custom message handle as you would a regular function pointer. Obviously, you won't need to use the `HNAME` macro
when you do this.

Note that the `EZTR_GET_ID_H` macro is shorthand for `EZTR_GET_ID(EZTR_HNAME(my_handle))`, and therefore shouldn't be used with function pointers.
Use `EZTR_GET_ID` instead.

### Further Info

For more information on EZTR Custom Message Handles, you can find the complete documentation of all involved macros and types under [CustomMsgHandle](@ref CustomMsgHandle).
