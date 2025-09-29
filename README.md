# Minishell
🐚 Project Overview
A feature-rich custom Linux shell built from scratch in C that replicates core functionality of bash, including command execution, piping, job control, and custom signal handling. 
This project demonstrates deep understanding of process management, inter-process communication, and system programming.

🏗️ System Architecture
Core Components
Command Parser: Handles user input and tokenization
Command Dispatcher: Routes commands to appropriate handlers
Process Manager: Handles fork/exec and job control
Signal Handler: Manages interrupts and process signals
Pipe Engine: Implements multi-pipe functionality

🚀 Key Features
Command Execution
Built-in Commands: cd, pwd, echo, exit, jobs, fg, bg
External Commands: 150+ system commands via execvp
Pipe Support: Multiple pipe handling (cmd1 | cmd2 | cmd3)
Environment Variables: $$, $?, $SHELL support
