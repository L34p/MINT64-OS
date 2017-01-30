# MINT64-OS
![MINT64-OS](https://cloud.githubusercontent.com/assets/8179234/22420590/7dc3e1c8-e726-11e6-9224-6272fccadafe.png)    

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
- [ ] 10. 64비트 모드로 전환하자
- [ ] 11. 키보드 디바이스 드라이버를 추가하자
- [ ] 12. GDT와 IDT 테이블, TSS 세그먼트를 추가하여 인터럽트를 대비하자
- [ ] 13. PIC 컨트롤러와 인터럽트 핸들러를 이용해 인터럽트를 처리하자
- [ ] 14. 키보드 디바이스 드라이버를 업그레이드하자
- [ ] 15. 콘솔 셸을 만들자
- [ ] 16. 타이머 디바이스 드라이버를 추가하자
- [ ] 17. 태스크 개념을 추가해 멀티태스킹을 구현하자
- [ ] 18. 라운드 로빈 스케줄러를 추가하자
- [ ] 19. 멀티레벨 큐 스케쥴러로 업그레이드하고 태스크 종료 기능을 추가하자
- [ ] 20. TODO...
