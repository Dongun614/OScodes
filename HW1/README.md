# 명령어 출력 검색 도구

이 프로그램은 특정 명령어의 출력 결과에서 지정된 단어를 검색하고, 결과를 다양한 형식으로 표시할 수 있는 서비스입니다.

## 기본 기능
- 명령어 실행 결과에서 특정 단어 검색
- 검색된 단어를 빨간색으로 강조 표시

## 추가 기능
- 다양한 출력 옵션 지원:
  - `-n`: 매치된 줄 번호만 표시
  - `-l`: 매치가 있는 경우 명령어 이름만 한 번 표시
  - `-c`: 총 매치 개수만 표시
  - `-i`: 대소문자 구분 없이 검색

## 사용법
./findword [-n] [-l] [-c] [-i] <command> <findword>

-n, -l, -c, -i 는 옵션입니다.

### 예시

1. 기본 사용법 (단어 강조 표시):
   ./findword <command> <findword>

2. 대소문자 구분 없이 검색 (-i 이용):
   ./findword -i <command> <findword>

3. 매치 개수만 보기 (-c 이용):
   ./findword -c <command> <findword>

4. 매치된 줄 번호만 보기 (-n 이용):
   ./findword -n <command> <findword>

5. 매치가 있는 파일 이름만 보기 (-l 이용):
   ./findword -l <command> <findword>

## 컴파일 방법

## Makefile 설명
이 프로젝트의 Makefile은 다음과 같이 동작합니다.

### 1. 변수 정의
- CC = gcc
  C 컴파일러로 `gcc`를 사용합니다.
- CFLAGS = -Wall -g  
  컴파일 옵션으로, 모든 주요 경고 메시지를 출력하고 디버깅 정보를 포함합니다.
- TARGET = findword  
  최종 실행 파일의 이름을 `findword`로 지정합니다.
- OBJS = main.o findword.o  
  컴파일 과정에서 생성되는 오브젝트 파일 목록입니다.

### 2. 타겟 정의

- all: $(TARGET)  
  `make` 명령어로 기본적으로 실행되는 타겟입니다. `findword` 실행 파일을 만듭니다.

- $(TARGET): $(OBJS)  
  `findword` 실행 파일을 만들기 위해 필요한 오브젝트 파일들을 링크합니다.
  ```
  gcc -Wall -g -o findword main.o findword.o
  ```

- main.o: main.c findword.h  
  `main.o` 오브젝트 파일을 컴파일합니다. 헤더 파일(`findword.h`)이 변경되어도 재컴파일됩니다.
  ```
  gcc -Wall -g -c main.c
  ```

- findword.o: findword.c findword.h  
  `findword.o` 오브젝트 파일을 컴파일합니다.
  ```
  gcc -Wall -g -c findword.c
  ```

---

### 3. clean 타겟

- clean:  
  실행 파일과 오브젝트 파일을 삭제합니다.
  ```
  rm -f findword main.o findword.o
  ```
  `make clean` 명령어로 실행할 수 있습니다.

---

### 4. 사용 예시

```bash
make          # 실행 파일 컴파일
make clean    # 실행 파일 및 오브젝트 파일 삭제
```

## 필요 라이브러리

프로그램에 필요한 모든 라이브러리는 표준 C 라이브러리에 포함되어 있으므로 추가 설치는 필요하지 않습니다. 다만 아래 헤더파일들을 통해 구현됩니다.
- stdio.h
- stdlib.h
- unistd.h
- sys/types.h
- sys/wait.h
- fcntl.h
- getopt.h
- ctype.h

## 작동 원리

이 프로그램은 파이프와 프로세스 포크를 사용하여 명령어를 실행하고, 그 출력을 읽어서 지정된 단어를 검색합니다. 검색 결과는 사용자 옵션에 따라 다양한 형식으로 출력됩니다.