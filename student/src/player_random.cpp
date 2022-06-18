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

const int maxmove = 3;
const int dir[4][2] = {-1, 1, 0, 1, 1, 1, 1, 0};

class coord
{
public:
    coord()
    {
        for (int i = 0; i < 7; i++)
            atk[i] = def[i] = 0;
    };
    int atk[7], def[7];
};

/*
int judge(std::vector<int> &ref)
{
    if (ref.size() == 4)
    {
        //JUMP_THREE OO.O || O.OO
        if ((ref[0] == ref[1] && ref[1] == ref[3] && ref[2] == 0)
        ||  (ref[0] == ref[2] && ref[2] == ref[3] && ref[1] == 0))
            return JUMP_THREE;
        //TWO O..O
        if (ref[1] == 0 && ref[0] == ref[3] && ref[1] == ref[2])
            return TWO;
    }
    if (ref.size() == 5)
    {
        //FIVE OOOOO
        if (ref[0] == ref[1] && ref[1] == ref[2] && ref[2] == ref[3] && ref[3] == ref[4])
            return FIVE;
        //DEAD_FOUR OO.OO || O.OOO || OOO.O
        if ((ref[2] == 0 & ref[0] == ref[1] && ref[1] == ref[3] && ref[3] == ref[4])
        ||  (ref[1] == 0 && ref[0] == ref[2] && ref[2] == ref[3] && ref[3] == ref[4])
        ||  (ref[3] == 0 && ref[0] == ref[1] && ref[1] == ref[2] && ref[2] == ref[4]))
            return DEAD_FOUR;
        //THREE .OOO.
        if (ref[0] == 0 && ref[4] == 0 && ref[1] == ref[2] && ref[2] == ref[3])
            return THREE;
        //SLEEP_THREE O..OO || OO..O || O.O.O
        if ((ref[1] == 0 && ref[1] == ref[2] && ref[0] == ref[3] && ref[3] == ref[4])
        ||  (ref[2] == 0 && ref[2] == ref[3] && ref[0] == ref[1] && ref[1] == ref[4])
        ||  (ref[1] == 0 && ref[1] == ref[3] && ref[0] == ref[2] && ref[2] == ref[4]))
            return SLEEP_THREE;
        //TWO .O.O.
        if (ref[0] == 0 && ref[0] == ref[2] && ref[2] == ref[4] && ref[1] == ref[3])
            return TWO;
        //SLEEP_TWO O...O
        if (ref[2] == 0 && ref[0] == ref[4] && ref[1] == ref[2] && ref[2] == ref[3])
            return SLEEP_TWO;
    }
    if (ref.size() == 6)
    {
        //FOUR .OOOO.
        if (ref[0] == 0 && ref[0] == ref[5] && ref[1] == ref[2] && ref[2] == ref[3] && ref[3] == ref[4])
            return FOUR;
        //DEAD_FOUR XOOOO. || .OOOOX
        if ((ref[5] == 0 && ref[0] != ref[1] && ref[0] != ref[2] && ref[1] == ref[2] && ref[2] == ref[3] && ref[3] == ref[4])
        ||  (ref[0] == 0 && ref[1] == ref[2] && ref[2] == ref[3] && ref[3] == ref[4] && ref[4] != ref[5] && ref[5] != ref[3]))
            return DEAD_FOUR;
        //SLEEP_THREE XOOO.. || ..OOOX || XOO.O. || .O.OOX || XO.OO. || .OO.OX
    }
}
*/

int judge(std::string &ref)
{
    if (ref.size() == 4)
    {
        //JUMP_THREE OO.O || O.OO
        if (ref == "OO.O" || ref == "O.OO")
            return JUMP_THREE;
        //TWO O..O
        if (ref == "O..O")
            return TWO;
    }
    if (ref.size() == 5)
    {
        //FIVE OOOOO
        if (ref == "OOOOO")
            return FIVE;
        //DEAD_FOUR OO.OO || O.OOO || OOO.O
        if (ref == "OO.OO" || ref == "O.OOO" || ref == "OOO.O")
            return DEAD_FOUR;
        //THREE .OOO.
        if (ref == ".OOO.")
            return THREE;
        //SLEEP_THREE O..OO || OO..O || O.O.O
        if (ref == "O..OO" || ref == "OO..O" || ref == "O.O.O")
            return SLEEP_THREE;
        //TWO .O.O.
        if (ref == ".O.O.")
            return TWO;
        //SLEEP_TWO O...O
        if (ref == "O...O")
            return SLEEP_TWO;
    }
    if (ref.size() == 6)
    {
        //FOUR .OOOO.
        if (ref == ".OOOO.")
            return FOUR;
        //DEAD_FOUR XOOOO. || .OOOOX
        if (ref == "XOOO." || ref == ".OOOOX")
            return DEAD_FOUR;
        //SLEEP_THREE XOOO.. || ..OOOX || XOO.O. || .O.OOX || XO.OO. || .OO.OX
        if (ref == "XOOO.." || ref == "..OOOX" || ref == "XOO.O." || ref == ".O.OOX" || ref == "XO.OO." || ref == ".OO.OX")
            return SLEEP_THREE;
        //TWO ..OO..
        if (ref == "..OO..")
            return TWO;
        //SLEEP_TWO XOO... || ...OOX || XO.O.. || ..O.OX || XO..O. || .O..OX
        if (ref == "XOO..." || ref == "...OOX" || ref == "XO.O.." || ref == "..O.OX" ||ref == "XO..O." || ref == ".O..OX")
            return SLEEP_TWO;
    }
    if (ref.size() == 7)
    {
        //SLEEP_THREE X.OOO.X
        if (ref == "X.OOO.X")
            return SLEEP_THREE;
    }
}

int evaluate(int cur)
{
    int val = 0, x, y;
    std::array<std::array<coord, SIZE>, SIZE> p;
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] != 0)
                for (int d = 0; d < 4; d++)
                {
                    x = i, y = j;
                    while (x >= 0 && y >= 0 && x < SIZE && y < SIZE)
                    {
                    }
                }
        }
    }
}

void dfs(int step, std::ofstream &fout)
{
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
    dfs(3, fout);
    fin.close();
    fout.close();
    return 0;
}
