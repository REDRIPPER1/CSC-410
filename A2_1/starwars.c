#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Starting shield power level
int shield_power = 50;  

int main() {
    const char *characters[] = {"Luke", "Han", "Chewbacca", "Leia"};
    const int adjustments[] = {25, 20, 30, 15};
    pid_t children[4];
    int child_count = 0;

    printf("Millennium Falcon: Initial shield power level: %d%%\n\n", shield_power);


    // Create 4 child processes - 4 different characters adjusting shield power
    for (int i = 0; i < 4; i++) {
        pid_t pid = fork();

        // Check if process creation failed
        if (pid < 0) {
            perror("fork");
            break;
        }

        if (pid == 0) {
            printf("%s: Adjusting shield power...\n", characters[i]);
            shield_power += adjustments[i];
            printf("%s: Shield power level now at %d%%\n",
                   characters[i], shield_power);
            return 0;
        }

        children[child_count++] = pid;
    }

    // Make parent process wait for all child processes to complete
    for (int i = 0; i < child_count; i++) {
        if (waitpid(children[i], NULL, 0) < 0) {
            perror("waitpid");
            return 1;
        }
    }

    if (child_count != 4) {
        return 1;
    }


    // Parent process reports final state
    printf("\nMillennium Falcon: Final shield power level: %d%%\n", shield_power);
    printf("\nMay the forks be with you!\n");
    return 0;
}
