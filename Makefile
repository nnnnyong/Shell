CC = gcc
CFLAGS = -Wall -g
SRCDIR = src
BINDIR = .

# src 디렉토리에 있는 모든 .c 파일을 찾습니다.
SRCS := $(wildcard $(SRCDIR)/*.c)

# 각 .c 파일에 대한 대응되는 실행 파일을 생성합니다.
BINS := $(patsubst $(SRCDIR)/%.c,$(BINDIR)/%,$(SRCS))

all: $(BINS)

$(BINDIR)/%: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS)

