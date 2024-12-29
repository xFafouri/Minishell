[![MINISHELL 42. Tinyshell is a compact version of the… | by abkabex | Medium](https://miro.medium.com/v2/da:true/resize:fit:600/1*i5jJpep8voRxtzCP9mJRgQ.gif)![MINISHELL 42. Tinyshell is a compact version of the… | by abkabex | Medium](https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRJ2mPaaOmtICte6GyyE_YWrikaKL8zbdEfNw&s)

# **What is Minishell?**

Minishell is a project from 42 designed to challenge students by recreating a simplified version of Bash. The program parses user commands, executes them with arguments, and includes several built-in functions. It's a hands-on way to learn about shell behavior, process management, and system calls.

[![Minishell Image](https://camo.githubusercontent.com/59de649cadf69776d7e965ad6f4b2326d3289ae38e2000112b85a36be3c89a3b/68747470733a2f2f692e696d6775722e636f6d2f456e31334137702e706e67)](https://camo.githubusercontent.com/59de649cadf69776d7e965ad6f4b2326d3289ae38e2000112b85a36be3c89a3b/68747470733a2f2f692e696d6775722e636f6d2f456e31334137702e706e67)

## **How to Use?**
-------------------------------------
### **Installation**
-------------------------------------------
1. Clone the repository:
    `git clone https://github.com/yourusername/minishell.git cd minishell`
    
2. Compile the program using `make`:

    `make`
    
3. Run the executable:

    `./minishell`
    

---

## **Key Features**
-----------------------------------------
### **1. Command Execution**
-----------------------------------------
- Supports both absolute and relative paths for executables (`/bin/ls` or `ls`).
- Handles command-line arguments and options seamlessly.
- Implements single (`'`) and double (`"`) quotes with behavior similar to Bash, except for multiline commands.

### **2. Command Separators and Redirections**
--------------------------------------------
- Use `;` to separate multiple commands on the same line.
- Implements redirection operators:
    - Output redirection: `>` and `>>`
    - Input redirection: `<`
- Supports pipes (`|`) for chaining commands.

### **3. Environment Variables**
-------------------------------------------
- Fully supports variables like `$HOME` and `$PATH`.
- Includes the special variable `$?`, which provides the exit status of the last executed command.

### **4. Signal Handling**
-------------------------------------------
- **Ctrl-C**: Interrupt the current command.
- **Ctrl-**: Quit the program.
- **Ctrl-D**: Send an End-of-File (EOF) signal to exit the shell.

### **5. Built-In Functions**
------------------------------------------------
Minishell directly implements several commands without invoking external executables:

- `echo`: Print text to the console.
- `pwd`: Display the current working directory.
- `cd`: Change the working directory.
- `env`: Print environment variables.
- `export`: Set environment variables.
- `unset`: Remove environment variables.
- `exit`: Exit the shell.

---

## **Project Goals**
----------------------------------------
Minishell is more than just a shell—it’s an opportunity to learn about:

- **Process Management**: How to create and manage processes with `fork`, `exec`, and `wait`.
- **File Descriptors**: Handling standard input/output redirection and pipes.
- **Signal Handling**: Managing signals to provide a smooth user experience.
- **String Parsing**: Breaking down user input into meaningful commands and arguments.
- **Error Handling**: Dealing with unexpected scenarios gracefully.

---
## **Demo**
-------------------------------------
Here’s an example of Minishell in action:

```
$ pwd
/home/user/minishell

$ echo "Hello, Minishell!"
Hello, Minishell!

$ ls | grep minishell
minishell.c
minishell.h

$ export MY_VAR="Minishell is cool"
$ echo $MY_VAR
Minishell is cool

$ exit
Goodbye!

```