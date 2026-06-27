# pomoc 

**What this is about :**
`pomoc` is a simple pomodoro for unix based systems, which uses **unix sockets** for communication between the **client** : `pomoc` and the **daemon** : `pomod`. This is part of my C learning program, specifically, unix sockets and project structure. 

**Why did I create this?**
Because I was in the need for creating it. I wanted to integrate a productive pomodoro, that is not bloated like with GUI. I am a maximal terminal guy who spends the time mostly learning or reading books in zathura and programming in neovim. I need to keep track of my time, and that's why this was born. Apart from that, this project is just a learning phase. 

**What is special about this?**

It is simply just terminal based. It is a daemon. The daemon process is the `pomod` (pomodoro daemon) and you communicate commands and start, stop or modify the timer via `pomoc` (pomodoro client). As this is also for a learning phase, I have included write ups in the **docs** directory.

Uses the unix philosophy. Does one thing, does that thing right. And it does - keeps track of time. I have baked in a set of commands in this client so that it is scriptable and easily able to integrate with bar systems and other programs (that is, `pomoc` acts as an IPC).

You will be able to keep track of your learning or workflow, the time spend by using `pomoc`, `task`, `taskwarrior-tui` and some bar like `polybar`. That's it. No heavy GUI, No bloated window occupying space.

