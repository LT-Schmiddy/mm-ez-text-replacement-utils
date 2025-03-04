# EZ Text Replacer API

This is an API for modders that enables easily replacing in-game text and dialog.

To use this API in your own projects, include `MM_EZ_Text_Replacer_API:2.0.0` as a dependency in your `mod.toml`,
and download the API header from GitHub [here](https://github.com/LT-Schmiddy/mm-ez-text-replacement-utils/blob/main/include_in_dependents/eztr_api.h).

The main header contains a large amound of EZTR's documentation, but more information can be found in the online documentation [here](https://lt-schmiddy.github.io/docs/EZTR_for_Zelda64Recomp/index.html).

In addition to simple text replacements, v2.0.0 offers:

* Dynamically generates messages using a callback system.
* Support for adding new message entries. Use EZTR to quickly implement additional dialog for your mods.
* Tools and functions for and generating manipulating message strings, including a specialized version of the `printf` family of functions.

EZ Text Replacer is simple to use. For example, the following code is used to replace the all dialog related the Razor Sword's breaking after 100 uses.

```C
#include "eztr_api.h"

EZTR_ON_INIT void replace_msgs() {
    EZTR_Basic_ReplaceText(
        0x0038,
        EZTR_STANDARD_TEXT_BOX_I,
        1,
        EZTR_ICON_RAZOR_SWORD,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "|17Your Kokiri Sword has been|11strengthened and forged into a|11|01Razor Sword|00!|18|11|12This new, sharper blade is a cut|11above the rest!|BF",
        NULL
    );

    EZTR_Basic_ReplaceText(
        0x0C3B,
        EZTR_STANDARD_TEXT_BOX_I,
        49,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        100,
        EZTR_NO_VALUE,
        true,
        "All right...To |01reforge |00your sword,|11it will cost you |06100 Rupees|00. It'll|11be ready at |01sunrise.|11|00|12|17You'll have to let us hold onto|11your sword until then.|10So, would you like your sword|11reforged for |06100 Rupees|00?|11|02|C2I'll do it|11No thanks|BF|00|00",
        NULL
    );

    EZTR_Basic_ReplaceText(
        0x0C51,
        EZTR_STANDARD_TEXT_BOX_I,
        48,
        EZTR_ICON_NO_ICON,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "Now here's a secret...If you bring|11me |01gold dust|00, I'll be able to make|11it the |01strongest sword |00around.|10You got that? |01Gold dust|00!|19|BF|00",
        NULL
    );

    EZTR_Basic_ReplaceText(
        0x1785,
        EZTR_STANDARD_TEXT_BOX_II,
        1,
        EZTR_ICON_RAZOR_SWORD,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        EZTR_NO_VALUE,
        true,
        "|04Razor Sword|11|00The Kokiri Sword reforged at the|11smithy. A cut above the rest.|BF",
        NULL
    );
}
```

## Credits

Special thanks to:

* [https://github.com/mpaland/printf](https://github.com/mpaland/printf) for their printf implementation.
* [https://github.com/embeddedartistry/libc](https://github.com/embeddedartistry/libc) for various libc headers needed to compile this mod.
* [https://github.com/Decompollaborate/libgcc_vr4300](https://github.com/Decompollaborate/libgcc_vr4300) for linking against to provide additional math operations.
