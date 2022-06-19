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

const int INF = 1e9 + 87;
const int typenum = 8;
const int maxmove = 3;
const int dir[4][2] = {-1, 1, 0, 1, 1, 1, 1, 0};
const int dic[10] = {94879487, 9487487, 948787, 94878, 48700, 9487, 4870, 870, 487, 87};
const int Dir[8][2] = {-1, 1, 0, 1, 1, 1, 1, 0, 1, -1, 0, -1, -1, -1, -1, 0};

int judge(string &ref)
{
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
        if (ref == "XOOOO." || ref == ".OOOOX")
            return DEAD_FOUR;
        if (ref == "OXXXX." || ref == ".XXXXO")
            return typenum + DEAD_FOUR;
        // JUMP_THREE OO.O || O.OO
        if (ref == ".OO.O." || ref == ".O.OO.")
            return JUMP_THREE;
        if (ref == ".XX.X." || ref == ".X.XX.")
            return typenum + JUMP_THREE;
        // SLEEP_THREE XOOO.. || ..OOOX || XOO.O. || .O.OOX || XO.OO. || .OO.OX
        if (ref == "XOOO.." || ref == "..OOOX" || ref == "XOO.O." || ref == ".O.OOX" || ref == "XO.OO." || ref == ".OO.OX")
            return SLEEP_THREE;
        if (ref == "OXXX.." || ref == "..XXXO" || ref == "OXX.X." || ref == ".X.XXO" || ref == "OX.XX." || ref == ".XX.XO")
            return typenum + SLEEP_THREE;
        // TWO ..OO.. || .O..O.
        if (ref == "..OO.." || ref == ".O..O.")
            return TWO;
        if (ref == "..XX.." || ref == ".X..X.")
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
            cur = 2;
        else
            cur = 1;
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
                        if (tmp == "....")
                            break;
                        if (tmp.size() > 4 && tmp.size() < 8)
                        {
                            t = judge(tmp);
                            if (t >= 0)
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
        if (cur == 1)
        {
            // atk
            val += type[FIVE] * dic[0];
            val += type[FOUR] * dic[2];
            val += type[DEAD_FOUR] * dic[4];
            val += type[JUMP_THREE] * dic[5];
            val += type[THREE] * dic[5];
            val += type[SLEEP_THREE] * dic[7];
            val += type[TWO] * dic[8];
            val += type[SLEEP_TWO] * dic[9];
            // def
            val -= type[typenum + FIVE] * dic[0];
            val -= type[typenum + FOUR] * dic[1];
            val -= type[typenum + DEAD_FOUR] * dic[1];
            val -= type[typenum + JUMP_THREE] * dic[3];
            val -= type[typenum + THREE] * dic[3];
            val -= type[typenum + SLEEP_THREE] * dic[7];
            val -= type[typenum + TWO] * dic[8];
            val -= type[typenum + SLEEP_TWO] * dic[9];
        }
        else
        {
            // def
            val -= type[FIVE] * dic[0];
            val -= type[FOUR] * dic[1];
            val -= type[DEAD_FOUR] * dic[1];
            val -= type[JUMP_THREE] * dic[3];
            val -= type[THREE] * dic[3];
            val -= type[SLEEP_THREE] * dic[7];
            val -= type[TWO] * dic[8];
            val -= type[SLEEP_TWO] * dic[9];
            // atk
            val += type[typenum + FIVE] * dic[0];
            val += type[typenum + FOUR] * dic[2];
            val += type[typenum + DEAD_FOUR] * dic[4];
            val += type[typenum + JUMP_THREE] * dic[5];
            val += type[typenum + THREE] * dic[5];
            val += type[typenum + SLEEP_THREE] * dic[7];
            val += type[typenum + TWO] * dic[8];
            val += type[typenum + SLEEP_TWO] * dic[9];
        }
    }
};

int dfs(int step, int alpha, int beta, ofstream &fout)
{
    // base case
    if (step == maxmove)
    {
        state cur(step & 1);
        return cur.val;
    }

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
    int cur;
    if ((player == 1 && (step & 1) == 0) || (player == 2 && (step & 1) == 1))
        cur = 1;
    else
        cur = 2;
    if (step % 2 == 0)
    {
        int v = -INF;
        for (auto tmp : candicate)
        {
            board[tmp.first][tmp.second] = cur;
            v = max(v, dfs(step + 1, alpha, beta, fout));
            if (v > alpha)
            {
                alpha = v;
                if (step == 0)
                    fout << tmp.first << ' ' << tmp.second << '\n';
            }
            board[tmp.first][tmp.second] = 0;
            if (beta <= alpha)
                return alpha;
        }
        return v;
    }
    else
    {
        int v = INF;
        for (auto tmp : candicate)
        {
            board[tmp.first][tmp.second] = cur;
            v = min(v, dfs(step + 1, alpha, beta, fout));
            if (beta > v)
            {
                beta = v;
            }
            board[tmp.first][tmp.second] = 0;
            if (beta <= alpha)
                return alpha;
        }
        return v;
    }
    return 0;
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
    dfs(0, -INF, INF, fout);
    fin.close();
    fout.close();
    return 0;
}
