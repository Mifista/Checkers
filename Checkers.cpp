#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include "windows.h"
using namespace std;

//На моём ПК Русский язык работать не захотел, поэтому UI на английском

void translate(string, int[4]);
string translate(int[4]);

class checker {
public:

    int turnage = 0; //first_turn
    int side = 0;      //Сторона, от числа зависит сторона и вид фигуры: дамка или пешка.

    bool life = 0;                                                              //"населённость" клетки
    int coords[2];

    void check(checker desk[8][8], vector<int>* can, vector<int>* cant) {              //can и cant относится к возможности съесть фигуру на поле

        int stepx;
        int stepy;

        for (int dir = 0; dir <= 4; dir++) {
            int coor0 = coords[0];
            int coor1 = coords[1];
            int counter = 0;

            if (dir == 0) {                                 //типы передвижения по полю: вперёд влево, вперёд вправо, назад влево и назад вправо НЕ в указанном порядке. вроде
                stepx = 1;
                stepy = 1;
            }

            else if (dir == 1) {
                stepx = 1;
                stepy = -1;
            }

            else if (dir == 2) {
                stepx = -1;
                stepy = 1;
            }

            else if (dir == 3) {
                stepx = -1;
                stepy = -1;
            }

            int c = 0;

            while (coor0 + stepx >= 0 && coor0 + stepx < 8 && coor1 + stepy >= 0 && coor1 + stepy < 8 && c <2) {
                c += 1;
                coor0 += stepx;
                coor1 += stepy;

                //далее расписано что делать, если клетка населена: можно ли есть, нет и что делать, если что-то не так.

                if (desk[coor0][coor1].life == 0 && counter == 0 && ((turnage == 1 && (dir == 0 or dir == 1)) or (turnage == 0 && (dir == 2 or dir == 3)))) {
                    (*cant).push_back((coords[0] + 1) * 1000 + (coords[1] + 1) * 100 + (coor0 + 1) * 10 + coor1);
                    break;
                }

                else if (desk[coor0][coor1].life == 1 && (desk[coor0][coor1].side + 1) / 2 == (side + 1) / 2) {
                    break;
                }

                else if (desk[coor0][coor1].life == 1 && (desk[coor0][coor1].side + 1) / 2 != (side + 1) / 2) {
                    counter += 1;
                    if (counter == 2) {
                        break;
                    }
                }

                else if (desk[coor0][coor1].life == 0 && counter == 1) {
                    (*can).push_back((coords[0] + 1) * 1000 + (coords[1] + 1) * 100 + (coor0 + 1) * 10 + coor1);
                    break;
                }
            }
        }
    }


    void turn(int turn[2], checker desk[8][8]) {
        checker ch;
        desk[turn[1]][turn[0]] = desk[coords[0]][coords[1]];

        for (int i = abs(turn[1] - coords[0]) - 1; i >= 0; i -= 1) {
            desk[coords[0] + i * (turn[1] - coords[0]) / abs(turn[1] - coords[0])][coords[1] + i * (turn[0] - coords[1]) / abs(turn[0] - coords[1])] = ch;
        }

        desk[turn[1]][turn[0]].coords[0] = turn[1];
        desk[turn[1]][turn[0]].coords[1] = turn[0];
        if (turn[1] == desk[turn[1]][turn[0]].turnage * 7 && desk[turn[1]][turn[0]].side % 2 != 0) {
            desk[turn[1]][turn[0]].side += 1;
        }
    }


};

class king :public checker {
public:
    void check(checker desk[8][8], vector<int>* can, vector<int>* cant) {
        int stepx;
        int stepy;
        for (int dir = 0; dir <= 4; dir++) {
            int coor0 = coords[0];
            int coor1 = coords[1];
            int counter = 0;

            //за исключением того, что дамка может ходить куда хочет, код почти не отличается.

            if (dir == 0) {
                stepx = 1;
                stepy = 1;
            }
            else if (dir == 1) {
                stepx = 1;
                stepy = -1;
            }
            else if (dir == 2) {
                stepx = -1;
                stepy = 1;
            }
            else if (dir == 3) {
                stepx = -1;
                stepy = -1;
            }


            while (coor0 + stepx >= 0 && coor0 + stepx < 8 && coor1 + stepy >= 0 && coor1 + stepy < 8) {
                coor0 += stepx;
                coor1 += stepy;
                if (desk[coor0][coor1].life == 0 && counter == 0) {
                    (*cant).push_back((coords[0] + 1) * 1000 + (coords[1] + 1) * 100 + (coor0 + 1) * 10 + coor1);
                }
                else if (desk[coor0][coor1].life == 1 && (desk[coor0][coor1].side + 1) / 2 == (side + 1) / 2) {
                    break;
                }
                else if (desk[coor0][coor1].life == 1 && (desk[coor0][coor1].side + 1) / 2 != (side + 1) / 2) {
                    counter += 1;
                    if (counter == 2) {
                        break;
                    }
                }
                else if (desk[coor0][coor1].life == 0 && counter == 1) {
                    (*can).push_back((coords[0] + 1) * 1000 + (coords[1] + 1) * 100 + (coor0 + 1) * 10 + coor1);
                }
            }
        }
    }
};

class desk {


    void check(int side) {                                                                                              //проверка поля на фишку чтобы уточнить съедобность.
        for (int i = 0; i < 8; i++) {
            for (int u = 0; u < 8; u++) {
                if (field[i][u].side == side) {
                    field[i][u].check(field, &can_eat, &cant_eat);
                }
                else if (field[i][u].side == side + 1) {
                    static_cast<king*>(&field[i][u])->check(field, &can_eat, &cant_eat);
                }
            }
        }
    }


public:
    int first_turn = rand() % 2;//1 - игрок первый, 0 - ПК первый.
    int flag_turn = first_turn;
    string history = "";

    checker field[8][8];
    vector<int> can_eat;
    vector<int> cant_eat;

    desk() {
        int o = 0;
        if (first_turn) {                                               //создание поля и расположение на нём шашек
            cout << "First turn for Player" << endl;
            Sleep(1500);
            system("cls");
        }
        else {
            cout << "First turn for AI" << endl;
            Sleep(1500);
            system("cls");
        }

        for (int i = 0; i < 3; i++) {                                                                                   //два цикла по заполнению поля
            for (int u = 0; u < 8; u++) {
                if (o == 0) {
                    field[i][u].side = first_turn * 2 + 1;
                    field[i][u].life = 1;
                    field[i][u].turnage = 1;
                    field[i][u].coords[0] = i;
                    field[i][u].coords[1] = u;
                }
                o = (o + 1) % 2;
            }
            o = (o + 1) % 2;
        }


        for (int i = 7; i > 4; i--) {
            o = (o + 1) % 2;
            for (int u = 0; u < 8; u++) {
                o = (o + 1) % 2;
                if (o == 0) {
                    field[i][u].side = -2 * (first_turn - 1) + 1;
                    field[i][u].life = 1;
                    field[i][u].turnage = 0;
                    field[i][u].coords[0] = i;
                    field[i][u].coords[1] = u;
                }
            }
        }
    }

    void print_pole() {//вывод игрового поля  1 - шашка "ии", 2 - дамка "ии", 3 - шашка игрока, 4 - дамка игрока, # - пусто
        cout << "  abcdefgh" << endl;
        for (int i = 0; i < 8; i++) {
            cout << i + 1 << " ";
            for (int u = 0; u < 8; u++) {
                if (field[i][u].life == 0) {
                    cout << "#";
                }
                else {
                    cout << field[i][u].side;
                }
            }
            cout << endl;
        }
        cout << endl;
    }

    void start() {
        cout << "markings: \n 1 - 1'st player peasant, 2 - 1'st player king, 3 - 2'nd player's peasant, 4 - 2'nd player's king\n Input something to start:" << endl;
        cin >> history;
        system("cls");
        history = "";
    }



    void bot_turn(int side) {//1 - bot, 3 - player.
        can_eat.clear();
        cant_eat.clear();                           //Зачистка съедобных-несъедобных списков


        check(side);


        if (can_eat.size() == 0 && cant_eat.size() == 0) {

            if (side == 1) {
                cout << "you win" << endl;                                          //если больше фигур "противника" нет - победа (в программе фигуры 1 и 2 всегда считаются игроком, а 3 и 4 - противником, даже для ботов
                system("pause");
            }

            if (side == 3) {
                cout << "you lose" << endl;
                system("pause");
            }
        }


        if (can_eat.empty() != true) {
            int val = int(rand() % can_eat.size());

            int t[4] = { ((can_eat[val] % 1000) / 100) - 1 , int(can_eat[val] / 1000) - 1 , can_eat[val] % 10,  ((can_eat[val] % 100) / 10) - 1 };
            history += translate(t) + " ";
            cout << translate(t) << endl;


            int end_pos[2] = { can_eat[val] % 10 , ((can_eat[val] % 100) / 10) - 1 };
            field[int(can_eat[val] / 1000) - 1][int((can_eat[val] % 1000) / 100) - 1].turn(end_pos, field);
            int time_copy = can_eat[val] % 100;
            can_eat.clear();

            if (field[(time_copy / 10) - 1][time_copy % 10].side == side) {
                field[(time_copy / 10) - 1][time_copy % 10].check(field, &can_eat, &cant_eat);
            }

            else if (field[(time_copy / 10) - 1][time_copy % 10].side == side + 1) {
                static_cast<king*>(&field[(time_copy / 10) - 1][time_copy % 10])->check(field, &can_eat, &cant_eat);
            }

            while (can_eat.empty() != true) {
                val = int(rand() % can_eat.size());

                int t[4] = { ((can_eat[val] % 1000) / 100) - 1 , int(can_eat[val] / 1000) - 1 , can_eat[val] % 10,  ((can_eat[val] % 100) / 10) - 1 };
                history += translate(t) + " ";
                cout << translate(t) << endl;

                int end_pos[2] = { can_eat[val] % 10 , ((can_eat[val] % 100) / 10) - 1 };
                field[int(can_eat[val] / 1000) - 1][int((can_eat[val] % 1000) / 100) - 1].turn(end_pos, field);
                time_copy = can_eat[val] % 100;
                can_eat.clear();
                if (field[(time_copy / 10) - 1][time_copy % 10].side == side) {
                    field[(time_copy / 10) - 1][time_copy % 10].check(field, &can_eat, &cant_eat);
                }
                else if (field[(time_copy / 10) - 1][time_copy % 10].side == side + 1) {
                    static_cast<king*>(&field[(time_copy / 10) - 1][time_copy % 10])->check(field, &can_eat, &cant_eat);
                }
            }

        }


        else {
            int val = int(rand() % cant_eat.size());

            int t[4] = { ((cant_eat[val] % 1000) / 100) - 1 , int(cant_eat[val] / 1000) - 1 , cant_eat[val] % 10,  ((cant_eat[val] % 100) / 10) - 1 };
            history += translate(t) + " ";
            cout << translate(t) << endl;

            int end_pos[2] = { cant_eat[val] % 10 , ((cant_eat[val] % 100) / 10) - 1 };
            field[int(cant_eat[val] / 1000) - 1][int((cant_eat[val] % 1000) / 100) - 1].turn(end_pos, field);
        }
    }

    void pl_turn(int side) {
        string sturn;
        int turn[4];
        can_eat.clear();
        cant_eat.clear();



        check(side);



        if (can_eat.size() == 0 && cant_eat.size() == 0) {
            cout << "you lose" << endl;
            system("pause");
        }

        cin >> sturn;
        translate(sturn, turn);
        if (can_eat.empty() != true) {
            history += sturn + " ";
            int end_pos[2] = { turn[2],turn[3] };
            field[turn[1]][turn[0]].turn(end_pos, field);

            can_eat.clear();
            if (field[turn[3]][turn[2]].side == 3) {
                field[turn[3]][turn[2]].check(field, &can_eat, &cant_eat);
            }
            else if (field[turn[3]][turn[2]].side == 4) {
                static_cast<king*>(&field[turn[3]][turn[2]])->check(field, &can_eat, &cant_eat);
            }

            while (can_eat.empty() != true) {
                system("cls");
                cout << history << endl << endl;
                print_pole();
                cin >> sturn;
                translate(sturn, turn);
                history += sturn + " ";
                int end_pos[2] = { turn[2],turn[3] };
                field[turn[1]][turn[0]].turn(end_pos, field);

                can_eat.clear();
                if (field[turn[3]][turn[2]].side == 3) {
                    field[turn[3]][turn[2]].check(field, &can_eat, &cant_eat);
                }
                else if (field[turn[3]][turn[2]].side == 4) {
                    static_cast<king*>(&field[turn[3]][turn[2]])->check(field, &can_eat, &cant_eat);
                }
            }
        }
        else {
            history += sturn + " ";
            int end_pos[2] = { turn[2],turn[3] };
            field[turn[1]][turn[0]].turn(end_pos, field);

        }
    }
};



int main()
{
    srand(time(0));
    int gmode;
    cout << "Please, select game mode:\n1-AI vs AI, 2-Player vs AI, 3-Player vs Player:";
    cin >> gmode;
    system("cls");
    desk table;
    table.start();

    if (gmode == 1) {
        while (1) {
            cout << table.history << endl << endl;
            table.print_pole();
            if (table.flag_turn == 1) {
                // table.pl_turn();
                table.bot_turn(3);
                Sleep(2000);
            }
            else {
                table.bot_turn(1);
                Sleep(2000);
            }
            table.flag_turn = (table.flag_turn + 1) % 2;
            system("cls");
        }
    }


    if (gmode == 2) {
        while (1) {
            cout << table.history << endl << endl;
            table.print_pole();
            if (table.flag_turn == 1) {
                // table.pl_turn();
                table.pl_turn(3);
            }
            else {
                table.bot_turn(1);
                Sleep(2000);
            }
            table.flag_turn = (table.flag_turn + 1) % 2;
            system("cls");
        }
    }


    if (gmode == 3) {
        while (1) {
            cout << table.history << endl << endl;
            table.print_pole();
            if (table.flag_turn == 1) {
                table.pl_turn(3);
            }
            else {
                table.pl_turn(1);
            }
            table.flag_turn = (table.flag_turn + 1) % 2;
            system("cls");
        }
    }


}




void translate(string sturn, int iturn[4]) {
    iturn[0] = (sturn[0] - '0') - 49;
    iturn[1] = (sturn[1] - '0') - 1;

    iturn[2] = (sturn[3] - '0') - 49;
    iturn[3] = (sturn[4] - '0') - 1;
}

string translate(int iturn[4]) {
    string sturn = "00-00";

    sturn[0] = (iturn[0] + 49) + '0';
    sturn[1] = (iturn[1] + 1) + '0';

    sturn[3] = (iturn[2] + 49) + '0';
    sturn[4] = (iturn[3] + 1) + '0';

    return sturn;
}
