#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define FIFOFILE "fifo"

int main(int argc, char **argv) {
    int fd;
    time_t current_time = time(NULL); // 현재 시간으로 초기화
    char timestamp[20];

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

        // 현재 시간을 "YYYY-MM-DD HH:MM:SS" 형식으로 변환
        struct tm *tm_current = localtime(&current_time);
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_current);

        // 각 센서 ID에 따라 데이터 생성
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

        // FIFO로 sensor_id, reading, temperature, humidity, luminance, timestamp 데이터 전송
        dprintf(fd, "%d %d %d %d %d %s\n", sensor_id, reading, temperature, humidity, luminance, timestamp);

        // 현재 시간을 10분 증가시킴
        tm_current->tm_min += 10;
        current_time = mktime(tm_current); // 업데이트된 tm 구조체를 다시 time_t로 변환

        sleep(1); // 1초 대기
    }

    close(fd);
    return 0;
}
