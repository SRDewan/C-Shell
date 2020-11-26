#How To Run:

	Run the following commands in the shell:
		$ make
		$ ./a.out

	NOTE:- 1. This shell's home directory is set as the directory from where the executable is invoked.

#Manually implemented commands:

	1. echo - similar to bash's "echo" command
	2. pwd - similar to bash's "pwd" command
	3. cd (flags: ~, -, ., ..) - similar to bash's "cd" command
	4. ls (flags: -a, -l) - similar to bash's "ls" command
	5. jobs - gives info on all background jobs
	6. setenv <var> <value> - creates environment variable var and stores value in it
	7. unsetenv <var> - destroys environment variable var
	8. bg - resumes a suspended background job in the background itself
	9. nightswatch - similar to bash's "watch" command
	10. newborn - prints pid of most recently created process
	11. interrupt - prints CPU interrupt info
	12. kjob <job number> <signal number> - sends signal corresponding to signal number to job corresponding to job number
	13. overkill - kill all background jobs
	14. quit - terminate shell

	NOTE:- Ctrl-C terminates the current running foreground command/job and Ctrl-Z suspends it and sends it to the background. Can use Ctrl-D to terminate shell.
