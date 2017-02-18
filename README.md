# MINT64-OS
![MINT64-OS](https://cloud.githubusercontent.com/assets/8179234/23093799/fb1ea424-f62f-11e6-8272-ecbea602bbf7.png)

Clone of [MINT64 OS](https://github.com/kkamagui/mint64os) for personal study.  
MINT64 OS is a project that making own 64bit multicore operating system.  

## Setup
macOS Sierra 10.12.2

```bash
brew install nasm
brew install qemu
```

Setup cross compile environment. - [wiki](https://github.com/L34p/MINT64-OS/wiki/Cross-compiler-setup-on-macOS)

## Build and Run
```bash
git clone https://github.com/L34p/MINT64-OS.git
cd MINT64-OS
make
./run.sh
```

## Progress
- [x] 1. 시작하기 전에
- [x] 2. OS 개발 환경을 구축하자
- [x] 3. 64비트 프로세서의 이모저모
- [x] 4. 내 PC를 부팅하자
- [x] 5. 플로피 디스크에서 OS 이미지를 로딩하자
- [x] 6. 32비트 보호 모드로 전환하자
- [x] 7. C언어로 커널 작성하기
- [x] 8. A20 게이트를 활성화하여 1MB이상 영역에 접근해보자
- [x] 9. 페이징 기능을 활성화하여 64비트 전환을 준비하자
- [x] 10. 64비트 모드로 전환하자
- [x] 11. 키보드 디바이스 드라이버를 추가하자
- [x] 12. GDT와 IDT 테이블, TSS 세그먼트를 추가하여 인터럽트를 대비하자
- [ ] 13. PIC 컨트롤러와 인터럽트 핸들러를 이용해 인터럽트를 처리하자
- [ ] 14. 키보드 디바이스 드라이버를 업그레이드하자
- [ ] 15. 콘솔 셸을 만들자
- [ ] 16. 타이머 디바이스 드라이버를 추가하자
- [ ] 17. 태스크 개념을 추가해 멀티태스킹을 구현하자
- [ ] 18. 라운드 로빈 스케줄러를 추가하자
- [ ] 19. 멀티레벨 큐 스케쥴러로 업그레이드하고 태스크 종료 기능을 추가하자
- [ ] 20. 태스크와 인터럽트, 태스크와 태스크 사이의 동기화 문제를 해결하자
- [ ] 21. 멀티스레딩 기능을 추가하자
- [ ] 22. 실수 연산 기능을 추가하자
- [ ] 23. 메모리를 내 마음대로 할당받자
- [ ] 24. 하드 디스크 디바이스 드라이버를 추가하자
- [ ] 25. 간단한 파일 시스템을 구현하자
- [ ] 26. C 표준 입출력 함수를 추가하자
- [ ] 27. 캐시와 램 디스크를 추가해 속도를 높이자
- [ ] 28. 시리얼 포트 디바이스 드라이버를 추가해 외부와 연결하자
- [ ] 29. 코어 개수와 시스템 구성 정보를 추룰하자
- [ ] 30. 잠자는 코어를 깨우자
- [ ] 31. 대칭 I/O 모드로 전환해 인터럽트 분산 처리에 대비하자
- [ ] 32. TODO...
