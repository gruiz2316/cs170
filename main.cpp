#include <iostream>
#include <vector>
#include <deque>
using namespace std;

const int N = 3; // for NxN puzzle
int GOAL[N][N]; //goal state
vector<int**> nodes; //tree of nodes implemented as 2D array
deque<int**> Q; //queue of children
vector<int> visit; //to keep track of visited nodes to get to solution

void generate_goal() //generate the goal state
{
    int count = 1; //start with 1 for 1st tile
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            GOAL[i][j] = count;
            count++;
        }
    }
    
    GOAL[N - 1][N - 1] = 0; //last tile = 0
}

bool is_goal(int *a[N]) //test if input is goal state
{
    bool goal = true;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if(a[i][j] != GOAL[i][j])
                goal = false;
        }
    }
    
    return goal;
}

int misplaced(int *a[N]) //count number of misplaced tiles in input
{
    int count = 0;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if(a[i][j] != GOAL[i][j] && a[i][j] != 0) //don't count 0
                count++;
        }
    }
    return count;
}

int manhattan(int *a[N]) //calculate sum of manhattan distances for input
{
	int d = 0;
	
	if(is_goal(a)) return d;
	
	//loop through all tiles of input
	for(int x2 = 0; x2 < N; x2++)
	{
		for(int y2 = 0; y2 < N; y2++)
		{
			if(a[x2][y2] != 0) //don't count 0 (blank tile)
			{
			    //look for the right tile in goal state
				for(int x1 = 0; x1 < N; x1++)
				{
					for(int y1 = 0; y1 < N; y1++)
					{
						if(a[x2][y2] == GOAL[x1][y1])
							d += abs(x2 - x1) + abs(y2 - y1); //d = |x2 - x1| + |y2 - y1|
					}
				}
			}
		}
	}
	
	return d;
}

void print(int *a[N]) //print configuration as NxN table
{
    for(int i = 0; i < N; ++i)
    {
        for(int j = 0; j < N; ++j)
        {
            cout << a[i][j];
        }
        
        cout << endl;
    }
}

void print_path(int i) //print path from initail state to goal
{
    while(visit[i] != 0)
    {
        print(nodes[visit[i]]);
        cout << endl;
        i = visit[i];
    }
}

int** new_child(int *a[N]) //allocate memory for a new child node
{
    int** in = new int*[N]; 
    for(int i = 0; i < N; ++i)
        in[i] = new int[N];
        
      //copy to new child
    for(int i = 0; i < N; i ++)
        for(int j = 0; j < N; j++)
            in[i][j] = a[i][j];
            
    return in;
}

void operators(int *a[N], int index) //perform operators: up, down, left, right
{
    //first check if this node has been visited before
    bool visited = false;
    int count;
    for(int n = 0; n < index; n++)
    {
        count = 0;
        for(int i = 0; i < N; i++)
        {
            for(int j = 0; j < N; j++)
            {
                if(a[i][j] == nodes[n][i][j])
                    count++;
            }
        }
        if(count == N*N)
        {
            visited = true;
            break;
        }
    }
    
    if(!visited) { //if not visited expand
    
    //look for 0 (empty tile)
    int i0, j0; //to save the position of 0
    for(int i = 0; i < N; i ++)
        for(int j = 0; j < N; j++)
        {
            if(a[i][j] == 0) //found zero
            {
                //save i and j positiion of zero
                i0 = i;
                j0 = j;
            }
        }
        
    //Perform possible operators
    
    //swap with top neighbor if possible
    if((i0 - 1) >= 0)
    {
        int** in = new_child(a);
        in[i0][j0] = in[i0 - 1][j0];
        in[i0 - 1][j0] = 0;
        nodes.push_back(in); //add child to nodes
        Q.push_back(in); //add child to queue
        visit.push_back(index); //this node has been visited
    }
    
    //swap with right neighbor if possible
    if((j0 + 1) < N)
    {
        int** in = new_child(a);
        in[i0][j0] = in[i0][j0 + 1];
        in[i0][j0 + 1] = 0;
        nodes.push_back(in);
        Q.push_back(in);
        visit.push_back(index);
    }
    
     //swap with left neighbor if possible
    if((j0 - 1) >= 0)
    {
        int** in = new_child(a);
        in[i0][j0] = in[i0][j0 - 1];
        in[i0][j0 - 1] = 0;
        nodes.push_back(in);
        Q.push_back(in);
        visit.push_back(index);
    }
    
     //swap with bottom neighbor if possible
    if((i0 + 1) < N)
    {
        int** in = new_child(a);
        in[i0][j0] = in[i0 + 1][j0];
        in[i0 + 1][j0] = 0;
        nodes.push_back(in);
        Q.push_back(in);
        visit.push_back(index);
    }
    
  }
    
}

int general_search(int *in[N])
{
    nodes.push_back(in); //push initial state to nodes
    Q.push_back(in); //push initial state to queue
    visit.push_back(0); //initialize visited vector
    unsigned int i = 0; //index
    
    while(1)
    {
        if(Q.empty()) return -1; //return failure
        
        if(is_goal(Q.front())) return i; //return index of goal
        
        operators(Q.front(), i); //expand node
        
        Q.pop_front(); //remove front node
        
        i++;
    }

}

void sortQ(int index, int opt)
{
   int g1, g2; //g(n)
    
  // sort queue according to heuristic using bubble sort
  for (int j = 0; j < (Q.size() -1); j++) {
      
    for (int i = 0; i < (Q.size() - j - 1); i++) {
      
      if(opt == 0) //sort according to misplaced tile heuristic
      {
        g1 = misplaced(Q[i]);
        g2 = misplaced(Q[i + 1]);
      }
      else //sort according to manhattan heuristic
      {
        g1 = manhattan(Q[i]);
        g2 = manhattan(Q[i + 1]);
      }
      
      // compare g(n) of two adjacent elements
      if (g1 > g2) 
      {
        // swapping elements in Q
        int** temp = Q[i];
        Q[i] = Q[i + 1];
        Q[i + 1] = temp;
        
        // swapping elements in nodes
        temp = nodes[index + i];
        nodes[index + i] = nodes[index + i + 1];
        nodes[index + i + 1] = temp;
        
        // swapping elements in visited
        int tempi = visit[index + i];
        visit[index + i] = visit[index + i + 1];
        visit[index + i + 1] = tempi;
        
      }
    }
  }
}

int misplaced_heuristic(int *in[N])
{
    nodes.push_back(in); //push initial state to nodes
    Q.push_back(in); //push initial state to queue
    visit.push_back(0); //initialize visited vector
    unsigned int i = 0; //index
    
    while(1)
    {
        if(Q.empty()) return -1; //return failure
        
        sortQ(i, 0); //sort queue according to misplaced tile heuristic
        
        if(is_goal(Q.front())) return i; //return index of goal
        
        operators(Q.front(), i); //expand node
        
        Q.pop_front(); //remove front node
        
        i++;
    }
}

int manhattan_heuristic(int *in[N])
{
    nodes.push_back(in); //push initial state to nodes
    Q.push_back(in); //push initial state to queue
    visit.push_back(0); //initialize visited vector
    unsigned int i = 0; //index
    
    while(1)
    {
        if(Q.empty()) return -1; //return failure
        
        sortQ(i, 1); //sort queue according to manhattan heuristic
        
        if(is_goal(Q.front())) return i; //return index of goal
        
        operators(Q.front(), i); //expand node
        
        Q.pop_front(); //remove front node
        
        i++;
    }
}

int main()
{
    int option;
    int** in = new int*[N]; //input numbers as 2D array
    for(int i = 0; i < N; ++i)
        in[i] = new int[N];
    
    generate_goal(); //generate the goal state for this problem
    
    //get input from user
    cout << "Choose search." << endl;
    cout << "1. General search" << endl;
    cout << "2. Misplaced tile" << endl;
    cout << "3. Manhattan heuristic" << endl;
    cout << "Enter 1, 2 or 3: ";
    cin >> option;
    cout << endl << "Enter numbers (use 0 as blank): " << endl;
    for(int i = 0; i < N; i ++)
        for(int j = 0; j < N; j++)
            cin >> in[i][j];
            
    int i = -1; //index of solution
    switch(option)
    {
        case 1: 
                i = general_search(in);
                break;
                
        case 2: i = misplaced_heuristic(in);
                break;
                
        case 3: i = manhattan_heuristic(in);
                break;
                
        default: break;
    }
    
    if(i < 0)
        cout << "No solution" << endl;
    else
    {
        cout << "Solution from goal to ---> initial state:" << endl;
        print(nodes[i]); cout << endl;
        print_path(i);
        print(in);
        cout << "tree size: " << nodes.size() << endl;
    }
    
    
    //free allocated memory
    for(unsigned int i = 0; i < nodes.size(); i++)
    {
        for(int j = 0; j < N; j++)
            delete [] nodes[i][j];
            
        delete [] nodes[i];
    }

    return 0;
}


