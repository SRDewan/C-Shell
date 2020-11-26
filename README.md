# How To Run:

	Run the following commands in the shell:
		$ make
		$ ./a.out

	NOTE:- 1. This shell's home directory is set as the directory from where the executable is invoked.

# Manually implemented commands:

1. echo - similar to bash's "echo" command
1. pwd - similar to bash's "pwd" command
1. cd (flags: ~, -, ., ..) - similar to bash's "cd" command
1. ls (flags: -a, -l) - similar to bash's "ls" command
1. jobs - gives info on all background jobs
1. setenv \<var> <value> - creates environment variable var and stores value in it
1. unsetenv \<var> - destroys environment variable var
1. bg - resumes a suspended background job in the background itself
1. nightswatch - similar to bash's "watch" command
1. newborn - prints pid of most recently created process
1. interrupt - prints CPU interrupt info
1. kjob <job number> <signal number> - sends signal corresponding to signal number to job corresponding to job number
1. overkill - kill all background jobs
1. quit - terminate shell

	NOTE:- Ctrl-C terminates the current running foreground command/job and Ctrl-Z suspends it and sends it to the background. Can use Ctrl-D to terminate shell.
