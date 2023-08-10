

// theek hai ab toh chod de- 20 mins ke liye
// okkk
// mai abhi to ready hora anshika ke photoshoot keliye abd sumit dusre team me
// karra abhi kaam so its only us.

// same same i'll get ready for photoshoot, then i'll get back to this
// ok done!

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

using namespace std;
#define N (int)3 //--

using i64 = long long int; //--
#define INT_MAX 2147483647

// structure for tree nodes
struct Node
{
    Node *parent;            // here we store the current node's parent node; helping to
                             // trace the path when we find the answer
    int mtx[N][N];           // to store the matrix and its values
    bool discovered = false; // in Dijkstra Algorithm discovered is used to mark a
                             // vertex that has been discovered
    bool visited = false;    // used in BFS
    int x, y;                // to store the blank coordinates
    int cost;                // to store # misplaced tiles
    int dis;                 // to store depth of DFS and distance in BFS, Dijkstra
    int moved;               // to store the number which is on the moved tile
    int depth;

    // below we are comparing two nodes
    bool operator==(Node rhs)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                if (this->mtx[i][j] != rhs.mtx[i][j])
                    return false;
        }
        return true;
    }
};

// below is the function we use to print the matrix (N x N)
void print_mtx(int mtx[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            cout << mtx[i][j];
        cout << endl;
    }
}

// in the function below we are allocating a new node
Node *new_node(int mtx[N][N], int x, int y, int new_x, int new_y, int moved,
               int dis, Node *parent)
{
    Node *node = new Node;
    node->parent = parent; // setting the pointer to root
    memcpy(node->mtx, mtx,
           sizeof node
               ->mtx); // transferring data to current node from the parent node
    swap(node->mtx[x][y],
         node->mtx[new_x][new_y]); // here we are moving the tile by 1 position
    node->cost = INT_MAX - 1;      // #misplaced tiles
    node->moved =
        moved; // here we are setting a number for the tile which is to be moved
    node->depth = 0;
    node->dis =
        dis; // distance is being set from the initial point to this vertex
    // below we are updating the new tile co-ordinates
    node->x = new_x;
    node->y = new_y;

    return node;
}

int row[] = {1, 0, -1, 0}; // bottom, left, top, right
int col[] = {0, -1, 0, 1}; // bottom, left, top, right

// below is the function which we are using to calculate the #misplaced tiles
int cal_cost(int start[N][N], int final[N][N])
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (start[i][j] && start[i][j] != final[i][j])
            {
                count++;
            }
        }
    }
    return count;
}

// using the below function to check if the co-ordinate (x, y) is valid
int check(int x, int y) { return (x >= 0 && x < N && y >= 0 && y < N); }

// below function helps us print the path starting from the root node till the
// destination node
void print_path(Node *root)
{
    if (root == NULL)
        return;
    print_path(root->parent);
    print_mtx(root->mtx);

    cout << endl;
}

bool cmp(Node *lhs, Node *rhs) // comparing two node pointers
{
    return (*lhs == *rhs);
}

struct comp
{
    bool operator()(const Node *lhs, const Node *rhs) const
    {
        return (lhs->cost + lhs->dis) > (rhs->cost + rhs->dis);
    }
};

bool node_cmp(const Node *lhs, const Node *rhs) // helps the min_el algo
{
    return (lhs->dis) < (rhs->dis);
}

// Dijkstra's here

// 8 puzzle using a star search algorithm. The cost of a move in the 8-puzzle is
// equal to the number of the moved tile. That is, the cost of moving tile 1 is
// $1, the cost of moving tile 2 is $2, and so on.

// below function is to help solve the 8 Puzzle with the help of Dijkstra's
// Algorithm (x & y = blank tile co-ordinates which are in their starting
// state)
void dijkstra(int start[N][N], int x, int y, int final[N][N])
{
    vector<Node *> abc; // to store the created vertices of the graph

    // below we are creating a root node with the starting state
    Node *r1 = new_node(start, x, y, x, y, 0, 0, NULL);
    r1->cost = cal_cost(start, final);

    abc.push_back(r1);

    while (!abc.empty())
    {
        // here we are trying to get the vertex with the most minimum dist
        Node *min = *min_element(abc.begin(), abc.end(), node_cmp);

        // to avoid further relaxation of the vertex, we set discovered to true
        min->discovered = true;

        if (min->cost == 0)
        {
            print_path(min); // printing path from root -> destination
            cout << "Cost: " << min->dis << endl;
            return;
        }
        // visit every child, or every possible next step from the puzzle's
        // current
        // state
        for (int i = 0; i < 4; i++)
        {
            if (check(min->x + row[i], min->y + col[i])) // checking move
            {
                // ch = child
                Node *ch =
                    new_node(min->mtx, min->x, min->y, min->x + row[i], min->y + col[i],
                             min->mtx[min->x + row[i]][min->y + col[i]], 0, min);

                ch->cost = cal_cost(ch->mtx, final);
                ch->dis = min->dis + ch->moved;

                // relaxation of the distance
                unsigned int j = 0;

                while (j < abc.size())
                {
                    if (*ch == *abc[j] && abc[j]->discovered != true)
                        break;
                    j++;
                }

                if (j != abc.size()) // checks if vector already has the vertex
                {
                    if (ch->dis <
                        abc[j]->dis) // comparing if new dist is lesser than previous
                    {
                        abc[j]->dis = ch->dis;
                    }
                }
                else // if vertex is not in the vector -> child is added
                {
                    abc.push_back(ch);
                }
            }
        }

        // we are doing this to make sure min_el does not visit some discovered
        // vertex again
        min->dis = INT_MAX;
    }
}

// BFS

// below function is to help solve the 8 Puzzle with the help of BFS (x & y =
// blank tile co-ordinates which are in their starting state)
void BFS(int start[N][N], int x, int y, int final[N][N])
{
    vector<Node *> abc; // to store vertices

    Node *r1 = new_node(start, x, y, x, y, 0, 0,
                        NULL); // creating a root node which has the start
                               // state
    r1->visited = true;

    abc.push_back(r1);

    while (!abc.empty())
    {
        // vector acting as a queue here:
        Node *min = abc.front();
        for (auto d : abc)
        { // taking the first undiscovered vertex from front
            if (d->discovered == false)
            {
                min = d;
                break;
            }
        }
        // to delete the vertex form the queue, we mark discovered as true now
        // (not
        // really removed because used to print solution)
        min->discovered = true;

        if (min->cost == 0)
        {
            // printing path from root->destination
            print_path(min);
            cout << "Cost: " << min->dis << endl;
            return;
        }

        // visiting every child, or every possible next step from current state
        for (int i = 0; i < 4; i++)
        {
            if (check(min->x + row[i], min->y + col[i]))
            {
                Node *ch = new_node(min->mtx, min->x, min->y, min->x + row[i],
                                    min->y + col[i], 0, 0, min);
                ch->cost = cal_cost(ch->mtx, final);
                ch->dis = min->dis + 1; // adding cost

                unsigned int j = 0;
                while (j < abc.size())
                {
                    if (*ch == *abc[j] && abc[j]->discovered != true)
                        break;
                    j++;
                }

                if (j !=
                    abc.size()) // dis is updated if child is already found in the
                // queue
                {
                    abc[j]->dis = ch->dis;
                }
                else // child is added to queue if not already present
                {
                    abc.push_back(ch);
                    ch->visited = true;
                }
            }
        }
    }
}

// // DFS

// new DFS code
void DFS(int start[N][N], int x, int y, int final[N][N], int depth_limit)
{
    priority_queue<Node *, vector<Node *>, comp> pq;
    Node *root = new_node(start, x, y, x, y, 0, 0, NULL);
    root->cost = cal_cost(start, final);
    root->depth = 0;
    pq.push(root);

    while (!pq.empty())
    {
        Node *min = pq.top();
        pq.pop();

        if (min->cost == 0)
        {
            print_path(min);
            return;
        }

        if (min->depth >= depth_limit)
        {
            continue;
        }

        for (int i = 0; i < 4; i++)
        {
            if (min->x + row[i] >= 0 && min->x + row[i] < N && min->y + col[i] >= 0 &&
                min->y + col[i] < N)
            {
                Node *child = new_node(min->mtx, min->x, min->y, min->x + row[i],
                                       min->y + col[i], 0, 0, min);
                child->depth = min->depth + 1;
                pq.push(child);
            }
        }
    }
    print_path(root);
    cout << "cost is : " << root->cost << endl;
}

// end of dfs new

// // below is the helper function used in DFS which is used to solve the 8
// Puzzle void helper_DFS(Node *min, int final[N][N], Node *answer) {
//   if (min->cost == 0) {
//     if (answer->dis > min->dis) {
//       answer->parent = min->parent;
//       answer->dis = min->dis;
//       memcpy(answer->mtx, min->mtx, sizeof answer->mtx);
//     }
//   }
//   else {
//     // visiting every child
//     for (int i = 0; i < 4; i++) {
//       if (check(min->x + row[i], min->y + col[i])) {
//         Node *ch = new_node(min->mtx, min->x, min->y, min->x + row[i],
//                             min->y + col[i], 0, 0, min);
//         ch->cost = cal_cost(ch->mtx, final);
//         ch->dis = min->dis + 1;

//         if (ch->dis < 15) {
//           helper_DFS(ch, final, answer);
//         }
//       }
//     }
//   }
// }

// // below function is to help solve the 8 Puzzle with the help of DFS (x & y =
// // blank tile co-ordinates which are in their starting state)
// void DFS(int start[N][N], int x, int y, int final[N][N]) {

//   Node *r1 = new_node(start, x, y, x, y, 0, 0, NULL);
//   Node *t = new_node(start, x, y, x, y, 0, INT_MAX, NULL);

//   r1->cost = cal_cost(start, final);
//   helper_DFS(r1, final, t);
//   print_path(t);
//   cout << "Cost: " << t->dis << endl;

// }

// below is the driver code:
int main()
{
    int start[N][N] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    // initial arrangement (empty space = 0)

    // filling start with input file
    ifstream inp;
    inp.open("input_file1.txt");
    if (inp)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                inp >> start[i][j];
            }
        }
    }
    inp.close();

    // final solvable arrangement (empty space = 0)\
  //ruk ja
    int final[N][N] = {{1, 2, 3}, {8, 0, 4}, {7, 6, 5}};

    // blank tile co-ordinates in initial arrangement:

    int x = 1, y = 1;
    cout << "------8 Puzzle Solver-----" << endl
         << endl;

    cout << "Solution using Dijkstra's Algortihm" << endl
         << endl;
    dijkstra(start, x, y, final);

    cout << endl
         << "Solution using BFS" << endl;
    BFS(start, x, y, final);

    cout << "Solution using DFS" << endl;
    int max_depth = 10;
    DFS(start, 1, 1, final, max_depth);

    // DFS(start, x, y, final);

    cout << "thanks" << endl;
    return 0;
}