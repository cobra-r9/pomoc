#!/usr/bin/env bash

# This is the path to binary. If you do not have a global installation of the binary, you can edit it below
# /path/to/pomoc
POMOC="/home/y2k/stuff/dev/git/0l-dev-env/pomoc/build/bin/pomoc"

# case you have it installed in your system's PATH.
# state="$(pomoc status state 2> /dev/null)"
state="$($POMOC status state 2>/dev/null)"

case "$state" in
idle) echo "󱎫" ;;
running) echo "󰥔" ;;
paused) echo "󰥕" ;;
break) echo "󰔟" ;;
*)
    echo "?"
    ;;
esac
