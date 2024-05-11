#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int main() {
    // 소켓 생성
    int sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (sock < 0) {
        std::cerr << "소켓 생성 오류" << std::endl;
        return 1;
    }

    // CAN FD 활성화
    setsockopt(sock, SOL_CAN_RAW, CAN_RAW_FD_FRAMES, &sock, sizeof(sock));

    // 인터페이스 설정
    struct ifreq ifr;
    strcpy(ifr.ifr_name, "can0");
    if (ioctl(sock, SIOCGIFINDEX, &ifr) < 0) {
        std::cerr << "인터페이스 설정 오류" << std::endl;
        return 1;
    }

    // 소켓 바인딩
    struct sockaddr_can addr;
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        std::cerr << "소켓 바인딩 오류" << std::endl;
        return 1;
    }

    // 데이터 수신 및 출력
    struct canfd_frame frame;
    while (true) {
        int bytes_read = read(sock, &frame, sizeof(struct canfd_frame));
        if (bytes_read > 0) {
            std::cout << "ID: " << std::hex << frame.can_id << " 데이터: ";
            for (int i = 0; i < frame.len; ++i) {
                std::cout << std::hex << static_cast<int>(frame.data[i]) << " ";
            }
            std::cout << std::endl;
        }
    }

    // 소켓 닫기
    close(sock);

    return 0;
}

