#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

// ��������� ��������� ��� ��������������� ������
void setup_terminal(struct termios* saved_settings) {
    struct termios new_settings;
    tcgetattr(STDIN_FILENO, saved_settings); // ��������� ������� ���������
    new_settings = *saved_settings;
    new_settings.c_lflag &= ~ICANON; // ��������� ������������ �����
    new_settings.c_lflag &= ~ECHO;    // ��������� ���
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings); // ��������� ����� ���������
}

// �������� ���������� ������� �� ������ �����
void remove_last_char(char* buffer, int* position) {
    if (*position > 0) {
        (*position)--; // ��������� �������
        buffer[*position] = '\0'; // ������� ������
        write(STDOUT_FILENO, "\b \b", 3); // ������� ������ �� ������
    }
    else {
        write(STDOUT_FILENO, "\a", 1); // �������� ������, ���� ������ ��� ��� ��������
    }
}

// �������� ���������� ����� �� ������ �����
void remove_last_word(char* buffer, int* position) {
    while (*position > 0 && buffer[*position - 1] == ' ') {
        remove_last_char(buffer, position); // ������� ������� � ����� �����
    }
    while (*position > 0 && buffer[*position - 1] != ' ') {
        remove_last_char(buffer, position); // ������� ������� �����
    }
}

int main() {
    struct termios original_settings;
    setup_terminal(&original_settings); // ����������� ��������

    char input_buffer[41] = { 0 }; // ����� ��� ����� (�������� 40 �������� + ����-����������)
    int current_index = 0; // ������� ������� � ������

    while (1) {
        char input_char;
        if (read(STDIN_FILENO, &input_char, 1) <= 0) {
            perror("������ ������ �����");
            break;
        }

        if (input_char == 4 && current_index == 0) { // Ctrl-D ��� ������
            break;
        }
        else if (input_char == 127) { // ������ �������� (Backspace)
            remove_last_char(input_buffer, &current_index);
        }
        else if (input_char == 21) { // Ctrl-U ��� ������� ������
            while (current_index > 0) {
                remove_last_char(input_buffer, &current_index);
            }
        }
        else if (input_char == 23) { // Ctrl-W ��� �������� ���������� �����
            remove_last_word(input_buffer, &current_index);
        }
        else if (input_char >= 32 && input_char <= 126) { // ���������� �������
            if (current_index < 40) {
                input_buffer[current_index++] = input_char; // ��������� ������ � �����
                write(STDOUT_FILENO, &input_char, 1); // ������� ������ �� �����
                if (current_index == 40) { // ���� ��������� �����
                    write(STDOUT_FILENO, "\n", 1);
                    current_index = 0; // ���������� ������
                }
            }
            else {
                write(STDOUT_FILENO, "\a", 1); // �������� ������ ��� ���������� �����
            }
        }
        else {
            write(STDOUT_FILENO, "\a", 1); // �������� ������ ��� ������������� �����
        }
    }

    write(STDOUT_FILENO, "\n", 1); // �������� ����� ������ ����� �������
    tcsetattr(STDIN_FILENO, TCSANOW, &original_settings); // ��������������� ��������� ���������
    return 0;
}
