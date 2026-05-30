# 🔐 Escape Room Zero

![C++](https://img.shields.io/badge/C++-00599C?style=flat&logo=cplusplus&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-Windows-blue)
![Status](https://img.shields.io/badge/Status-In%20Development-yellow)

> 의문의 공간에 홀로 갇힌 당신. 흩어진 단서들을 모으고 조합하여 탈출하라.

<p align="center">
  <img src="docs/screenshot.png" width="700"/>
</p>

---

## 목차
- [소개](#소개)
- [조작법](#조작법)
- [설치 방법](#설치-방법)
- [세이브 시스템](#세이브-시스템)
- [개발 현황](#개발-현황)
- [폴더 구조](#폴더-구조)

---

## 소개

1인칭 시점의 솔로 퍼즐 탈출 게임입니다.  
서버 없이 클라이언트 단독으로 동작하며,  
플레이어는 공간에 숨겨진 아이템을 수집하고 조합해 탈출을 목표로 합니다.

---

## 조작법

| 키 | 동작 |
|----|------|
| `W A S D` | 이동 |
| `E` | 아이템 상호작용 |
| `I` | 인벤토리 열기 |
| `ESC` | 일시정지 / 메뉴 |

---

## 설치 방법

1. 이 리포지토리를 클론합니다
```bash
   git clone https://github.com/yourname/escape-room-zero.git
```
2. Visual Studio 2022 이상으로 `EscapeRoom.sln` 을 열어주세요
3. **Release 모드**로 빌드 후 실행

<details>
<summary>빌드 오류가 날 때</summary>

- OpenGL 라이브러리 경로가 설정되어 있는지 확인
- `vcpkg install glfw3 glm` 실행 후 재빌드

</details>

---

## 세이브 시스템

진행 상황은 로컬 세이브 파일로 관리됩니다. 데이터베이스를 사용하지 않습니다.

```
save/
└── slot1.dat   ← 수집 아이템, 현재 위치, 퍼즐 진행도 저장
```

---

## 개발 현황

- [x] 플레이어 이동 및 시점 구현
- [x] 아이템 수집 시스템
- [x] 인벤토리 UI
- [x] 세이브 / 로드 기능
- [ ] 퍼즐 #3 구현 중
- [ ] 엔딩 시퀀스

---

## 폴더 구조

```
escape-room-zero/
├── src/
│   ├── main.cpp
│   ├── Player.cpp
│   ├── Inventory.cpp
│   └── SaveManager.cpp
├── assets/
│   ├── textures/
│   └── sounds/
├── save/
└── README.md
```
