#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>

// jmp_buf contains the process context and location to jump back to
static jmp_buf env;

void try_catch(int sig) {
	// jump back to the setjmp point
	longjmp(env, sig);
}

int main() {
	// mark this as the point to jump back to
	int jump_val = setjmp(env);

	// TRY BLOCK: only try this the first time (when jump_val is 0)
	if (jump_val == 0) {
		signal(SIGFPE, try_catch);
		int x = 0;
		int y = 10 / x;
	}	else {
		// CATCH BLOCK: do this, then continue
		if (jump_val == SIGFPE) printf("divide by zero exception thrown\n");
		else printf("unknown exception thrown\n");
	}
}
