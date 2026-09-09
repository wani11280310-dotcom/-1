#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_DICE 5
#define TOTAL_ROUNDS 5

// 족보 및 점수를 담는 구조체
typedef struct {
    char name[30];
    int score;
} HandResult;

// 주사위 굴리기 함수
void roll_dice(int dice[], int keep[]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (!keep[i]) {
            dice[i] = (rand() % 6) + 1;
        }
    }
}

// 주사위 상태 출력 함수
void print_dice(const int dice[]) {
    printf("  주사위 결과: [ ");
    for (int i = 0; i < NUM_DICE; i++) {
        printf("%d ", dice[i]);
    }
    printf("]\n");
}

// 연속된 눈의 개수 계산 (스트레이트 판정용)
int getMaxConsecutive(const int counts[]) {
    int max_consec = 0;
    int current_consec = 0;

    for (int i = 1; i <= 6; i++) {
        if (counts[i] > 0) {
            current_consec++;
            if (current_consec > max_consec) {
                max_consec = current_consec;
            }
        } else {
            current_consec = 0;
        }
    }
    return max_consec;
}

// 가장 높은 점수의 족보를 판정하는 함수
HandResult evaluate_hand(const int dice[]) {
    int counts[7] = {0}; // 1부터 6까지 눈의 개수
    for (int i = 0; i < NUM_DICE; i++) {
        counts[dice[i]]++;
    }

    HandResult best_hand = {"꽝 (0점)", 0};

    // 1. 요트 (5개 모두 같음): 50점
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 5) {
            if (50 > best_hand.score) {
                strcpy(best_hand.name, "요트 (Yacht)");
                best_hand.score = 50;
            }
        }
    }

    // 2. 스트레이트 (연속된 눈 4개 이상): 30점
    if (getMaxConsecutive(counts) >= 4) {
        if (30 > best_hand.score) {
            strcpy(best_hand.name, "스트레이트 (Straight)");
            best_hand.score = 30;
        }
    }

    // 3. 풀하우스 (3개 + 2개 또는 5개 동일도 포함): 25점
    int has_three = 0;
    int has_two = 0;
    for (int i = 1; i <= 6; i++) {
        if (counts[i] == 3) has_three = 1;
        if (counts[i] == 2) has_two = 1;
        if (counts[i] == 5) { has_three = 1; has_two = 1; } // 요트일 경우도 풀하우스 조건 충족
    }
    if (has_three && has_two) {
        if (25 > best_hand.score) {
            strcpy(best_hand.name, "풀하우스 (Full House)");
            best_hand.score = 25;
        }
    }

    // 4. 포카드 (같은 눈 4개 이상): 해당 눈 x 4
    for (int i = 1; i <= 6; i++) {
        if (counts[i] >= 4) {
            int score = i * 4;
            if (score > best_hand.score) {
                sprintf(best_hand.name, "포카드 (%d x 4)", i);
                best_hand.score = score;
            }
        }
    }

    // 5. 트리플 (같은 눈 3개 이상): 해당 눈 x 3
    for (int i = 1; i <= 6; i++) {
        if (counts[i] >= 3) {
            int score = i * 3;
            if (score > best_hand.score) {
                sprintf(best_hand.name, "트리플 (%d x 3)", i);
                best_hand.score = score;
            }
        }
    }

    // 6. 원페어 (같은 눈 2개 이상): 해당 눈 x 2
    for (int i = 1; i <= 6; i++) {
        if (counts[i] >= 2) {
            int score = i * 2;
            if (score > best_hand.score) {
                sprintf(best_hand.name, "원페어 (%d x 2)", i);
                best_hand.score = score;
            }
        }
    }

    return best_hand;
}

// 한 플레이어의 턴을 진행하는 함수
HandResult play_turn(int player_num, int round_num) {
    int dice[NUM_DICE] = {0};
    int keep[NUM_DICE] = {0}; // 0: 다시 굴림, 1: 보관

    printf("\n========================================\n");
    if (player_num > 0) {
        printf("   [ 라운드 %d - 플레이어 %d 턴 ]\n", round_num, player_num);
    } else {
        printf("   [ 라운드 %d ]\n", round_num);
    }
    printf("========================================\n");

    // 첫 번째 굴리기
    roll_dice(dice, keep);
    print_dice(dice);

    // 다시 굴리기 기회 (최대 2회)
    for (int reroll = 1; reroll <= 2; reroll++) {
        char choice;
        printf("\n주사위를 다시 굴리시겠습니까? (y/n): ");
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y') {
            for (int i = 0; i < NUM_DICE; i++) {
                int keep_input;
                printf(" %d번 주사위 (%d)를 고정하시겠습니까? (1: 고정, 0: 다시 굴림): ", i + 1, dice[i]);
                scanf("%d", &keep_input);
                keep[i] = keep_input;
            }
            roll_dice(dice, keep);
            printf("\n[%d차 다시 굴리기 결과]\n", reroll);
            print_dice(dice);
        } else {
            break; // 더 이상 안 굴림
        }
    }

    // 최종 족보 계산
    HandResult result = evaluate_hand(dice);
    printf("\n>> 최종 주사위: [ ");
    for (int i = 0; i < NUM_DICE; i++) printf("%d ", dice[i]);
    printf("]\n");
    printf(">> 달성 족보: %s\n", result.name);
    printf(">> 획득 점수: %d점\n", result.score);

    return result;
}

int main() {
    srand((unsigned int)time(NULL));

    printf("****************************************\n");
    printf("*        주사위 게임 「요트」          *\n");
    printf("****************************************\n");
    printf("1. 1인 싱글 모드\n");
    printf("2. 2인 대전 모드\n");
    printf("모드를 선택하세요 (1 또는 2): ");

    int mode = 1;
    scanf("%d", &mode);

    if (mode == 2) {
        // 2인 대전 모드
        int score_p1 = 0, score_p2 = 0;
        HandResult history_p1[TOTAL_ROUNDS];
        HandResult history_p2[TOTAL_ROUNDS];

        for (int r = 1; r <= TOTAL_ROUNDS; r++) {
            history_p1[r - 1] = play_turn(1, r);
            score_p1 += history_p1[r - 1].score;

            history_p2[r - 1] = play_turn(2, r);
            score_p2 += history_p2[r - 1].score;
        }

        // 최종 결과 출력
        printf("\n========================================\n");
        printf("            [ 최종 게임 결과 ]          \n");
        printf("========================================\n");
        printf("라운드 | 플레이어 1 (점수/족보)       | 플레이어 2 (점수/족보)\n");
        printf("--------------------------------------------------\n");
        for (int r = 0; r < TOTAL_ROUNDS; r++) {
            printf("  %d    | %2d점 (%-18s) | %2d점 (%-18s)\n",
                   r + 1,
                   history_p1[r].score, history_p1[r].name,
                   history_p2[r].score, history_p2[r].name);
        }
        printf("--------------------------------------------------\n");
        printf("총 점수 | 플레이어 1: %d점             | 플레이어 2: %d점\n", score_p1, score_p2);
        printf("========================================\n");

        if (score_p1 > score_p2) {
            printf("★ 승리: 플레이어 1 승리! ★\n");
        } else if (score_p2 > score_p1) {
            printf("★ 승리: 플레이어 2 승리! ★\n");
        } else {
            printf("★ 무승부입니다! ★\n");
        }

    } else {
        // 1인 싱글 모드
        int total_score = 0;
        HandResult history[TOTAL_ROUNDS];

        for (int r = 1; r <= TOTAL_ROUNDS; r++) {
            history[r - 1] = play_turn(0, r);
            total_score += history[r - 1].score;
        }

        // 최종 결과 출력
        printf("\n========================================\n");
        printf("            [ 최종 게임 결과 ]          \n");
        printf("========================================\n");
        printf(" 라운드  |   획득 점수   | 판정 족보\n");
        printf("----------------------------------------\n");
        for (int r = 0; r < TOTAL_ROUNDS; r++) {
            printf("   %d     |     %2d점     | %s\n", r + 1, history[r].score, history[r].name);
        }
        printf("----------------------------------------\n");
        printf(" 최종 총점: %d점\n", total_score);
        printf("========================================\n");
    }

    return 0;
}
