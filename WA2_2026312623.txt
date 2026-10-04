/*
 * DASF004 (2026-2) · 실습#2 — 습관 트래커 ②: 빈 달력을 그리고, 함수로 쪼갠다
 * ─────────────────────────────────────────────────────────
 * 학번을 붙여 WA2_학번.c 로 저장하고, 제출할 때 확장자를 .txt 로 바꾸세요.
 * 빌드: gcc -Wall -Wextra -std=c11 WA2_학번.c -o wa2
 *
 * [TODO 요구사항 1] ~ [TODO 요구사항 4] — 함수 다섯 개의 본문을 채우면 완성입니다(요구사항 2 가 두 개).
 *   · main 과 함수 선언부는 [수정 금지] 입니다.
 *   · 배열·문자열·구조체·포인터는 쓰지 않습니다. (아직 안 배웠습니다)
 *   · (void)lo; 같은 줄은 "아직 안 쓰는 값" 표시입니다 — 구현을 시작하면 지우세요.
 */
#include <stdio.h>

/* ── [수정 금지] 함수 선언 ─────────────────────────────── */
int  readIntInRange(int lo, int hi);
int  isLeapYear(int year);
int  daysInMonth(int year, int month);
int  firstWeekdayOf(int year, int month);
void printCalendar(int firstWeekday, int days);

/* ── [TODO 요구사항 1] 범위 안의 값이 들어올 때까지 다시 묻는다 ──── */
int readIntInRange(int lo, int hi)
{
    int val = 0;
    scanf("%d", &val);
    
    while (val < lo || val > hi) {
        printf("[오류] %d~%d 사이의 값을 입력하세요.\n", lo, hi);
        printf("> ");
        scanf("%d", &val);
    }
    
    return val;
}

/* ── [TODO 요구사항 2-a] 윤년이면 1, 아니면 0 을 반환 ───────────── */
int isLeapYear(int year)
{
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
        return 1;
    }
    return 0;
}

/* ── [TODO 요구사항 2-b] 그 달의 일수를 반환 ────────────────────── */
int daysInMonth(int year, int month)
{
    if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else if (month == 2) {
        if (isLeapYear(year)) {
            return 29;
        } else {
            return 28;
        }
    } else {
        return 31;
    }
}

/* ── [TODO 요구사항 3] 그 달 1일의 요일 (0=일 … 6=토) ───────────── */
int firstWeekdayOf(int year, int month)
{
    int totalDays = 0;
    
    /* 1. 2000년부터 (year - 1)년까지 경과 일수 누적 */
    for (int y = 2000; y < year; y++) {
        if (isLeapYear(y)) {
            totalDays += 366;
        } else {
            totalDays += 365;
        }
    }
    
    /* 2. 그 해 1월부터 (month - 1)월까지 경과 일수 누적 */
    for (int m = 1; m < month; m++) {
        totalDays += daysInMonth(year, m);
    }
    
    /* 2000년 1월 1일(토요일, 요일코드=6) 기준 요일 계산 */
    return (6 + totalDays) % 7;
}

/* ── [TODO 요구사항 4] 달력 격자 출력 ───────────────────────────── */
void printCalendar(int firstWeekday, int days)
{
    int dayCount = 1;
    
    /* 바깥 반복문: 주(Week) 단위 출력 */
    while (dayCount <= days) {
        /* 안쪽 반복문: 요일(일~토, 7칸) 단위 출력 */
        for (int col = 0; col < 7; col++) {
            if (dayCount == 1 && col < firstWeekday) {
                /* 1일이 시작하기 전 빈칸 */
                printf("    ");
            } else if (dayCount <= days) {
                /* 날짜 출력 */
                printf("%3d ", dayCount);
                dayCount++;
            } else {
                /* 말일 이후 남아있는 빈칸 */
                printf("    ");
            }
        }
        printf("\n"); /* 7칸 다 그리면 줄바꿈 */
    }
}

/* ── [수정 금지] ─────────────────────────────────────────── */
int main(void)
{
    int year = 0, month = 0, days = 0, firstWeekday = 0;

    printf("=== 습관 트래커 ===\n");
    printf("연도를 입력하세요 (2000~2100): ");
    year = readIntInRange(2000, 2100);
    printf("월을 입력하세요 (1~12): ");
    month = readIntInRange(1, 12);

    days = daysInMonth(year, month);
    firstWeekday = firstWeekdayOf(year, month);

    printf("\n         %d년 %d월\n", year, month);
    printf("  일  월  화  수  목  금  토\n");
    printCalendar(firstWeekday, days);

    return 0;
}