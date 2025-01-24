#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "rlutil.h"//��� ���������� ��������� ������ ���� ������
#include <locale.h>
#include <ctype.h>

void put_ships(void);//�������, ������� ���������� ����������� �������� ����������
void player_move(void);//������� ���� ������
void comp_move(void);//������� ���� ����������
bool alive_comp(int i, int j, int prev_i, int prev_j);
void ship_killed(int i, int j, int i_prev, int j_prev);
bool alive_player(int i, int j, int prev_i, int prev_j);
void player_ship_killed(int i, int j, int i_prev, int j_prev);//��� ��� �-��� ������ ����� �� ����������

char player_field[12][12], comp_field[12][12];//� ���� �������� �������� ���� � ������������ ��������
char player_battlefield[12][12], comp_battlefield[12][12];//�� ��� ������������ �� ������
bool hit = 0;//���� ��� - ���� ����� �����, ��� ������ ���������, � �������� �������
bool who_moves=1;//���� ��� ����� �����
int hit_i[4], hit_j[4];//���� ���� ��������� ���� ��������� (����� �������� �������)
int player_hit=0, comp_hit=0;//������� ���������� ��������� ����� � ������. ����� � ����-�� 20 ��������� - �� �������

/* ��� ��������� � ���������:
------------------>j
|
|
|
|
|
\/
 i
*/

int main()
{
    setlocale(0, "");
    rlutil::setColor(15);
    //������� 12*12, ����� �� ���� ���� �������� � ���, ��� ���������� �� �����
    //����� � ���� ���������: '-' - ���� ��� �� ��������, '+' - ���� ��� ��������, �� ���������, '�' - ������ �������
    srand(time(NULL));
    //��������� ��������� ���� ���� �������� ������
    for (int i=0; i<12; i++) {
        player_field[i][0] = '+';
        comp_field[i][0] = '+';
        player_field[i][11] = '+';
        comp_field[i][11] = '+';
        player_battlefield[i][0] = '+';
        comp_battlefield[i][0] = '+';
        player_battlefield[i][11] = '+';
        comp_battlefield[i][11] = '+';
    }
    for (int j=0; j<12; j++) {
        player_field[0][j] = '+';
        comp_field[0][j] = '+';
        player_field[11][j] = '+';
        comp_field[11][j] = '+';
        player_battlefield[0][j] = '+';
        comp_battlefield[0][j] = '+';
        player_battlefield[11][j] = '+';
        comp_battlefield[11][j] = '+';
    }
    for (int i=0; i<4; i++) {
        hit_i[i] = 0;
        hit_j[i] = 0;
    }

    printf ("����� ���������� � ������� ���! ������� ���� �����������. ��� ������ ���� ��� � ����� �������:\n");
    printf("- - - X X X X - - -\n");
    printf("X X - - - - - - X -\n");
    printf("- - - - - - X - X -\n");
    printf("- - - X - - - - X -\n");
    printf("- - - - - - - - - -\n");
    printf("X X X - - - X - - -\n");
    printf("- - - - - - - - - -\n");
    printf("- X - - X - - - - -\n");
    printf("- X - - X - - X - -\n");
    printf("- - - - - - - - - -\n\n");

    //����� ������ ���� �����������
    for (int i=1; i<=10; i++) {
        for (int j=1; j<=10; j++) {
            scanf(" %c", &player_field[i][j]);
        }
    }

    put_ships();

    while(1) {
        //����� ����� �����
        printf("\n");
        printf("   A B C D E F G H I J \t\t    A B C D E F G H I J\n");
        for (int i=1; i<=10; i++) {
                printf("%2d ", i);
            for (int j=1; j<=10; j++) {
                if (player_battlefield[i][j]=='+')
                    rlutil::setColor(11);
                if (player_battlefield[i][j]=='X')
                    rlutil::setColor(12);

                printf("%c ", player_battlefield[i][j]);
                rlutil::setColor(15);
            }

            printf("\t\t %2d ", i);

            for (int j=1; j<=10; j++) {
                if (comp_battlefield[i][j]=='+')
                    rlutil::setColor(11);
                if (comp_battlefield[i][j]=='X')
                    rlutil::setColor(12);

                printf("%c ", comp_battlefield[i][j]);
                rlutil::setColor(15);
            }
            printf("\n");
        }

        printf("-------------------------------------------------------");

    if (player_hit == 20) {
        printf("\n�����������, �� ��������!\n");
        return 0;
    }

    if (comp_hit == 20) {
        printf("\n�� ���������. � ��������� ��� ��� ����� �������!\n");
        return 0;
    }

    if (who_moves)
        player_move();
    else
        comp_move();
    }
}


void put_ships(void)//�������, ������� ���������� ����������� �������� ����������
{
    bool horizontal, ok;

    //������� �������� ��� ��������. ������ ������� �� �� ��� ����� battlefield
    for (int i=1; i<=10; i++) {
        for (int j=1; j<=10; j++) {
            comp_field[i][j] = '-';
            comp_battlefield[i][j] = '-';
            player_battlefield[i][j] = '-';
        }
    }

    int i, j;

    //�������� 4-��������� �������. �.�. �� ������, ���� ��������� �� ����
    horizontal = rand()%2;
    if (horizontal) {
        i = rand()%10+1;//�������� ���������� ������� ����� ������
        j = rand()%7+1;
        for (int x = j-1; x<=j+4; x++) {
            comp_field[i-1][x] = '+'; //��������� ����������� ���������, ����� ���� �� ����������� ��������� �������
            comp_field[i][x] = '+';
            comp_field[i+1][x] = '+';
        }
        for (int x = j; x<=j+3; x++) {
            comp_field[i][x] = 'X'; //�������� ��� �������
        }
    }
    else {
        i = rand()%7+1;//�������� ���������� ����� ������� ������
        j = rand()%10+1;
        for (int y = i-1; y<=i+4; y++) {
            comp_field[y][j-1] = '+';//��������� ����������� ���������, ����� ���� �� ����������� ��������� �������
            comp_field[y][j] = '+';
            comp_field[y][j+1] = '+';
        }
        for (int y = i; y<=i+3; y++) {
            comp_field[y][j] = 'X'; //�������� ��� �������
        }
    }

    //�������� 3-��������� �������
    for (int c = 1; c<=2; c++) {
        horizontal = rand()%2;
        if (horizontal) {
            do {
            i = rand()%10+1;//�������� ���������� ������� ����� ������
            j = rand()%8+1;
            ok = comp_field[i][j] == '-' && comp_field[i][j+1] == '-' && comp_field[i][j+2] == '-';
            } while (!ok); //�� - ������� ������ �� �����. ���, ���� �� ���� ������� �������� ������� ����� ������

            for (int x = j-1; x<=j+3; x++) {
            comp_field[i-1][x] = '+';
            comp_field[i][x] = '+';
            comp_field[i+1][x] = '+';//��������� ����������� ���������, ����� ���� �� ����������� ��������� �������
            }
            for (int x = j; x<=j+2; x++) {
                comp_field[i][x] = 'X'; //�������� ��� �������
            }
        }
        else {
            do {
            i = rand()%8+1;//�������� ���������� ������� ����� ������
            j = rand()%10+1;
            ok = comp_field[i][j] == '-' && comp_field[i+1][j] == '-' && comp_field[i+2][j] == '-';
            } while (!ok);//�� - ������� ������ �� �����. ���, ���� �� ���� ������� �������� ������� ����� ������

            for (int y = i-1; y<=i+3; y++) {
            comp_field[y][j-1] = '+';
            comp_field[y][j] = '+';
            comp_field[y][j+1] = '+';//��������� ����������� ���������, ����� ���� �� ����������� ��������� �������
            }
            for (int y = i; y<=i+2; y++) {
                comp_field[y][j] = 'X'; //�������� ��� �������
            }
        }
    }

    //�������� 2-��������� �������
    for (int c = 1; c<=3; c++) { //� �� ���� ����� ��� �����������, ���� ��� ��������� - ��. �������������
        horizontal = rand()%2;
        if (horizontal) {
            do {
            i = rand()%10+1;
            j = rand()%9+1;
            ok = comp_field[i][j] == '-' && comp_field[i][j+1] == '-' && comp_field[i][j+2] == '-';
            } while (!ok);

            for (int x = j-1; x<=j+2; x++) {
            comp_field[i-1][x] = '+';
            comp_field[i][x] = '+';
            comp_field[i+1][x] = '+';
            }
            for (int x = j; x<=j+1; x++) {
                comp_field[i][x] = 'X';
            }
        }
        else {
            do {
            i = rand()%9+1;
            j = rand()%10+1;
            ok = comp_field[i][j] == '-' && comp_field[i+1][j] == '-' && comp_field[i+2][j] == '-';
            } while (!ok);

            for (int y = i-1; y<=i+2; y++) {
            comp_field[y][j-1] = '+';
            comp_field[y][j] = '+';
            comp_field[y][j+1] = '+';
            }
            for (int y = i; y<=i+1; y++) {
                comp_field[y][j] = 'X';
            }
        }
    }

    //�������� �������������
    for(int c=1; c<=4; c++) { //��� ���� ��� �������, ������ ��������� �������� �� ������
        do {
        i = rand()%10+1;
        j = rand()%10+1;
        } while (comp_field[i][j] != '-');
        comp_field[i][j] = 'X';
    }

    //������ ��������� (��� ������ �� �����)
    //�� ���� ����� �� �������, �� � �� ������
    for (int i=1; i<=10; i++) {
        for (int j=1; j<=10; j++) {
            if (comp_field[i][j] == '+')
                comp_field[i][j] = '-';
        }
    }
}

//���������, ��� ������� ��� ���. ������ ��� ����������:
//������� ��������� ������� ����� �������, ���� ������ ���� ������ - ������ �� �� ��� ���
//prev_i � prev_j �����, ����� ������� �� �������������
bool alive_comp(int i, int j, int prev_i, int prev_j)
{
    if (comp_battlefield[i-1][j] != 'X' && comp_field[i-1][j] == 'X')
        return 1;
    if (comp_battlefield[i+1][j] != 'X' && comp_field[i+1][j] == 'X')
        return 1;
    if (comp_battlefield[i][j+1] != 'X' && comp_field[i][j+1] == 'X')
        return 1;
    if (comp_battlefield[i][j-1] != 'X' && comp_field[i][j-1] == 'X')
        return 1;

    if (comp_field[i-1][j] == 'X' && !(i-1==prev_i && j==prev_j))
        if (alive_comp(i-1,j, i, j))
            return 1;
    if (comp_field[i+1][j] == 'X' && !(i+1==prev_i && j==prev_j))
        if (alive_comp(i+1,j, i, j))
            return 1;
    if (comp_field[i][j-1] == 'X' && !(j-1==prev_j && i==prev_i))
        if (alive_comp(i,j-1, i, j))
            return 1;
    if (comp_field[i][j+1] == 'X' && !(j+1==prev_j && i==prev_i))
        if (alive_comp(i,j+1, i, j))
            return 1;

    return 0;
}

//������������ ���� ������ ������� ������� ����������. ������� ������ ���������� alive_comp
//��, �������� ������� ���������� ���������� �-��� ��� ����� � ������. ��� ������� ������ ������� �������
void comp_ship_killed(int i, int j, int i_prev, int j_prev)
{
    if (comp_battlefield[i-1][j-1] != 'X')
        comp_battlefield[i-1][j-1] = '+';
    if (comp_battlefield[i-1][j] != 'X')
        comp_battlefield[i-1][j] = '+';
    if (comp_battlefield[i-1][j+1] != 'X')
        comp_battlefield[i-1][j+1] = '+';
    if (comp_battlefield[i][j-1] != 'X')
        comp_battlefield[i][j-1] = '+';
    if (comp_battlefield[i][j+1] != 'X')
        comp_battlefield[i][j+1] = '+';
    if (comp_battlefield[i+1][j-1] != 'X')
        comp_battlefield[i+1][j-1] = '+';
    if (comp_battlefield[i+1][j] != 'X')
        comp_battlefield[i+1][j] = '+';
    if (comp_battlefield[i+1][j+1] != 'X')
        comp_battlefield[i+1][j+1] = '+';

     if (comp_battlefield[i-1][j] == 'X' && !(i-1==i_prev && j==j_prev))
        comp_ship_killed(i-1, j, i, j);
     if (comp_battlefield[i+1][j] == 'X' && !(i+1==i_prev && j==j_prev))
        comp_ship_killed(i+1, j, i, j);
     if (comp_battlefield[i][j-1] == 'X' && !(i==i_prev && j-1==j_prev))
        comp_ship_killed(i, j-1, i, j);
     if (comp_battlefield[i][j+1] == 'X' && !(i==i_prev && j+1==j_prev))
        comp_ship_killed(i, j+1, i, j);

}

void player_move(void)//�����������, ����� ����� �����
{
    int i, j;
    char letter;
    char coord[4];

    printf("\n��� ���! ������� ����� � �����\n");
   // scanf(" %c %d", &letter, &i);

    scanf("%s", coord);
    if(isalpha(coord[0])) {
        letter = coord[0];
        i = (coord[2] == '0') ? 10 : coord[1] - 48;
    } else {
        letter = (coord[1] == '0') ? coord[2] : coord[1];
        i = (coord[1] == '0') ? 10 : coord[0] - 48;
    }

    letter = toupper(letter);
    j = letter - 64;//����� ASCII ������������ ����� � �����
    //printf("%d %d", i, j);

    if (comp_field[i][j] == 'X') {
        who_moves = 1;
        player_hit++;
        comp_battlefield[i][j] = 'X';
        if (alive_comp(i, j, 11, 11))
            printf("�����!\n");
        else {
            comp_ship_killed(i, j, 11, 11);
            printf("����!\n");
        }
    } else {
        who_moves = 0;
        comp_battlefield[i][j] = '+';
        printf("����!\n");
    }
    Sleep(1000);
}

//�� �� ��� alive_comp, �� ��� ������� ������
bool alive_player(int i, int j, int prev_i, int prev_j)
{
    if (player_battlefield[i-1][j] != 'X' && player_field[i-1][j] == 'X')
        return 1;
    if (player_battlefield[i+1][j] != 'X' && player_field[i+1][j] == 'X')
        return 1;
    if (player_battlefield[i][j+1] != 'X' && player_field[i][j+1] == 'X')
        return 1;
    if (player_battlefield[i][j-1] != 'X' && player_field[i][j-1] == 'X')
        return 1;

    if (player_field[i-1][j] == 'X' && !(i-1==prev_i && j==prev_j))
        if (alive_player(i-1,j, i, j))
            return 1;
    if (player_field[i+1][j] == 'X' && !(i+1==prev_i && j==prev_j))
        if (alive_player(i+1,j, i, j))
            return 1;
    if (player_field[i][j-1] == 'X' && !(j-1==prev_j && i==prev_i))
        if (alive_player(i,j-1, i, j))
            return 1;
    if (player_field[i][j+1] == 'X' && !(j+1==prev_j && i==prev_i))
        if (alive_player(i,j+1, i, j))
            return 1;

    return 0;
}

//�� ��, ��� comp_ship killed, �� ��� ������� ������
void player_ship_killed(int i, int j, int i_prev, int j_prev)
{
    if (player_battlefield[i-1][j-1] != 'X')
        player_battlefield[i-1][j-1] = '+';
    if (player_battlefield[i-1][j] != 'X')
        player_battlefield[i-1][j] = '+';
    if (player_battlefield[i-1][j+1] != 'X')
        player_battlefield[i-1][j+1] = '+';
    if (player_battlefield[i][j-1] != 'X')
        player_battlefield[i][j-1] = '+';
    if (player_battlefield[i][j+1] != 'X')
        player_battlefield[i][j+1] = '+';
    if (player_battlefield[i+1][j-1] != 'X')
        player_battlefield[i+1][j-1] = '+';
    if (player_battlefield[i+1][j] != 'X')
        player_battlefield[i+1][j] = '+';
    if (player_battlefield[i+1][j+1] != 'X')
        player_battlefield[i+1][j+1] = '+';

     if (player_battlefield[i-1][j] == 'X' && !(i-1==i_prev && j==j_prev))
        player_ship_killed(i-1, j, i, j);
     if (player_battlefield[i+1][j] == 'X' && !(i+1==i_prev && j==j_prev))
        player_ship_killed(i+1, j, i, j);
     if (player_battlefield[i][j-1] == 'X' && !(i==i_prev && j-1==j_prev))
        player_ship_killed(i, j-1, i, j);
     if (player_battlefield[i][j+1] == 'X' && !(i==i_prev && j+1==j_prev))
        player_ship_killed(i, j+1, i, j);

}

void comp_move(void)//�����������, ����� ����� ����.
{
    int i, j, k=0;

    printf("\n��� ����������\n");

    if (hit) {
    //hit==true, ���� � ���� �� ���������� ����� ���� ����� � �������, �� �� ���� ���.
    //���������� ���� ��������� �� ����� ������� ������������ � ������� hit_i � hit_j
    //�� ��������� ��� ��������� ������
        k=0;
        while (hit_i[k]!=0 && k<4)
            k++;
        //k - ���������� ������� �������� �������� �������
        if (k==1) {
            //k==1 ���� ����� ����� ���� ���. � ���� ������ ������ ����� ������������ �������� ������
            //�� ������ ��� �� ������, �� ���������, ����� �� ����� �� ���� ������. ���� ��� - �������� �����
            do {
                int cell = rand()%4; //�������� ����� � [0;3]. ������� ������������� ���� �������� ������
                switch (cell) {
                    case 0:
                        i = hit_i[0]-1;
                        j = hit_j[0];
                        break;
                    case 1:
                        i = hit_i[0]+1;
                        j = hit_j[0];
                        break;
                    case 2:
                        i = hit_i[0];
                        j = hit_j[0]-1;
                        break;
                    case 3:
                        i = hit_i[0];
                        j = hit_j[0]+1;
                        break;
                }
            } while (player_battlefield[i][j] != '-');
        } else {
        //���� ����� �������, ���� ���� ����� ��� 1 ���������. � ���� ������, ����� ������ ����������,
        //��� ������������ �������: ������������� ��� �����������, � ����� �������� ������ ��� � ���� �� ���� ������.
            if (hit_i[0] == hit_i[1]) { //�������� ����������������
                //������� ������������. � ���� ������ ����� ����� ����������� � ������������ j � ��������
                //���� � ����� (i, j_max+1) ���� � (i, j_min-1)
                int j_max=0, j_min=11;
                bool choise;//��� ���������� ����������, ����� �� ���� ����� ����� �������

                i = hit_i[0];
                for (int c=0; c<k; c++) {//������� j_min � j_max
                    if (hit_j[c]>j_max)
                        j_max = hit_j[c];
                    if (hit_j[c]<j_min)
                        j_min = hit_j[c];
                }

                choise = rand()%2;//�� ��������� choise ��� ������ ������
                if (player_battlefield[i][j_max+1] == '+')
                    choise = 0;//�� ���� ������ �� ������ ������� ����� +, �� ����� ������� ������ �����.
                               //��� �� ��������, ���� ������� �� ������ ���� ��������� (����� ������� 12*12)
                if (player_battlefield[i][j_min-1] == '+')
                    choise = 1;//����������, �� ��� ������ ������
                if (choise)
                    j=j_max+1;
                 else
                    j=j_min-1;
                //�������� ������, ��� ������, ����� � ������ � ����� �� ������� ����� ������� ����������,
                //�.�. ��� ������� ��, ��� ������� ����, � ���� �� ��� ����, �� hit = false
            } else { //� �������� �� �� �����, �� ������� ������������ � �������� i-�����
                int i_max=0, i_min=11;
                bool choise;

                choise = rand()%2;
                j = hit_j[0];
                for (int c=0; c<k; c++) {
                    if (hit_i[c]>i_max)
                        i_max = hit_i[c];
                    if (hit_i[c]<i_min)
                        i_min = hit_i[c];
                }
                if (player_battlefield[i_max+1][j] == '+')
                    choise = 0;
                if (player_battlefield[i_min-1][j] == '+')
                    choise = 1;
                if (choise)
                    i=i_max+1;
                 else
                    i=i_min-1;
            }
        }
    } else {//���� ������� ���� ����, ���� ���, ���� ��� ���� � ������ ������ ������
        do {
            i = rand()%10+1;
            j = rand()%10+1;
        } while (player_battlefield[i][j] != '-');
    }
    //����� �������� ������� ��������� ���������� ����
    if (player_field[i][j] == 'X') {
        who_moves = 0;
        comp_hit++;
        player_battlefield[i][j] = 'X';
        if (alive_player(i, j, 11, 11)) {
            k=0;
            while (hit_i[k]!=0)
                k++;
            hit = 1;
            hit_i[k] = i;
            hit_j[k] = j;
            printf("�����!\n");
        } else {
        hit=0;
        for (int c=0; c<4; c++) {
            hit_i[c] = 0;
            hit_j[c] = 0;
        }
        player_ship_killed(i, j, 11, 11);
        printf("����!\n");
        }
    } else {
        who_moves = 1;
        player_battlefield[i][j] = '+';
        printf("����!\n");
    }
    Sleep(1000);
}
