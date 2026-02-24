# What is gimmetask and why is gimmetask

The goal of this program, outside of having been a good tutorial project I could dogfood while learning C, is to avoid overwhemling the user with the size of their task list while still tracking everything. It shows you one (random) task at a time, and saves your task in human-redable plain text inspired by Todo.txt's rules.

The inspiration came from one of the times I experienced some major decision paralysis and spoke to my manager about it. He said when in doubt, it's better to just work on a random task than be blocked stressing about the whole list. With gimmetask, you can quickly enter or copy/paste your list and work with it one task at a time.

# What it looks like

**First Run**
```
No backup file.
Enter one task per line, max 80 chars per task, TO STOP TYPE done ----------- |
buy groceries
Got: buy groceries
call mom
Got: call mom
done
** Added the new ones. **
Max tasks: 100; Total tasks: 2
Next is #2: call mom
(D)one, (N)ext, (R)emove, (A)dd, (L)ist, (G)et, (C)lear, or (Q)uit?
q
~>ls
task-backup.txt
```

**Second Run**
```
** Import Successful **
Max tasks: 100; Total tasks: 2
Next is #2: call mom
(D)one, (N)ext, (R)emove, (A)dd, (L)ist, (G)et, (C)lear, or (Q)uit?
```

**The Backup File**
The backup file is a plain text list of your tasks with an `x ` in front of the ones you marked done and a `- ` in front of ones you removed.
```
~> cat task-backup.txt
buy groceries
x call mom
x pet cat
draft the readme
- go to Camelot
```

**The Done List**
When you (C)lear your active list, all the tasks marked done are removed and appended to task-done.txt
```
(D)one, (N)ext, (R)emove, (A)dd, (L)ist, (G)et, (C)lear, or (Q)uit?
c
Cleaned out done/removed and replaced backup. New task:
#1 buy groceries
(D)one, (N)ext, (R)emove, (A)dd, (L)ist, (G)et, (C)lear, or (Q)uit?
q
~> cat task-done.txt
x call mom
x pet cat
- go to Camelot
```

# Suggestion

There's currently no support for telling gimmetask where you want it to save the task files, so you should just put gimme's executable wherever you want them saved. Fortunately, it's very small making it practical to have multiple copies in different folders (be they cloud or not) for multiple lists! Then you can add shortcuts to each one, such as gimmework, gimmehome, and gimmeidea.

# On AI, LLM, and Agents

LLMs are cool and all, but this was a learning project started in 2023 before LLMs were putting out decent code. Thus I wrote it by hand with just vim, no plugins, no auto-complete, C documentation, _Learn C the Hard Way_ by Zed A. Shaw and a bit of googling. For posterity, I will continue to only include code written in such a way. Enjoy this organically produced software written by free-range humans!

# Future Plans for gimmetask

The biggest limitation of this for practical use is that it doesn't help you break tasks down into smaller sizes. You can already include +projectname or any other text tags of course, but one big project would overwhelm your list. It might make sense to add some depth value, but I'm wary of going too far. This is not a PM tool, and there's already a nice todo.txt CLI project. Another fun idea I had to extend this was some gamification as seen in the points.c file. Practically though, gimmetask is done. Lastly it might be useful to run gimmetask as a one shot command which would just need the last task number to be tracked between runs.

# Building gimmetask

This should be simple as it requires nothing beyond a C compiler.

1. Have some C or C++ build tools installed for your OS
2. Clone the repo
3. Navigate to the repo
4. Enter the command make

At the time of writing, I have a pre-release version of gimmetask built for Windows. The latest I only have for GNU/Linux.
