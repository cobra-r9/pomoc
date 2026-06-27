## Installation

**To install from source (for non arch systems) :**

```bash
make
```

Binaries land at `build/bin/pomod` and `build/bin/pomoc`. You can either symlink them to some directory which is in `$PATH`, or just copy it to `~/.local/bin` or `/usr/bin` for user or systemwide binaries respectively. 


**To install via makepkg (for arch systems) :**

```bash
makepkg -si
```

This installs `pomod` and `pomoc` to `/usr/bin/`.


## Setup

Create the config file:

```bash
mkdir -p ~/.config/pomodoro
nvim ~/.config/pomodoro/config
```

Of course, you can use the editor of your choice to edit the configuration.
Note, that the config is not immune to whitespaces. It should be how it is. You just configure these two. That's it. 


```ini
# focus time hr:min:sec
focus = 00:25:00

# break time min:sec
break = 05:00
```

If no config file is found, defaults are used: focus `25:00`, break `5:00`.

## Starting the Daemon

`pomod` must be running before any `pomoc` command works. As stated, pomod is the pomodoro daemon, wheras, the pomoc is the pomodoro client. 

```bash
pomod
```

To run it in the background:
```bash
pomod &
```

To stop it:
```bash
pkill pomod
```

## Commands

The below given table will **teach** you how to use this tool. I have also included some useful scripts in the scripts directory to make it easy for the users (you ppl) to integrate into other programs and bars. 

### Control

| Command | Description |
|---|---|
| `pomoc start` | Start or resume focus timer |
| `pomoc pause` | Pause focus timer |
| `pomoc end` | End focus early, triggers break immediately |

### Status

| Command | Output |
|---|---|
| `pomoc status` | Full status: state, phase, remaining time |
| `pomoc status active` | Active phase: `focus` or `break` |
| `pomoc status time` | Remaining time formatted |
| `pomoc status hr` | Remaining hours (raw) |
| `pomoc status min` | Remaining minutes (raw) |
| `pomoc status sec` | Remaining seconds (raw) |

### Config (only when not running)

| Command | Description |
|---|---|
| `pomoc focus hr:min:sec` | Set focus time |
| `pomoc break min:sec` | Set break time |
| `pomoc focus hr +N` | Increment/decrement focus hours |
| `pomoc focus min +N` | Increment/decrement focus minutes |
| `pomoc focus sec +N` | Increment/decrement focus seconds |
| `pomoc break min +N` | Increment/decrement break minutes |
| `pomoc break sec +N` | Increment/decrement break seconds |


### Rules

- Break is **mandatory** — cannot be paused, skipped, or ended. This is because : **you need to touch some grass**. 
- Config changes are **blocked** while focus or break is running. 
- Config changes **are allowed** while paused. 
- Once break ends, timer returns to idle with focus config reloaded.


## Time Format

`pomoc status` and `pomoc status time` display time as:
- `hr:min:sec` if hours are non-zero → `01:24:37`
- `min:sec` if hours are zero → `24:37`

Raw field queries (`hr`, `min`, `sec`) will always return the value as-is with zero padding → `00`, `05`, `37`.

## Example Session

So, power up your terminal and try this tool. I use alacritty btw. 

```bash
# start daemon
pomod &

# set a 45 minute focus session
pomoc focus 00:45:00

# add 5 more minutes
pomoc focus min +5

# start
pomoc start

# check status
pomoc status
# → running focus 49:42

# check time only
pomoc status time
# → 49:42

# pause
pomoc pause

# resume
pomoc start

# end focus early
pomoc end
# → break starts automatically

# check break status
pomoc status
# → break break 04:58

# after break ends, back to idle
pomoc status
# → idle focus 50:00
```


## Statusbar Integration

Since `pomoc status time` and `pomoc status active` print clean single-line output, you can use them directly in waybar, i3blocks, polybar or any statusbar:

```bash
# example shell script for statusbar
echo "$(pomoc status active) — $(pomoc status time)"
# → focus — 24:37
```

