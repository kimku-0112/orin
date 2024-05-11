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

    // 데이터 구성
    struct canfd_frame frame;
    frame.can_id = 0x123;
    frame.len = 2;
    frame.flags = CANFD_BRS | CANFD_ESI;
    frame.data[0] = 0xAB;
    frame.data[1] = 0xCD;

    // 데이터 전송
    int bytes_sent = write(sock, &frame, sizeof(struct canfd_frame));
    if (bytes_sent < 0) {
        std::cerr << "데이터 전송 오류" << std::endl;
        return 1;
    }

    // 소켓 닫기
    close(sock);

    return 0;
}

