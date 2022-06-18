#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <queue>
#include <array>
using namespace std;

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
const int maxmove = 1;
const int dir[4][2] = {-1, 1, 0, 1, 1, 1, 1, 0};
const int dic[typenum] = {94879487, 9487878, 948787, 9487, 8700, 2000, 487, 187};
const int Dir[8][2] = {-1, 1, 0, 1, 1, 1, 1, 0, 1, -1, 0, -1, -1, -1, -1, 0};

int judge(string &ref)
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
    return -1;
}

class state
{
public:
    state(int a)
    {
        for (int i = 0; i < (typenum << 1); i++)
            type[i] = 0;
        val = 0;
        if ((player == 1 && a == 0) || (player == 2 && a == 1))
            cur = 1;
        else
            cur = 2;
        evaluate();
        caculate();
    };

    int type[2 * typenum], cur, val;

private:
    void evaluate()
    {
        int x, y, t;
        for (int d = 0; d < 4; d++)
        {
            for (int i = 0; i < SIZE; i++)
            {
                for (int j = 0; j < SIZE; j++)
                {
                    string tmp;
                    x = i, y = j;
                    while (x >= 0 && y >= 0 && x < SIZE && y < SIZE && tmp.size() < 7)
                    {
                        switch (board[x][y])
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
                        {
                            t = judge(tmp);
                            if (t > 0)
                                type[t]++;
                        }
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
            if (i < typenum)
            {
                if (cur == 1)
                    val += type[i] * dic[i];
                else
                    val -= type[i] * dic[i];
            }
            else
            {
                if (cur == 1)
                    val -= type[i] * dic[i - typenum];
                else
                    val += type[i] * dic[i - typenum];
            }
        }
    }
};

int dfs(int step, ofstream &fout)
{
    // initiallize
    state cur(step & 1);

    // base case
    if (step == maxmove)
        return cur.val;

    // find candicate
    int dist[15][15];
    vector<pair<int, int>> candicate;
    queue<pair<int, int>> q;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] != 0)
            {
                q.emplace(i, j);
                dist[i][j] = 0;
            }
            else
                dist[i][j] = 87;
        }
    while (!q.empty())
    {
        int x, y;
        auto tmp = q.front();
        if (dist[tmp.first][tmp.second] == 1)
            break;
        q.pop();
        for (int i = 0; i < 8; i++)
        {
            x = tmp.first + Dir[i][0];
            y = tmp.second + Dir[i][1];
            if (x >= 0 && y >= 0 && x < SIZE && y < SIZE)
            {
                if (dist[x][y] > dist[tmp.first][tmp.second] + 1)
                {
                    dist[x][y] = dist[tmp.first][tmp.second] + 1;
                    candicate.emplace_back(x, y);
                    q.emplace(x, y);
                }
            }
        }
    }
    if (candicate.empty())
        candicate.emplace_back(8, 7);

    // recursion
    int min = 1e9 + 87;
    for (auto tmp : candicate)
    {
        board[tmp.first][tmp.second] = player + (step & 1);
        int v = dfs(step + 1, fout);
        board[tmp.first][tmp.second] = 0;
        if (v < min)
        {
            min = v;
            if (step == 0)
            {
                fout << tmp.first << " " << tmp.second << endl;
                fout.flush();
            }
        }
    }
    return min;
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
