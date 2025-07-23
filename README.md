# HW08
[8번 과제] 게임 루프 및 UI 재설계하기

실제 구현 내용을 간략하게 정리함
## 📌 필수 기능
### 멀티 웨이브 구조 구현
- 현재 프로젝트는 3번의 레벨 전환이 이루어짐

  - 한 레벨 내에서 최소 3단계 이상의 웨이브(Wave 1, Wave 2, Wave 3)를 구현

- 각 웨이브 시작 시 “Wave X 시작!” 메시지를 UE_LOG 또는 GEngine->AddOnScreenDebugMessage를 통해 출력

### HUD & 메뉴 UI 리뉴얼
- HUD에 점수, 시간, 체력 정보를 모두 한 화면에 표시되도록 배치

- 메인 메뉴(시작, 종료), 게임 오버 메뉴(재시작, 메인 메뉴로 돌아가기)를 새로 디자인 함

- 폰트, 색상, 배경 등을 조정하여 UI 스타일 구성

- 각 버튼은 C++ 또는 블루프린트로 연결되어 실제 레벨 이동 또는 게임 종료 기능을 수행

## 💡 도전 기능
### 아이템 상호작용 로직 고도화
- 새로운 부정적 아이템 추가

  - SlowingItem: 일정 시간 동안 이동 속도가 50% 감소

  - ReverseControlItem: 일정 시간 동안 이동 키 방향 반전
 
## 🎥 시연 영상
[![Video Label](https://img.youtube.com/vi/ObXoFsDbEGY/0.jpg)](https://youtu.be/ObXoFsDbEGY)

