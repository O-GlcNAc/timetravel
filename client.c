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
        switch(sensor_id) {
        case 1: // 당근
            temperature = rand() % 5 + 16; // 16°C - 20°C
            humidity = rand() % 11 + 60; // 60% - 70%
            break;
        case 2: // 상추
            temperature = rand() % 4 + 15; // 15°C - 18°C
            humidity = rand() % 11 + 70; // 70% - 80%
            break;
        case 3: // 가지
            temperature = rand() % 5 + 20; // 20°C - 24°C
            humidity = rand() % 11 + 60; // 60% - 70%
            break;
        }
        // int hour = tm_current->tm_hour;
        // if (hour >= 6 && hour < 18) { // 낮 시간
        //     // 선형 증가 광도 계산
        //     int max_luminance = (sensor_id == 1) ? 50 : (sensor_id == 2) ? 70 : 100;
        //     luminance = (hour - 6) * max_luminance / 12;
        // } else { // 밤 시간
        //     luminance = 0;
        // }
        luminance = 70;


        // FIFO로 sensor_id, reading, temperature, humidity, luminance, timestamp 데이터 전송
        dprintf(fd, "%d %d %d %d %d %s\n", sensor_id, reading, temperature, humidity, luminance, timestamp);

        // 현재 시간을 10분 증가시킴
        tm_current->tm_min += 10;
        current_time = mktime(tm_current); // 업데이트된 tm 구조체를 다시 time_t로 변환

        char updated_time_str[20];
        strftime(updated_time_str, sizeof(updated_time_str), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
        printf("Updated Time: %s\n", updated_time_str);

        sleep(1); // 1초 대기
        
    }

    close(fd);
    return 0;
}
