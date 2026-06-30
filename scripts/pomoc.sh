#!/usr/bin/env bash

# This is the path to binary. If you do not have a global installation of the binary, you can edit it below
# /path/to/pomoc
POMOC="/home/y2k/stuff/dev/git/0l-dev-env/pomoc/build/bin/pomoc"

# case you have it installed in your system's PATH.
# result="$(pomoc status time 2> /dev/null)"
# active="$(pomoc status active 2>/dev/null)"
result=$("$POMOC" status time 2>/dev/null)
active=$("$POMOC" status active 2>/dev/null)
if [[ ! "$result" ]]; then
    echo ""
else
    echo "$result" | grep -qP "^\d{2}:\d{2}:\d{2}" &&
        echo "$result" | sed 's/\([0-9]*\):\([0-9]*\):\([0-9]*\)/\1h \2m \3s/' ||
        echo "$result" | sed 's/\([0-9]*\):\([0-9]*\)/\1m \2s/'

    if [[ "$result" == "00:00" || "$result" = "00:00:00" ]]; then
        notify-send "Pomodoro Client : $active" "Your $active period has ended." -u "critical" -t "5000" &
        timeout 5 paplay /usr/share/sounds/freedesktop/stereo/alarm-clock-elapsed.oga &
    fi
fi
