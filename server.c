#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define FIFOFILE "fifo"
#define PYTHON_SCRIPT "mydb.py" // 파이썬 스크립트 파일명

int main(int argc, char **argv) {
    int fd;
    char buf[BUFSIZ];

    unlink(FIFOFILE); // 기존 FIFO 파일 삭제

    if (mkfifo(FIFOFILE, 0666) < 0) { // 새로운 FIFO 파일 생성
        perror("mkfifo()");
        return -1;
    }

    if ((fd = open(FIFOFILE, O_RDONLY)) < 0) { // FIFO를 읽기 전용으로 열기
        perror("open()");
        return -1;
    }

    FILE *fp;
    char command[BUFSIZ];

    while (1) {
        int n = read(fd, buf, sizeof(buf)); // FIFO로부터 데이터를 읽음

        if (n > 0) {
            buf[n] = '\0'; // Null-terminate 문자열

                 // 타임스탬프 추출 및 출력
            char *last_token = strrchr(buf, ' ');
            if (last_token != NULL) {
                printf("Timestamp: %s\n", last_token + 1);
            }

            // 파이썬 스크립트를 호출하여 FIFO로부터 받은 데이터를 전달하고 MariaDB에 데이터 삽입
            sprintf(command, "python3 %s \"%s\"", PYTHON_SCRIPT, buf);
            fp = popen(command, "r");
            if (fp == NULL) {
                printf("Failed to execute Python script.\n");
                return -1;
            }
            char result_buffer[1024]; // 출력 결과를 담을 버퍼

            while (fgets(result_buffer, sizeof(result_buffer), fp) != NULL) {
                printf("%s", result_buffer); // 파이썬 스크립트의 출력 결과를 터미널에 출력
            }

            pclose(fp);
        }
    }

    close(fd);
    return 0;
}
