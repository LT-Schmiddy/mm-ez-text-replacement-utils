# EZ Text Replacer API

This is an API for modders that enables easily replacing in-game text and dialog.

For example, the following code is used to replace the all dialog related the Razor Sword's breaking after 100 uses.

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
        "|17Your Kokiri Sword has been|11strengthened and forged into a|11|01Razor Sword|00!|18|11|12This new, sharper blade is a cut|11above the rest!|BF"
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
        "All right...To |01reforge |00your sword,|11it will cost you |06100 Rupees|00. It'll|11be ready at |01sunrise.|11|00|12|17You'll have to let us hold onto|11your sword until then.|10So, would you like your sword|11reforged for |06100 Rupees|00?|11|02|C2I'll do it|11No thanks|BF|00|00"
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
        "Now here's a secret...If you bring|11me |01gold dust|00, I'll be able to make|11it the |01strongest sword |00around.|10You got that? |01Gold dust|00!|19|BF|00"
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
        "|04Razor Sword|11|00The Kokiri Sword reforged at the|11smithy. A cut above the rest.|BF"
    );
}
```

To use this API in your own projects, include `MM_EZ_Text_Replacer_API:1.1.0` as a dependency in your `mod.toml`,
and download the API header from GitHub [here](https://github.com/LT-Schmiddy/mm-ez-text-replacement-utils/blob/main/include_in_dependents/eztr_api.h).

More detailed usage instructions can be found on GitHub [here](https://github.com/LT-Schmiddy/mm-ez-text-replacement-utils/blob/main/docs/Basic_API.md).

The API is fairly simple at the moment, but will be expanded in future updates. Planned features include:

* Dynamic message replacement.
* Support for adding new message entries.
* Callbacks at message load time.
* Message header manipulation.
* and more!

## Tools
You'll need to install `clang`, `make`, and the latest version of Python to build this template.

* On Windows, using [chocolatey](https://chocolatey.org/) to install both is recommended. The packages are `llvm` and `make` respectively.
  * The LLVM 19.1.0 [llvm-project](https://github.com/llvm/llvm-project) release binary, which is also what chocolatey provides, does not support MIPS correctly. The solution is to install 18.1.8 instead, which can be done in chocolatey by specifying `--version 18.1.8` or by downloading the 18.1.8 release directly.
* On Linux, these can both be installed using your distro's package manager.
* On MacOS, these can both be installed using Homebrew. Apple clang won't work, as you need a mips target for building the mod code.

On Linux and MacOS, you'll need to also ensure that you have the `zip` utility installed.

## Building

If you have Python, simply running the `build_mod.py` script should take care of everything.

If not, you can follow the instructions below:

* First, you need to build the [N64Recomp](https://github.com/N64Recomp/N64Recomp) library (included) for the `RecompModTool` utility.
* Second, run `make` (with an optional job count) to build the mod code itself.
* Third, run the `RecompModTool` utility with `mod.toml` as the first argument and the build dir (`build` in the case of this template) as the second argument.
  * This will produce your mod's `.nrm` file in the build folder.
