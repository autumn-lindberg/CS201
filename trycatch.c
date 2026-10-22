#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

// kind of like task_struct, but for signals
static jmp_buf env;

void try_catch(int sig) {
	// jump back to the setjmp point
	longjmp(env, sig);
}

int main() {
	// mark this as the point to jump back to
	int jump_val = setjmp(env);

	if (jump_val == 0) {
		signal(SIGFPE, try_catch);
		int x = 0;
		int y = 10 / x;
	}	else {
		if (jump_val == SIGFPE) printf("exception thrown\n");
		else printf("unknown exception thrown\n");
	}
}
