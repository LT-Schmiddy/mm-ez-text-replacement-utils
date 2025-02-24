# EZTR Basic API

This guide provides general information on using EZTR.

## `EZTR_ON_INIT` and `RECOMP_CALLBACK("MM_EZ_Text_Replacer_API", EZTR_OnInit)`

This `EZTR_OnInit` callback runs immediately after EZTR finished its own startup. If you want to declare text replacements that aren't going to change,
this is the recommended place to do it. You should not try to declare any text replacements in `RECOMP_CALLBACK("*", recomp_on_init)`, since
there's no guarantee that EZTR will be initialized before your mod, and attempting to declare a text replacement before EZTR is initialized
will cause a crash.

`EZTR_ON_INIT` is a macro that expands to `RECOMP_CALLBACK("MM_EZ_Text_Replacer_API", EZTR_OnInit)`, in order to make declaring your callback
function easier. Something like `EZTR_ON_INIT void my_callback()` works perfectly.
