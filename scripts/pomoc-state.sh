#!/usr/bin/env bash

POMOC="/home/y2k/stuff/dev/git/0l-dev-env/pomoc/build/bin/pomoc"

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
