#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>

enum SPOT_STATE
{
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};

int player;
const int SIZE = 15;
std::array<std::array<int, SIZE>, SIZE> board;

void read_board(std::ifstream &fin)
{
    fin >> player;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            fin >> board[i][j];
        }
    }
}

/***********************************************************************************************************************************************/
enum TYPE
{
    FIVE,
    FOUR,
    DEAD_FOUR,
    JUMP_THREE,
    THREE,
    SLEEP_THREE,
    TWO,
    SLEEP_TWO,
};

const int typenum = 8;
const int maxmove = 3;
const int dir[4][2] = {-1, 1, 0, 1, 1, 1, 1, 0};
const int dic[typenum] = {94879487, 9487878, 948787, 9487, 8700, 2000, 487, 187};

int judge(std::string &ref)
{
    if (ref.size() == 4)
    {
        // JUMP_THREE OO.O || O.OO
        if (ref == "OO.O" || ref == "O.OO")
            return JUMP_THREE;
        if (ref == "XX.X" || ref == "X.XX")
            return typenum + JUMP_THREE;
        // TWO O..O
        if (ref == "O..O")
            return TWO;
        if (ref == "X..X")
            return typenum + TWO;
    }
    if (ref.size() == 5)
    {
        // FIVE OOOOO
        if (ref == "OOOOO")
            return FIVE;
        if (ref == "XXXXX")
            return typenum + FIVE;
        // DEAD_FOUR OO.OO || O.OOO || OOO.O
        if (ref == "OO.OO" || ref == "O.OOO" || ref == "OOO.O")
            return DEAD_FOUR;
        if (ref == "XX.XX" || ref == "X.XXX" || ref == "XXX.X")
            return typenum + DEAD_FOUR;
        // THREE .OOO.
        if (ref == ".OOO.")
            return THREE;
        if (ref == ".XXX.")
            return typenum + THREE;
        // SLEEP_THREE O..OO || OO..O || O.O.O
        if (ref == "O..OO" || ref == "OO..O" || ref == "O.O.O")
            return SLEEP_THREE;
        if (ref == "X..XX" || ref == "XX..X" || ref == "X.X.X")
            return typenum + SLEEP_THREE;
        // TWO .O.O.
        if (ref == ".O.O.")
            return TWO;
        if (ref == ".X.X.")
            return typenum + TWO;
        // SLEEP_TWO O...O
        if (ref == "O...O")
            return SLEEP_TWO;
        if (ref == "X...X")
            return typenum + SLEEP_TWO;
    }
    if (ref.size() == 6)
    {
        // FOUR .OOOO.
        if (ref == ".OOOO.")
            return FOUR;
        if (ref == ".XXXX.")
            return typenum + FOUR;
        // DEAD_FOUR XOOOO. || .OOOOX
        if (ref == "XOOO." || ref == ".OOOOX")
            return DEAD_FOUR;
        if (ref == "OXXX." || ref == ".XXXXO")
            return typenum + DEAD_FOUR;
        // SLEEP_THREE XOOO.. || ..OOOX || XOO.O. || .O.OOX || XO.OO. || .OO.OX
        if (ref == "XOOO.." || ref == "..OOOX" || ref == "XOO.O." || ref == ".O.OOX" || ref == "XO.OO." || ref == ".OO.OX")
            return SLEEP_THREE;
        if (ref == "OXXX.." || ref == "..XXXO" || ref == "OXX.X." || ref == ".X.XXO" || ref == "OX.XX." || ref == ".XX.XO")
            return typenum + SLEEP_THREE;
        // TWO ..OO..
        if (ref == "..OO..")
            return TWO;
        if (ref == "..XX..")
            return typenum + TWO;
        // SLEEP_TWO XOO... || ...OOX || XO.O.. || ..O.OX || XO..O. || .O..OX
        if (ref == "XOO..." || ref == "...OOX" || ref == "XO.O.." || ref == "..O.OX" || ref == "XO..O." || ref == ".O..OX")
            return SLEEP_TWO;
        if (ref == "OXX..." || ref == "...XXO" || ref == "OX.X.." || ref == "..X.XO" || ref == "OX..X." || ref == ".X..XO")
            return typenum + SLEEP_TWO;
    }
    if (ref.size() == 7)
    {
        // SLEEP_THREE X.OOO.X
        if (ref == "X.OOO.X")
            return SLEEP_THREE;
        if (ref == "O.XXX.O")
            return SLEEP_THREE;
    }
}

class state
{
public:
    state()
    {
        for (int i = 0; i < (typenum << 1); i++)
            type[i] = 0;
        val = 0;
        cur = player;
        evaluate();
        caculate();
    };

    int type[2 * typenum], cur, val;

private:
    void evaluate()
    {
        int val = 0, x, y;
        for (int d = 0; d < 4; d++)
        {
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    std::string tmp;
                    x = i, y = j;
                    while (x >= 0 && y >= 0 && x < SIZE && y < SIZE && tmp.size() < 7)
                    {
                        switch (board[i][j])
                        {
                        case EMPTY:
                            tmp += '.';
                            break;
                        case BLACK:
                            tmp += 'O';
                            break;
                        case WHITE:
                            tmp += 'X';
                            break;
                        }
                        if (tmp.size() > 3 && tmp.size() < 8)
                            type[judge(tmp)]++;
                        x += dir[d][0];
                        y += dir[d][1];
                    }
                }
            }
        }
    }
    void caculate()
    {
        for (int i = 0; i < (typenum << 1); i++)
        {
            if (i <= typenum)
                val += type[i] * dic[i];
            else
                val -= type[i] * dic[i - typenum];
        }
    }
};

void dfs(int step, std::ofstream &fout)
{
    state cur;
}

/***********************************************************************************************************************************************/

// void write_valid_spot(std::ofstream &fout)
// {
//     srand(time(NULL));
//     int x, y;
//     // Keep updating the output until getting killed.
//     while (true)
//     {
//         // Choose a random spot.
//         int x = (rand() % SIZE);
//         int y = (rand() % SIZE);
//         if (board[x][y] == EMPTY)
//         {
//             fout << x << " " << y << std::endl;
//             // Remember to flush the output to ensure the last action is written to file.
//             fout.flush();
//         }
//     }
// }

int main(int, char **argv)
{
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    dfs(0, fout);
    fin.close();
    fout.close();
    return 0;
}
