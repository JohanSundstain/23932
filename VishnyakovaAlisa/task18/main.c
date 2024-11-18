#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc > 1) {
        struct stat sb;
        struct group* grp;
        struct passwd* pwp;
        time_t mt;

        for (int i = 1; i < argc; i++) {
            if (stat(argv[i], &sb) == -1) {
                perror("Could not get information about the file");
                continue; // ���������� ����, ���� ��������� ������
            }

            // ����������� ���� �����
            if (S_ISREG(sb.st_mode)) {
                printf("- ");
            }
            else if (S_ISDIR(sb.st_mode)) {
                printf("d ");
            }
            else {
                printf("? ");
            }

            // ����� ���� �������
            printf("%c%c%c%c%c%c%c%c%c ",
                (sb.st_mode & S_IRUSR) ? 'r' : '-',
                (sb.st_mode & S_IWUSR) ? 'w' : '-',
                (sb.st_mode & S_IXUSR) ? 'x' : '-',
                (sb.st_mode & S_IRGRP) ? 'r' : '-',
                (sb.st_mode & S_IWGRP) ? 'w' : '-',
                (sb.st_mode & S_IXGRP) ? 'x' : '-',
                (sb.st_mode & S_IROTH) ? 'r' : '-',
                (sb.st_mode & S_IWOTH) ? 'w' : '-',
                (sb.st_mode & S_IXOTH) ? 'x' : '-');

            // ����� ����� ������
            grp = getgrgid(sb.st_gid);
            printf("%s ", grp ? grp->gr_name : "unknown_group");

            // ����� ����� ������������
            pwp = getpwuid(sb.st_uid);
            printf("%s ", pwp ? pwp->pw_name : "unknown_user");

            // ����� ������� �����
            printf("%ld ", sb.st_size);

            // ����� ������� ���������� ���������
            mt = sb.st_mtime;
            struct tm* mtime = localtime(&mt);
            printf("%02d/%02d %02d:%02d ",
                mtime->tm_mon + 1,
                mtime->tm_mday,
                mtime->tm_hour,
                mtime->tm_min);

            // ����� ����� �����
            char* filename = strrchr(argv[i], '/');
            printf("%s\n", filename ? filename + 1 : argv[i]);
        }
    }
    return 0;
}
