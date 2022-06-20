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
            if (ref[2] == '.')
            {
                // TWO .O.O.
                if (ref == ".O.O.")
                    return TWO;
                if (ref == ".X.X.")
                    return typenum + TWO;
            }
            else
            {
                // THREE .OOO.
                if (ref == ".OOO.")
                    return THREE;
                if (ref == ".XXX.")
                    return typenum + THREE;
            }
        }
        else
        {
            if (ref[1] == '.')
            {
                // SLEEP_TWO O...O
                if (ref == "O...O")
                    return SLEEP_TWO;
                if (ref == "X...X")
                    return typenum + SLEEP_TWO;
                // SLEEP_THREE O..OO || O.O.O
                if (ref == "O..OO" || ref == "O.O.O")
                    return SLEEP_THREE;
                if (ref == "X..XX" || ref == "X.X.X")
                    return typenum + SLEEP_THREE;
            }
            else
            {
                if (ref[2] == '.')
                {
                    // SLEEP_THREE OO..O
                    if (ref == "OO..O")
                        return SLEEP_THREE;
                    if (ref == "XX..X")
                        return typenum + SLEEP_THREE;
                    // DEAD_FOUR OO.OO
                    if (ref == "OO.OO")
                        return DEAD_FOUR;
                    if (ref == "XX.XX")
                        return typenum + DEAD_FOUR;
                }
                else
                {
                    // DEAD_FOUR O.OOO || OOO.O
                    if (ref == "O.OOO" || ref == "OOO.O")
                        return DEAD_FOUR;
                    if (ref == "X.XXX" || ref == "XXX.X")
                        return typenum + DEAD_FOUR;
                    // FIVE OOOOO
                    if (ref == "OOOOO")
                        return FIVE;
                    if (ref == "XXXXX")
                        return typenum + FIVE;
                }
            }
        }
    }
    if (ref.size() == 6)
    {
        if (ref[1] == '.' && ref[0] == '.')
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
            if (ref[0] == '.')
            {
                if (ref[5] == '.')
                {
                    // TWO .O..O.
                    if (ref == ".O..O.")
                        return TWO;
                    if (ref == ".X..X.")
                        return typenum + TWO;
                    // JUMP_THREE OO.O || O.OO
                    if (ref == ".OO.O." || ref == ".O.OO.")
                        return JUMP_THREE;
                    if (ref == ".XX.X." || ref == ".X.XX.")
                        return typenum + JUMP_THREE;
                    // FOUR .OOOO.
                    if (ref == ".OOOO.")
                        return FOUR;
                    if (ref == ".XXXX.")
                        return typenum + FOUR;
                }
                else
                {
                    // SLEEP_TWO .O..OX
                    if (ref == ".O..OX")
                        return SLEEP_TWO;
                    if (ref == ".X..XO")
                        return typenum + SLEEP_TWO;
                    // SLEEP_THREE .O.OOX || .OO.OX
                    if (ref == ".O.OOX" || ref == ".OO.OX")
                        return SLEEP_THREE;
                    if (ref == ".X.XXO" || ref == ".XX.XO")
                        return typenum + SLEEP_THREE;
                    // DEAD_FOUR .OOOOX
                    if (ref == ".OOOOX")
                        return DEAD_FOUR;
                    if (ref == ".XXXXO")
                        return typenum + DEAD_FOUR;
                }
            }
            else
            {
                // SLEEP_TWO XOO... || XO.O.. || XO..O. || .O..OX
                if (ref == "XOO..." || ref == "XO.O.." || ref == "XO..O.")
                    return SLEEP_TWO;
                if (ref == "OXX..." || ref == "OX.X.." || ref == "OX..X.")
                    return typenum + SLEEP_TWO;
                // SLEEP_THREE XOOO.. || ..OOOX || XOO.O. || XO.OO.
                if (ref == "XOOO.." || ref == "XOO.O." || ref == "XO.OO.")
                    return SLEEP_THREE;
                if (ref == "OXXX.." || ref == "OXX.X." || ref == "OX.XX.")
                    return typenum + SLEEP_THREE;
                // DEAD_FOUR XOOOO.
                if (ref == "XOOOO.")
                    return DEAD_FOUR;
                if (ref == "OXXXX.")
                    return typenum + DEAD_FOUR;
            }
        }
    }
    if (ref.size() == 7)
    {
        // SLEEP_THREE X.OOO.X
        if (ref == "X.OOO.X")
            return SLEEP_THREE;
        if (ref == "O.XXX.O")
            return typenum + SLEEP_THREE;
        // THREE
        if (ref == ".O.O.O.")
            return SLEEP_THREE;
        if (ref == ".X.X.X.")
            return typenum + SLEEP_THREE;
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
            // def
            val -= type[typenum + FIVE] * dic[0];
            val -= type[typenum + FOUR] * dic[1];
            val -= type[typenum + DEAD_FOUR] * dic[1];
            val -= type[typenum + JUMP_THREE] * dic[3];
            val -= type[typenum + THREE] * dic[3];
            val -= type[typenum + SLEEP_THREE] * dic[7];
            val -= type[typenum + TWO] * dic[8];
            val -= type[typenum + SLEEP_TWO] * dic[9];
            // atk
            val += type[FIVE] * dic[0];
            val += type[FOUR] * dic[2];
            val += type[DEAD_FOUR] * dic[4];
            val += type[JUMP_THREE] * dic[5];
            val += type[THREE] * dic[5];
            val += type[SLEEP_THREE] * dic[7];
            val += type[TWO] * dic[8];
            val += type[SLEEP_TWO] * dic[9];
            if (type[DEAD_FOUR] + type[JUMP_THREE] + type[THREE] > 2)
                val += dic[2];
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
            if (type[DEAD_FOUR] + type[JUMP_THREE] + type[THREE] > 2)
                val += dic[2];
        }
    }
};

int dfs(int step, int alpha, int beta)
{
    // initialize
    int cur;
    if ((player == 1 && (step & 1) == 0) || (player == 2 && (step & 1) == 1))
        cur = 1;
    else
        cur = 2;

    // base case
    if (step == maxmove)
    {
        state cur(step & 1);
        if (cur.val > 7000000)
            return INF;
        return cur.val;
    }

    // find candicate
    int dist[15][15];
    vector<pair<int, pair<int, int>>> candicate;
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
                    board[x][y] = cur;
                    state next((step & 1) ^ 1);
                    cnt++;
                    board[x][y] = 0;
                    candicate.emplace_back(next.val, make_pair(x, y));
                    q.emplace(x, y);
                }
            }
        }
    }
    if (candicate.empty())
        candicate.emplace_back(0, make_pair(8, 7));
    sort(candicate.rbegin(), candicate.rend());
    if (step != 0 && candicate[0].first > 7000000)
    {
        if (step & 1)
            return -INF;
        else
            return INF;
    }
    int size = candicate.size();
    if (size > 20)
        candicate.resize(20);

    // recursion
    if (step % 2 == 0)
    {
        int v = -INF;
        for (auto tmp : candicate)
        {
            board[tmp.second.first][tmp.second.second] = cur;
            v = max(v, dfs(step + 1, alpha, beta));
            if (v > alpha)
            {
                alpha = v;
                if (step == 0)
                    ans = tmp.second;
            }
            board[tmp.second.first][tmp.second.second] = 0;
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
            board[tmp.second.first][tmp.second.second] = cur;
            v = min(v, dfs(step + 1, alpha, beta));
            if (beta > v)
            {
                beta = v;
            }
            board[tmp.second.first][tmp.second.second] = 0;
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
    for (maxmove = 1; true; maxmove += 2)
    {
        int score = dfs(0, -INF, INF);
        fout << ans.first << ' ' << ans.second << endl;
        if (score == INF)
            break;
    }
    fin.close();
    fout.close();
    return 0;
}
