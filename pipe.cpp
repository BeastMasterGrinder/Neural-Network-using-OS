#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <cstring>

int main() {
    int pipefd[2];
    pid_t childPID;

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // Create child process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        // Child process
        close(pipefd[1]);  // Close the write end in the child process

        char buffer[256];
        ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer));
        if (bytesRead == -1) {
            perror("read");
            return 1;
        }

        std::cout << "Child process received: " << buffer << std::endl;

        close(pipefd[0]);  // Close the read end in the child process
        return 0;
    } else {
        // Parent process
        childPID = pid;
        close(pipefd[0]);  // Close the read end in the parent process

        std::string message = "Hello from parent to child";
        ssize_t bytesWritten = write(pipefd[1], message.c_str(), message.size());
        if (bytesWritten == -1) {
            perror("write");
            return 1;
        }

        close(pipefd[1]);  // Close the write end in the parent process
    }

    // Wait for the child process to finish
    waitpid(childPID, NULL, 0);

    return 0;
}
