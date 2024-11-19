#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // ������� �����
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // ������� ����������
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // �������� �������
        close(pipefd[1]); // ��������� ����� ������

        // ������ �� ������ � ����������� � ������� �������
        while (read(pipefd[0], buffer, BUFFER_SIZE) > 0) {
            for (int i = 0; buffer[i] != '0'; i++) {
                buffer[i] = toupper(buffer[i]);
            }
            printf("Up registor: %s\n", buffer);
        }

        close(pipefd[0]); // ��������� ����� ������
        exit(EXIT_SUCCESS);
    }
    else { // ������������ �������
        close(pipefd[0]); // ��������� ����� ������

        // ���������� ����� � �����
        const char* text = "Hello, our World!\nThis is a test.\n";
        write(pipefd[1], text, strlen(text) + 1); // +1 ��� �������� ����-�����������

        close(pipefd[1]); // ��������� ����� ������
        wait(NULL); // ������� ���������� ��������� ��������
        exit(EXIT_SUCCESS);
    }
}
