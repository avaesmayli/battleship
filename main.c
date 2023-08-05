#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

struct node {
    int start_x;
    int end_x;
    int start_y;
    int end_y;
    struct node* next ;
};
typedef struct node node ;
node* head ;
void init() {
    head = NULL ;
}
void append(node** Head , int start_y , int start_x , int end_x , int end_y) {
    node* new = (node*)malloc(sizeof(node)) ;
    new->start_x = start_x ;
    new->start_y = start_y ;
    new->end_x= end_x ;
    new->end_y = end_y ;
    new->next = *Head ;
    *Head = new ;
}
void delete(node** head , int start_y , int start_x , int end_x , int end_y) {
    node* cur = *head , *prev;
    if (cur != NULL && (cur->start_x == start_x && cur->start_y == start_y && cur->end_x == end_x && cur->end_y == end_y)) {
        *head = cur->next;
        free(cur);
        return;
    }
    while (cur != NULL && !(cur->start_x == start_x && cur->start_y == start_y && cur->end_x == end_x && cur->end_y == end_y)) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL)
        return;
    prev->next = cur->next;
    free(cur);
}

int flag ;
char board_player1[100][100];
char board_player2[100][100];
char map_res1[100][100] ;
char map_res2[100][100] ;
int point_player1=0 , point_player2=0 ;
char V[100] ;
char H[100][100] ;
void map_player(int size , char board[100][100]) ;

void complete_ship(node** head , char board_straight[100][100] , int *point) {
    int temp, num = 0;
    node* curr = *head ;
    while( curr != NULL) {
        if (curr->start_x - curr->end_x == 0 && curr->end_y - curr->start_y != 0) {
            if (curr->start_y > curr->end_y) {
                temp = curr->start_y;
                curr->start_y = curr->end_y;
                curr->end_y = temp;
            }
            int x = curr->start_x;
            int len = curr->end_y - curr->start_y;
            for (int i = curr->start_y; i <= curr->end_y; i++) {
                if (board_straight[i][x] == 'E') {
                    num++;
                    if (num == len) {
                        for (int j = curr->start_y; j <= curr->end_y; j++) {
                            board_straight[j][x] = 'C';
                            delete(&head , curr->start_y , curr->start_x , curr->end_x , curr->end_y) ;
                            if (len == 5) {
                                *point = +5;
                            } else if (len == 3) {
                                *point = +8;
                            } else if (len == 2) {
                                *point = +12;
                            }
                        }
                        board_straight[curr->start_y - 1][x] = 'W';
                        board_straight[curr->end_y + 1][x] = 'W';
                        for (int h = curr->start_y; h <= curr->end_y; h++) {
                            board_straight[h][x + 1] = 'W';
                            board_straight[h][x - 1] = 'W';
                        }
                        board_straight[curr->end_y + 1][x + 1] = 'W';
                        board_straight[curr->end_y + 1][x - 1] = 'W';
                        board_straight[curr->start_y - 1][x + 1] = 'W';
                        board_straight[curr->start_y - 1][x - 1] = 'W';
                        map_player(10, board_straight);
                    }
                }
            }
        }
        if (curr->start_x - curr->end_x != 0 && curr->end_y - curr->start_y == 0) {
            if (curr->start_x > curr->end_x) {
                temp = curr->start_x;
                curr->start_x = curr->end_x;
                curr->end_x = temp;
            }
            int y = curr->start_y;
            int len = curr->end_x - curr->start_x;
            for (int i = curr->start_x; i <= curr->end_x; i++) {
                if (board_straight[y][i] == 'E') {
                    num++;
                    if (num == len) {
                        for (int j = curr->start_x; j <= curr->end_x; j++) {
                            board_straight[y][j] = 'C';
                            delete(&head , curr->start_y , curr->start_x , curr->end_x , curr->end_y) ;
                            if (len == 5) {
                                *point = +5;
                            } else if (len == 3) {
                                *point = +8;
                            } else if (len == 2) {
                                *point = +12;
                            }
                        }
                        board_straight[y][curr->start_x - 1] = 'W';
                        board_straight[y][curr->end_x + 1] = 'W';
                        for (int h = curr->start_y; h <= curr->end_y; h++) {
                            board_straight[y + 1][h] = 'W';
                            board_straight[y - 1][h] = 'W';
                        }
                        board_straight[y + 1][curr->start_x - 1] = 'W';
                        board_straight[y - 1][curr->start_x - 1] = 'W';
                        board_straight[y + 1][curr->end_x + 1] = 'W';
                        board_straight[y - 1][curr->end_x + 1] = 'W';
                        map_player(10, board_straight);
                    }
                }
            }
        }
        if (curr->start_x - curr->end_x == 0 && curr->end_y - curr->start_y == 0) {
            if (board_straight[curr->start_y][curr->start_x] == 'E') {
                board_straight[curr->start_y][curr->start_x] = 'C';
                delete(&head , curr->start_y , curr->start_x , curr->end_x , curr->end_y) ;
                *point = +25;
                int x = curr->start_x;
                int y = curr->start_y;
                board_straight[y + 1][x] = 'W';
                board_straight[y - 1][x] = 'W';
                board_straight[y][x + 1] = 'W';
                board_straight[y][x - 1] = 'W';
                board_straight[y + 1][x + 1] = 'W';
                board_straight[y - 1][x - 1] = 'W';
                board_straight[y + 1][x - 1] = 'W';
                board_straight[y - 1][x + 1] = 'W';
                map_player(10, board_straight);
            }
        }
        curr=curr->next ;
    }
}
int play_game(int x , int y , char board_straight[100][100] , char res[100][100] , int *point) {
    printf("\n");
    if (board_straight[y][x] == 'w' || board_straight[y][x] == '_') {
        res[y][x] = 'W';
        map_player(10 , res) ;
        return 1 ;
    } else if (board_straight[y][x] == 's') {
        res[y][x] = 'E';
        complete_ship(&head , board_straight , point) ;
        *point=+1 ;
        printf("you have %d point" , (*point)) ;
        map_player(10 , res ) ;
        return 2 ;
    } else {
        return 0;
    }
}
int put_ship(int start_x , int end_x , int start_y , int end_y , char board_straight[100][100]){
    int temp ;
    if(start_x-end_x == 0 && end_y-start_y != 0){
        if(start_y > end_y){
            temp=start_y;
            start_y=end_y ;
            end_y=temp ;
        }
        int x=start_x ;
        for(int i=start_y ; i<=end_y ; i++ ){
            if(board_straight[i][x] == 0 ) {
                board_straight[i][x] = 's';
                append(&head, start_y, start_x, end_x, end_y);
                board_straight[start_y - 1][x] = 'w';
                board_straight[end_y + 1][x] = 'w';
                for (int j = start_y; j <= end_y; j++) {
                    board_straight[j][x + 1] = 'w';
                    board_straight[j][x - 1] = 'w';
                }
                board_straight[end_y + 1][x + 1] = 'w';
                board_straight[end_y + 1][x - 1] = 'w';
                board_straight[start_y - 1][x + 1] = 'w';
                board_straight[start_y - 1][x - 1] = 'w';
            }else {
                return 0 ;
            }
        }
    }
    if(start_y-end_y == 0 && start_x-end_x != 0){
        if(start_x > end_x){
            temp=start_x;
            start_x=end_x ;
            end_x=temp ;
        }
        int y=start_y ;
        for(int i=start_x ; i<=end_x ; i++ ){
            if(board_straight[y][i] ==  0) {
                board_straight[y][i] = 's';
                append(&head, start_y, start_x, end_x, end_y);
                board_straight[y][start_x - 1] = 'w';
                board_straight[y][end_x + 1] = 'w';
                for (int j = start_x; j <= end_x; j++) {
                    board_straight[y + 1][j] = 'w';
                    board_straight[y - 1][j] = 'w';
                }
                board_straight[y + 1][start_x - 1] = 'w';
                board_straight[y - 1][start_x - 1] = 'w';
                board_straight[y + 1][end_x + 1] = 'w';
                board_straight[y - 1][end_x + 1] = 'w';
            }else{
                return 0 ;
            }
        }
    }
    if(start_x-end_x == 0 && start_y-end_y == 0){
        if(board_straight[start_y][start_x]==0) {
            board_straight[start_y][start_x] = 's';
            append(&head, start_y, start_x, end_x, end_y);
            int x = start_x;
            int y = start_y;
            board_straight[y + 1][x] = 'w';
            board_straight[y - 1][x] = 'w';
            board_straight[y][x + 1] = 'w';
            board_straight[y][x - 1] = 'w';
            board_straight[y + 1][x + 1] = 'w';
            board_straight[y - 1][x - 1] = 'w';
            board_straight[y + 1][x - 1] = 'w';
            board_straight[y - 1][x + 1] = 'w';
        }else{return 0 ;}
    }
    // random return statement
    return 1 ;
}

void random_ship(int size , char board_straight[100][100]) {
    int l1 = 0, l2 = 0, l3 = 0, l5 = 0, length;
    while (1) {
        printf("\n");
        while (1){
            srand(time(0));
            int start_coordinate[2];
            int end_coordinate[2];
            int x = rand() % 2;

            if (x == 0) {
                while (1) {
                    start_coordinate[0] = rand() % size;
                    end_coordinate[0] = start_coordinate[0];
                    start_coordinate[1] = rand() % size;
                    end_coordinate[1] = fabs(start_coordinate[1] - 4);
                    if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                 board_straight) != 0) {
                        break;
                    }
                }
            } else if (x == 1) {
                while (1){ start_coordinate[1] = rand() % size;
                    end_coordinate[1] = start_coordinate[1];
                    start_coordinate[0] = rand() % size;
                    end_coordinate[0] = fabs(start_coordinate[0] - 4);
                    if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                 board_straight) != 0) {
                        break;
                    }
                }

            }
            break;
        }
        printf("lol\n");
        while (1){
            if (l3<2)
            {
                srand(time(0));
                int start_coordinate[2];
                int end_coordinate[2];
                int x = rand() % 2;

                if (x == 0) {
                    while (1) {
                        start_coordinate[0] = rand() % size;
                        end_coordinate[0] = start_coordinate[0];
                        start_coordinate[1] = rand() % size;
                        end_coordinate[1] = fabs(start_coordinate[1] - 2);
                        if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                     board_straight) != 0) {
                            l3++;
                            break;
                        }
                    }
                } else if (x == 1) {
                    while (1){ start_coordinate[1] = rand() % size;
                        end_coordinate[1] = start_coordinate[1];
                        start_coordinate[0] = rand() % size;
                        end_coordinate[0] = fabs(start_coordinate[0] - 2);
                        if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                     board_straight) != 0) {
                            l3++;
                            break;
                        }
                    }

                }
            }

            else break;
        }
        printf("lolo\n");
        while (1){
            if (l2<3)
            {
                srand(time(0));
                int start_coordinate[2];
                int end_coordinate[2];
                int x = rand() % 2;

                if (x == 0) {
                    while (1){
                        start_coordinate[0] = rand() % size;
                        end_coordinate[0] = start_coordinate[0];
                        start_coordinate[1] = rand() % size;
                        end_coordinate[1] = fabs(start_coordinate[1] - 1);
                        if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                     board_straight) != 0) {
                            l2++ ;
                            break;
                        }
                    }

                } else if (x == 1) {
                    while (1){ start_coordinate[1] = rand() % size;
                        end_coordinate[1] = start_coordinate[1];
                        start_coordinate[0] = rand() % size;
                        end_coordinate[0] = fabs(start_coordinate[0] - 1);
                        if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                     board_straight) != 0) {
                            l2++;
                            break;
                        }
                    }

                }
            }

            else break;
        }
        printf("lolol\n");
        while (1){
            if (l1<4)
            {
                srand(time(0));
                int start_coordinate[2];
                int end_coordinate[2];
                int x = rand() % 2;

                if (x == 0) {
                    while (1){start_coordinate[0] = rand() % size;
                        end_coordinate[0] = start_coordinate[0];
                        start_coordinate[1] = rand() % size;
                        end_coordinate[1] = fabs(start_coordinate[1] );
                        if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                     board_straight) != 0) {
                            l1++ ;
                            break;
                        }}

                } else if (x == 1) {
                    while (1){ start_coordinate[1] = rand() % size;
                        end_coordinate[1] = start_coordinate[1];
                        start_coordinate[0] = rand() % size;
                        end_coordinate[0] = fabs(start_coordinate[0] );
                        if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                     board_straight) != 0) {
                            l1++;
                            break;
                        }
                    }

                }
            }

            else break;
        }
        map_player( size , board_straight) ;
        break;
    }
}

void ship_place(int size, char board_straight[100][100]) {
    int loop = 0, l1 = 0, l2 = 0, l3 = 0, l5 = 0, length;
    while (loop < 10) {
        while (1) {
            printf("\n");
            printf("Enter the coordinate :");
            int start_coordinate[2];
            int end_coordinate[2];
            scanf("%d,%d", &start_coordinate[0], &start_coordinate[1]);
            scanf("%d,%d", &end_coordinate[0], &end_coordinate[1]);
            if (start_coordinate[0] > size || start_coordinate[1] > size || end_coordinate[0] > size ||
                end_coordinate[1] > size) {
                printf("your coordinate is out of the map");
            }
            if (start_coordinate[1] != end_coordinate[1] && start_coordinate[0] != end_coordinate[0]) {
                printf("your coordinate is wrong\n");
            } else {
                if (start_coordinate[1] == end_coordinate[1] && start_coordinate[0] != end_coordinate[0]) {
                    length = fabs(start_coordinate[0] - end_coordinate[0]) + 1;
                } else if (start_coordinate[1] != end_coordinate[1] && start_coordinate[0] == end_coordinate[0]) {
                    length = fabs(start_coordinate[1] - end_coordinate[1]) + 1;
                } else if (start_coordinate[1] == end_coordinate[1] && start_coordinate[0] == end_coordinate[0]) {
                    length = 1;
                }
                if (length == 1) {
                    l1++;
                    if (l1 <= 4) {
                        if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                     board_straight) != 0) {
                            map_player(10 , board_straight) ;
                            loop++;
                            break;
                        }
                    } else {
                        printf("you dont have a ship with length of 1");
                    }
                } else if (length == 2) {
                    l2++;
                    if (l2 <= 3) {
                        if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                     board_straight) != 0) {
                            map_player(10 , board_straight) ;
                            loop++;
                            break;
                        }
                    } else {
                        printf("you dont have a ship with length of 2");
                    }
                } else if (length == 3) {
                    l3++;
                    if (l3 <= 2) {
                        if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                     board_straight) != 0) {
                            map_player(10 , board_straight) ;
                            loop++;
                            break;
                        }
                    } else {
                        printf("you dont have a ship with length of 3");
                    }
                } else if (length == 5) {
                    l5++;
                    if (l5 <= 1) {
                        if (put_ship(start_coordinate[0], end_coordinate[0], start_coordinate[1], end_coordinate[1],
                                     board_straight) != 0) {
                            map_player(10 , board_straight) ;
                            loop++;
                            break;
                        }
                    } else {
                        printf("you dont have a ship with length of 5");
                    }
                }
            }
        }
    }
}
void map_player(int size , char board[100][100]){
    for (int number = 0; number < size; number++) {
        if (number>9){
            printf("      %d",number);
        }else printf("       %d",number);
    }
    printf("\n");

    for (int i = 0; i < size; i++) {
        printf("   ");
        for (int j = 0; j < (size*10)-(size*2-1); j++) {
            H[i][j] = '_';
            printf("%c", H[i][j]);
        }
        printf("\n");
        if(i>size)break;
        printf("   ");
        for (int n = 0; n < size; ++n) {
            V[n]='|';
            printf("%c       ",V[n]);
        }
        printf("\n");
        if (i>9){
            printf("%d ",i);
        }else printf("%d  ",i);
        for (int y = 0; y < size; y++) {
            V[y]='|';
            printf("%c   ",V[y]);
            if ( !flag)
                board[i][y]='_' ;
            printf("%c   ",board[i][y]);
        }
        printf("\n");
        printf("   ");
        for (int h = 0; h < size; h++) {
            V[h]='|';
            printf("%c       ",V[h]);
        }
        printf("\n");
    }
    printf("\n");
}
int menu() {
    int choice, size_map = 10;
    char player1[100], player2[100];
    printf("1)play with a friend\n 2)play with bot\n 3)load game\n 4)load last game\n 5)settings\n 6)score board\n 7)exit\n");
    scanf("%d", &choice);
    if (choice == 1) {
        printf("first player :\n");
        //choice 1
        printf("Enter the user of player1 :");
        scanf("%s", player1);
        printf("put ships : \n");
        printf("1)auto\n 2)manual");
        scanf("%d", &choice);
        if (choice == 1) {
            random_ship(size_map, board_player1);
            map_player(size_map, board_player1);
        } else if (choice == 2) {
            printf("choose the place of ship :\n");
            printf("you have 1 ship with length 5 , 2 ship with length 3 , 3 ship with length 2 and 4 ship with length 1\n");
            map_player(size_map, board_player1);
            ship_place(size_map, board_player1);
            map_player(size_map, board_player1);
        }
        printf("second player :\n");
        //choice 2
        printf("Enter the user of player2 :");
        scanf("%s", player2);
        printf("1)auto\n 2)manual");
        scanf("%d", &choice);
        if (choice == 1) {
            random_ship(size_map, board_player2);
            map_player(size_map, board_player2);
        } else if (choice == 2) {
            printf("choose the place of ship :\n");
            printf("you have 1 ship with length 5 , 2 ship with length 3 , 3 ship with length 2 and 4 ship with length 1\n");
            map_player(size_map, board_player2);
            ship_place(size_map, board_player2);
        }
        return 1;
    } else if (choice == 2) {
        printf("1)choose user\n 2)put ships\n");
        scanf("%d", &choice);
        if (choice == 1) {
            printf("1)choose from available users\n 2)new user\n");
            scanf("%d", &choice);
            //choice 1
            if (choice == 2) {
                printf("Enter the user of player1 :");
                scanf("%s", player1);
                printf("1)choose user\n 2)put ships\n");
                scanf("%d", &choice);
                if (choice == 2) {
                    printf("1)auto\n 2)manual");
                    scanf("%d", &choice);
                    if (choice == 1) {
                        random_ship(size_map, board_player1);
                        map_player(size_map, board_player1);
                    } else if (choice == 2) {
                        printf("choose the place of ship :\n");
                        printf("you have 1 ship with length 5 , 2 ship with length 3 , 3 ship with length 2 and 4 ship with length 1\n");
                        map_player(size_map, board_player1);
                        ship_place(size_map, board_player1);
                        map_player(size_map, board_player1);
                    }
                }
            } else if (choice == 2) {
                printf("1)auto\n 2)manual");
                scanf("%d", &choice);
                if (choice == 1) {
                    random_ship(size_map, board_player1);
                    map_player(size_map, board_player1);
                } else if (choice == 2) {
                    printf("choose the place of ship :\n");
                    printf("you have 1 ship with length 5 , 2 ship with length 3 , 3 ship with length 2 and 4 ship with length 1\n");
                    map_player(size_map, board_player1);
                    ship_place(size_map, board_player1);
                    map_player(size_map, board_player1);
                }
            }
        }
        return 2;
    } else if (choice == 5) {
        printf("Enter the size of map :");
        scanf("%d", &size_map);
        return 5;
    }
}
int main() {
    int x, y , map_size=10;
    init();
    flag = 1;
    if (menu() == 1) {
        while (1) {
            while (1) {
                printf("\n");
                printf("choose your goal(player 1):");
                scanf("%d,%d", &x, &y);
                if (play_game(x, y, board_player2, map_res1, &point_player1) == 1) {
                    break;
                } else if (play_game(x, y, board_player2, map_res1, &point_player1) == 0) {
                    printf("your goal is wrong , choose another goal :\n");
                } else if (play_game(x, y, board_player2, map_res1, &point_player1) == 2) {
                    printf("you find a part of ship , use your gift\n");
                }
            }
            while (1) {
                printf("\n");
                printf("choose your goal(player 2):");
                scanf("%d,%d", &x, &y);
                if (play_game(x, y, board_player1, map_res2, &point_player2) == 1) {
                    break;
                } else if (play_game(x, y, board_player1, map_res2, &point_player2) == 0) {
                    printf("your goal is wrong , choose another goal :\n");
                } else if (play_game(x, y, board_player1, map_res2, &point_player2) == 2) {
                    printf("you find a part of ship , use your gift\n");
                }
            }
            if (head == NULL) { break; }
        }
    }
    if (menu() == 2) {
        while (1) {
            while (1) {
                printf("\n");
                printf("choose your goal(player 1):");
                scanf("%d,%d", &x, &y);
                if (play_game(x, y, board_player2, map_res1, &point_player1) == 1) {
                    break;
                } else if (play_game(x, y, board_player2, map_res1, &point_player1) == 0) {
                    printf("your goal is wrong , choose another goal :\n");
                } else if (play_game(x, y, board_player2, map_res1, &point_player1) == 2) {
                    printf("you find a part of ship , use your gift\n");
                }
            }
            while (1) {
                printf("\n");
                srand(time(0)) ;
                x=rand() % map_size ;
                y=rand() % map_size ;
                if (play_game(x, y, board_player1, map_res2, &point_player2) == 1) {
                    break;
                } else if (play_game(x, y, board_player1, map_res2, &point_player2) == 0) {
                    printf("your goal is wrong , choose another goal :\n");
                } else if (play_game(x, y, board_player1, map_res2, &point_player2) == 2) {
                    printf("you find a part of ship , use your gift\n");
                }
            }
            if (head == NULL) { break; }
        }
    }
    return 0;
}