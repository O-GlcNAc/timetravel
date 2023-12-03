#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define FIFOFILE "fifo"

int main(int argc, char **argv) {
    int fd;
    struct tm start_time = { .tm_year = 123, .tm_mon = 11, .tm_mday = 3, .tm_hour = 0, .tm_min = 0, .tm_sec = 0 }; // 2023-12-03 00:00:00
    time_t current_time = mktime(&start_time);

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
        char timestamp[20];

        // 현재 시간을 "YYYY-MM-DD HH:MM:SS" 형식으로 변환
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&current_time));

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

        current_time += 600; // 10분 더하기
        sleep(1); // 1초 대기
    }

    close(fd);
    return 0;
}
