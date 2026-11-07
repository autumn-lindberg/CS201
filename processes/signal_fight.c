#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>	// fork()
#include <signal.h> // signals
#include <time.h>

// SIGUSR1 = "I hit you, take damage" USER_DEFINED_SIGNAL_HIT
// SIGUSR2 = "I died, u can stop fighting" USER_DEFINED_SIGNAL_DEATH
//
// each proces keeps its own hp, picks a random damage value when hit
// and kills its opponent on each turn until somebody dies.
// 
// SUBTLETIES
// - printf() can't be used in a signal handler (doesn't work with async)
// - rand() will give same value in both processes, so we XOR with the PID
// - sleep() can be interrupted, so sleep(rand() % 4) might come back early 

#define STDOUT 1

// volatile means "don't cache"
// sig_atomic_t means "won't be interrupted by signals" (no data corruption)
// USE CASE: "static" variables that differ between signals (that's about it)
volatile sig_atomic_t hp = 50;
volatile sig_atomic_t still_fighting = 1;
volatile sig_atomic_t won = 0;

// when this signal is received, TAKE DAMAGE
void onHit(int enemyPID) {
	if (hp > 0) {
		// calculate damage
		int dmg = rand() % 4;
		hp -= dmg;
		// signal occurring during a printf call corrupts the buffer
		// so sprintf is used (loads the string into buffer, returns its length)
		char buffer[80];
		int length = sprintf(buffer, "I, Mrs. %d, took %d damage!\n", getpid(), dmg);
		// along with write...
		write(STDOUT, buffer, length);
		if (hp <= 0) {
			// tell the opponent we're dead
			kill(enemyPID, SIGUSR2);
			// I should stop fighting cuz I'm dead
			still_fighting = 0;
		}
	}
}

// when this signal is received, STOP FIGHTING
void onDeath() {
	// if I have HP, I've won
	if (hp > 0) won = 1;
	// 
}

void fightLoop(int enemyPID) {
	while (still_fighting) {
		// display hp	
		char buffer[80];
    int length = sprintf(buffer, "I, Mrs. %d, have %d hp left\n\n", getpid(), hp);
    write(STDOUT, buffer, length);
		// deal damage to opponent (send the signal to them)
		kill(enemyPID, SIGUSR1);
		
		// sleep returns how much sleep is remaining
		// must be at least 1 second
		int p = sleep(rand() % 4 + 1);
		// while there's still sleep left, call sleep with the remaining time
		while (p > 0) {
			p = sleep(p);
		}
	}
	// end the loop
	if (won) {
		char buffer[80];
    int length = sprintf(buffer, "I, Mrs. %d, have emerged victorious!\n", getpid(), hp);
    write(STDOUT, buffer, length);
	} else {
		char buffer[80];
    int length = sprintf(buffer, "I, Mrs. %d, have died!\n", getpid(), hp);
    write(STDOUT, buffer, length);
		
	}
}


int main() {
	struct sigaction hit;
	// what to do when "hit" (handler function)
	hit.sa_handler = onHit;
	// clear the signal masks (masked = "block" the signal)
	sigemptyset(&hit.sa_mask);
	// allow signal to restart
	hit.sa_flags = SA_RESTART;
	// change the action of signal code SIGUSR1 to hit
	// (hit has the handler)
	sigaction(SIGUSR1, &hit, NULL);

	struct sigaction death;
	death.sa_handler = onDeath;
	sigemptyset(&hit.sa_mask);
	hit.sa_flags = SA_RESTART;
	sigaction(SIGUSR2, &death, NULL);

	// fork and return the pid of the parent (caller) 
	pid_t pid = fork();

	// re-seed after the fork so the processes don't roll identical damage
	srand(time(0) ^ getpid());

	if (pid == 0) {
		// child: opponent is the parent
		fightLoop(getppid());
	} else {
		// parent: opponent is child
		fightLoop(pid);
		// PARENT TELLS THE CHILD THEY CAN STOP THEIR PROCESS
		// OTHERWISE IT BECOMES A ZOMBIE AND CAN'T BE KILLED
		// ZOMBIE AND KILL ARE TECHNICAL TERMS.
		wait(NULL);
	}
	return 0;
}
