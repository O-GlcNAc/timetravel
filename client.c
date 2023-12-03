#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define FIFOFILE "fifo"

int main(int argc, char **argv) {
    int fd;
    srand(time(NULL)); // 난수 생성을 위한 시드 설정

    if ((fd = open(FIFOFILE, O_WRONLY)) < 0) { // FIFO를 쓰기 전용으로 열기
        perror("open()");
        return -1;
    }

    while (1) {
        int sensor_id = rand() % 3 + 1; // 1에서 3 사이의 랜덤 sensor_id 생성
        int reading;
        int humidity;
        int temperature;
        int luminance;
        if (sensor_id == 1) {
            reading = rand() % 61 - 20; // sensor_id 1의 reading은 -20에서 40 사이 값
            humidity = 10;
            temperature = 10;
            luminance = 10;
        } else if (sensor_id == 2) {
            reading = rand() % 101; // sensor_id 2의 reading은 0에서 100 사이 값
            humidity = 20;
            temperature = 20;
            luminance = 20;
        } else {
            reading = rand() % 21; // sensor_id 3의 reading은 0에서 20 사이 값
            humidity = 30;
            temperature = 30;
            luminance = 30;
        }

        // FIFO로 sensor_id와 reading 데이터 전송
        dprintf(fd, "%d %d\n", sensor_id, reading,humidity,temperature,luminance);

        sleep(1); // 1초 대기
    }

    close(fd);
    return 0;
}
