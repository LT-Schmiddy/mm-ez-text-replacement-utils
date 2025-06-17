# The Message Buffer {#the_message_buffer}

This document provides high-level information about the text handling in Majora's Mask, and the text manipulation tools provided by EZTR.

## Text in Majora's Mask

EZTR makes no changes to how the messages and text data in Majora's Mask are structured, stored, and processed. Therefore, any information relevant
to text in vanilla Majora's Mask should be applicable to EZTR.

In brief, the first 11 bytes of a message are a header, with the content of the message itself following after. In the header, you can set:

* The text box type/style.
* The text box's Y position on screen.
* What icon (if any) to display with the text.
* The ID of the next message to display.
* The cost of the first item being offered for sale, in rupees.
* The cost of the second item being offered for sale, in rupees.

The text encoding is non-standard: while all the printable characters are standard ASCII, the control codes are not. Of particular note, the termination character is `\xBF` rather than `\x00` (also written as `\0`).

The header and content regions combined cannot be more than 1280 bytes, as that is the maximum size of the buffer that is displayed.

Anything you need to know about the Majora's Mask text formatting can be found here: [https://wiki.cloudmodding.com/mm/Text_Format](https://wiki.cloudmodding.com/mm/Text_Format)

## Working with the message buffer
