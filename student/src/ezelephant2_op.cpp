#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <array>
#include <queue>
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
const int dir[4][2] = {-1, 1, 0, 1, 1, 1, 1, 0};
const int dic[10] = {94879487, 9487487, 948787, 94878, 48700, 9487, 4870, 870, 487, 87};
const int Dir[8][2] = {-1, 1, 0, 1, 1, 1, 1, 0, 1, -1, 0, -1, -1, -1, -1, 0};
int maxmove = 3;

int cnt, center_x, center_y;
pair<int, int> ans;

int judge(string &ref)
{
    if (ref.size() == 5)
    {
        if (ref[0] == '.')
        {
            // TWO .O.O.
            if (ref == ".O.O.")
                return TWO;
            if (ref == ".X.X.")
                return typenum + TWO;
            // THREE .OOO.
            if (ref == ".OOO.")
                return THREE;
            if (ref == ".XXX.")
                return typenum + THREE;
        }
        else
        {
            // SLEEP_TWO O...O
            if (ref == "O...O")
                return SLEEP_TWO;
            if (ref == "X...X")
                return typenum + SLEEP_TWO;
            // SLEEP_THREE O..OO || OO..O || O.O.O
            if (ref == "O..OO" || ref == "OO..O" || ref == "O.O.O")
                return SLEEP_THREE;
            if (ref == "X..XX" || ref == "XX..X" || ref == "X.X.X")
                return typenum + SLEEP_THREE;
            // DEAD_FOUR OO.OO || O.OOO || OOO.O
            if (ref == "OO.OO" || ref == "O.OOO" || ref == "OOO.O")
                return DEAD_FOUR;
            if (ref == "XX.XX" || ref == "X.XXX" || ref == "XXX.X")
                return typenum + DEAD_FOUR;
            // FIVE OOOOO
            if (ref == "OOOOO")
                return FIVE;
            if (ref == "XXXXX")
                return typenum + FIVE;
        }
    }
    if (ref.size() == 6)
    {
        if (ref[0] == '.' && ref[1] == '.')
        {
            // SLEEP_TWO ...OOX || ..O.OX
            if (ref == "...OOX" || ref == "..O.OX")
                return SLEEP_TWO;
            if (ref == "...XXO" || ref == "..X.XO")
                return typenum + SLEEP_TWO;
            // TWO ..OO..
            if (ref == "..OO..")
                return TWO;
            if (ref == "..XX..")
                return typenum + TWO;
            // SLEEP_THREE ..OOOX
            if (ref == "..OOOX")
                return SLEEP_THREE;
            if (ref == "..XXXO")
                return typenum + SLEEP_THREE;
        }
        else
        {
            // SLEEP_TWO XOO... || XO.O.. || XO..O. || .O..OX
            if (ref == "XOO..." || ref == "XO.O.." || ref == "XO..O." || ref == ".O..OX")
                return SLEEP_TWO;
            if (ref == "OXX..." || ref == "OX.X.." || ref == "OX..X." || ref == ".X..XO")
                return typenum + SLEEP_TWO;
            // TWO ..OO.. || .O..O.
            if (ref == ".O..O.")
                return TWO;
            if (ref == ".X..X.")
                return typenum + TWO;
            // SLEEP_THREE XOOO.. || ..OOOX || XOO.O. || .O.OOX || XO.OO. || .OO.OX
            if (ref == "XOOO.." || ref == "XOO.O." || ref == ".O.OOX" || ref == "XO.OO." || ref == ".OO.OX")
                return SLEEP_THREE;
            if (ref == "OXXX.." || ref == "OXX.X." || ref == ".X.XXO" || ref == "OX.XX." || ref == ".XX.XO")
                return typenum + SLEEP_THREE;
            // JUMP_THREE OO.O || O.OO
            if (ref == ".OO.O." || ref == ".O.OO.")
                return JUMP_THREE;
            if (ref == ".XX.X." || ref == ".X.XX.")
                return typenum + JUMP_THREE;
            // DEAD_FOUR XOOOO. || .OOOOX
            if (ref == "XOOOO." || ref == ".OOOOX")
                return DEAD_FOUR;
            if (ref == "OXXXX." || ref == ".XXXXO")
                return typenum + DEAD_FOUR;
            // FOUR .OOOO.
            if (ref == ".OOOO.")
                return FOUR;
            if (ref == ".XXXX.")
                return typenum + FOUR;
        }
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
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                int space = 0;
                for (int d = 0; d < 4; d++)
                {
                    string tmp;
                    x = i, y = j;
                    while (y < SIZE && x < SIZE && x >= 0 && tmp.size() < 7 && y >= 0)
                    {
                        switch (board[x][y])
                        {
                        case EMPTY:
                            tmp += '.';
                            space++;
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

struct cmp
{
    bool operator()(const pair<int, int> &a, const pair<int, int> &b) const
    {
        return abs(center_x - a.first) + abs(center_y - a.second) < abs(center_x - b.first) + abs(center_y - b.second);
    }
};

int dfs(int step, int alpha, int beta)
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
    int flag_num = 0;
    center_x = 0, center_y = 0;
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
        {
            if (board[i][j] != 0)
            {
                center_x += i;
                center_y += j;
                flag_num++;
                q.emplace(i, j);
                dist[i][j] = 0;
            }
            else
                dist[i][j] = 87;
        }
    if (flag_num != 0)
    {
        center_x /= flag_num;
        center_y /= flag_num;
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
    if (flag_num < 20)
        sort(candicate.begin(), candicate.end(), cmp());
    else
    {
        static mt19937 _rand(time(nullptr));
        shuffle(candicate.begin(), candicate.end(), _rand);
    }

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
            v = max(v, dfs(step + 1, alpha, beta));
            if (v > alpha)
            {
                alpha = v;
                if (step == 0)
                    ans = tmp;
            }
            board[tmp.first][tmp.second] = 0;
            if (beta <= alpha)
                break;
        }
        return v;
    }
    else
    {
        int v = INF;
        for (auto tmp : candicate)
        {
            board[tmp.first][tmp.second] = cur;
            v = min(v, dfs(step + 1, alpha, beta));
            if (beta > v)
            {
                beta = v;
            }
            board[tmp.first][tmp.second] = 0;
            if (beta <= alpha)
                break;
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
    maxmove = 1;
    dfs(0, -INF, INF);
    fout << ans.first << ' ' << ans.second << endl;
    maxmove = 3;
    dfs(0, -INF, INF);
    fout << ans.first << ' ' << ans.second << endl;
    fin.close();
    fout.close();
    return 0;
}
