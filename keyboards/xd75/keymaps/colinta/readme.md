# colinta's keymap for XD75

```
make xd75:colinta:dfu
```

The default layout is a Colemak keyboard with "sticky" keys assigned to the modifier keys.

    .--------------------------------------------------------------------------------------------------------------------------------------.
    | `      | 1      | 2      | 3      | 4      | 5      | MACRO 1| MACRO 2| KC_DEL | 6      | 7      | 8      | 9      | 0      | -      |
    |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
    | TAB    | Q      | W      | F      | P      | G      | MACRO 3| MACRO 4| BKSP   | J      | L      | U      | Y      | ;      | =      |
    |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
    | SHIFT  | A      | R      | S      | T      | D      | [      | FN LYR | ENTER  | H      | N      | E      | I      | O      | '      |
    |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------------------------+--------|
    | CTRL   | Z      | X      | C      | V      | B      | ]      | PG UP  | /      | K      | M      | ,      | .      | UP     | \      |
    |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------+--------|
    | VOL-   | PLAY   | VOL+   | ALT    | GUI    | SPACE  | HOME   | PG DN  | END    | SPACE  | ESC    | MENU   | LEFT   | DOWN   | RIGHT  |
    '--/RRND----/MUTE----/FFWD-------------------------------------------------------------------------------------------------------------'

I've implemented my own "tap/hold" feature, identical in spirit to "Space Cadet", but for generic use.  Tapping "1" sends a 1, but press and hold to send "F1".  Or, tap "VOL-" to turn the volume down, but press and hold to rewind.

The function layer can be used to switch to Qwerty (so other people can use this keyboard - also turns off sticky keys) or to start recording dynamic macros.

    .--------------------------------------------------------------------------------------------------------------------------------------.
    | SLEEP  |        |        |        |        |        | REC 1  | REC 2  |        |        |        |        |        |        |        |
    |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------|
    |        |        |        |        |        |        |COLEMAK | QWERTY |        |        |        |        |        |        |        |
    |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+-----------------+--------|
    |        |        |        |        |        |        |        |PREVIOUS|        |        |        |        |        |        |        |
    |--------+--------+--------+--------+--------+--------+--------+--LAYER-+--------+--------+--------+--------------------------+--------|
    |        |        |        |        |        |        |        |        |        |        |        |        |        |        |        |
    |--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------+--------|
    |        |        |        |        |CLEAR   |        |        |        |        |        | RESET  |        |        |        |        |
    '--------------------------------------MACROS------------------------------------------------------------------------------------------'

I implemented "stop recording" in a unique way; starting a macro recording sends the keyboard layer to a layer that has all the macro keys assigned to `DYN_REC_STOP`, and restores the layer to the default when recording is stopped.

I wish the Dynamic Macros feature supported more than just two, because I'd like to be able to record macros 3 & 4 instead of hardcoding them.

I try to keep http://www.keyboard-layout-editor.com/#/gists/f8a3c607b7a3c94c5de336bf74d7728f up to date with this key map.  I'm using DSA keycaps from [KBDFans DSA PBT](https://kbdfans.myshopify.com/collections/keycaps/products/dsa-pbt-145keys-keycaps-laser-etched-front-printed-legends) and [KBDFans DSA Blanks](https://kbdfans.myshopify.com/collections/keycaps/products/dsa-blank-keycaps-1).

## "secrets.h"

The macros I'm using are sensitive, and while I'm comfortable having them hardcoded onto my keyboard (no SSN or private GPG keys), they are not suitable for public viewing.  So I've put them in a header file that is excluded from the project.  This header file is included using `__has_include`, so if the file isn't present some blank defaults will be used instead.

If you would *also* like to take advantage of this feature, you'll first want to make sure your "secrets" file isn't included in the repo.  Open `.git/info/exclude` and add `secrets.h` to that file, below the comments.

###### .git/info/exclude
```
# git ls-files --others --exclude-from=.git/info/exclude
# Lines that start with '#' are comments.
# For a project mostly in C, the following would be a good set of
# exclude patterns (uncomment them if you want to use them):
# *.[oa]
# *~
/keyboards/xd75/keymaps/colinta/secrets.h
```

Then you can create this file and add your macro strings to it:

```
touch keyboards/xd75/keymaps/colinta/secrets.h
$EDITOR keyboards/xd75/keymaps/colinta/secrets.h
```

###### secrets.h
```
#define SENDSTRING_MM1 "shhhhh secret"
#define SENDSTRING_MM2 "stop saying pickle rick"
#define SENDSTRING_MM3 "abcd1234"
#define SENDSTRING_MM4 "HCF"
```

## Tap Hold

There is an array of `struct tap_hold_t`, and a list of `TH_n` keys in my custom keycodes.  They must be contiguous, because the keycode is used as an index for the `tap_hold_t` entry.

On key down the entry is updated with `.is_pressed = true, .timer = timer_read()`.  In `matrix_scan_user()` the entries are checked, and any entry where `is_pressed == true && timer > THRESHOLD` is considered a "hold" event, the hold key is sent (using `register_code/unregister_code`), and the entry is disabled (`is_pressed = false`).

If the key is released before the timer expires the event is considered a "tap" and the tap keypress is sent.

The use of `matrix_scan_user` makes for an immediate "hold" event, rather than checking the timer on key up.

## Sticky keys

I use sticky keys because of wrist issues, and my preferred logic is based off of how Mac OS treats sticky keys:

- press a modifier key once to turn it "on" for the next non-modifier key
- double tap a modifier to "lock" it (in mac os you just press the modifier key twice, but I prefer this to be on a timeout)
- pressing a non-modifier key turns the "sticky" setting off (but doesn't affect any locks)
- press the modifier again to turn it off

This behavior also works for press+hold events: on key down "sticky" is turned on, and on non-modifier key press sticky is turned off, but the modifier stays active as long as the modifier is held.

Another way to consider the modifier state is an `OR` of:

- currently pressed modifiers (`mods_down_state`)
- currently active sticky keys (`sticky_state`)
- currently active sticky locks (`sticky_lock`)

```c
set_mods(mods_down_state | sticky_state | sticky_lock);
```

## LEDs

First, check out the awesome typing effects on the WS2812 RGB LEDs.  Every non-modifier key will turn on an LED and fade it out, pretty cool when you're typing quickly.

The "gp103" LED flashes at startup and when the `RESET` key is sent.

The WS2812s turn white when the function layer is active, and yellow when the keyboard is in "programmable" mode.

The caps lock and "gp100" LEDs are used to show sticky status:

- caps lock on means one or more sticky keys are active
- caps lock AND gp100 means there is one or more sticky locks
- caps lock OFF and gp100 ON means you have just turned off a sticky key or sticky lock

If you need to turn off sticky keys in a hurry, using the "goto function layer" key will turn off all sticky keys and locks.
