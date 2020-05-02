#include<iostream>
#include<string>
#include<sstream>    // For istringstream(string x)
#include<fstream>
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()
using namespace std;

struct edge;
struct Node;

class Priority_Queue
{
	struct queuenode
	{
		int row;
		int col;
		int heuristic_value;
		queuenode *next_q;
	};
	int no_of_items;
public:
	queuenode* front;
	//queuenode* rear;
	Priority_Queue(int size)
	{
		front = NULL;
		//rear = NULL;
		no_of_items = 0;
	}
	
	void enqueue(int n1, int n2, int n3)
	{
		queuenode *ptr, *temp;
		ptr = new queuenode;
		ptr->row = n1;
		ptr->col = n2;
		ptr->heuristic_value = n3;
		if (isempty() || ptr->heuristic_value < front->heuristic_value)
		{
			ptr->next_q = front;
			front = ptr;
		}
		else
		{
			temp = front;
			while (temp->next_q != NULL && temp->next_q->heuristic_value <= n3)
			{
				temp = temp->next_q;
			}
			ptr->next_q = temp->next_q;
			temp->next_q = ptr;
		}
		no_of_items++;
	}
	string dequeue()
	{
		if (!isempty())
		{
			queuenode *temp;
			int n1 = front->row;
			int n2 = front->col;
			int n3 = front->heuristic_value;
	     	temp = front->next_q;
			delete front;
			front = temp;
			no_of_items--;

			string n4 = to_string(n1);
			string n5 = to_string(n2);
			string n6 = to_string(n3);

			string n = n4 + " " + n5 + " " + n6;

			return n;
		}
	}
	bool isempty()
	{
		if (no_of_items == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	~Priority_Queue(){
		// destructor
	}
};

struct Node
{
	int cell_value;
	int row;
	int col;
	int cost;
	int heuristic_value;
	string  color;
	Node *next;
	edge *e_head;
	edge *e_tail;
	int d;
};

struct edge
{
	int edge_row;
	int edge_col;
	int cost_e;
	int heuristicValue_e;
	Node *p;
	int d;
	string color;
	edge *next_e;

};

// -----  function definitions  -----

void create_graph(Node* & head, Node* & tail, Node *ptr1)
{
	if (head == NULL)
	{
		head = ptr1;
		tail = ptr1;
	}
	else
	{
		tail->next = ptr1;
		tail = ptr1;
	}
}

edge *create_edges(edge* &e_tail, edge* &e_head, edge* E)
{
	edge *temp1 = e_head;
	if (e_head == NULL)  // NULL
	{
		e_head = E;
		e_tail = e_head;
	}
	else
	{
		while (temp1->next_e != NULL)
		{
			temp1 = temp1->next_e;
		}
		temp1->next_e = E;
		e_tail = temp1;
	}
	return E;
}

void BestFirst_Search(Node* head, Node* s, int size, edge* &e_head, int key_row, int key_col, int goal_row, int goal_col, string** arr)
{
	Priority_Queue q2(size);
	cout << "Best First Search: \n";
	cout << "==================\n";
	int count = 0;
	s = head;
	while (s != NULL)
	{
		if (key_row == s->row && key_col == s->col){
			break;
		}
		s = s->next;
	}
	if (key_row == s->row && key_col == s->col)
	{
		Node* u = head;
		s->cost = 0;
		q2.enqueue(s->row, s->col, s->heuristic_value);
		u->cost = 0;
		while (!q2.isempty())
		{
			string x = q2.dequeue();
			int r, c, h;
			string x1 = x.substr(0, 1);              // starting from 0 index pick only 1 digit
			string x2 = x.substr(2, 1);
			string x3 = x.substr(4, 1);
			//converting string to int
			istringstream(x1) >> r;
			istringstream(x2) >> c;
			u = head;
			count++;
			while (r != u->row || c != u->col)
			{
				u = u->next;                // find the source
			}
			int distance = 0;
			edge *v = u->e_head;
			while (v != NULL)
			{
				if (v->color == "white")
				{
					v->color = "grey";
					q2.enqueue(v->edge_row, v->edge_col, v->heuristicValue_e);

					Node* testing_pointer1 = head;
					edge* testing_pointer2;
					while (testing_pointer1 != NULL)
					{
						testing_pointer2 = testing_pointer1->e_head;
						while (testing_pointer2 != NULL)
						{
							if (testing_pointer2->edge_row == v->edge_row && testing_pointer2->edge_col == v->edge_col)
							{
								if (testing_pointer2->color == "white")
								{
									testing_pointer2->color = "grey";
								}
								break;
							}
							testing_pointer2 = testing_pointer2->next_e;
						}
						if (testing_pointer1->row == v->edge_row && testing_pointer1->col == v->edge_col && testing_pointer1->color == "white")
						{
							testing_pointer1->color = "grey";
							testing_pointer1->cost = u->cost + v->cost_e;
						}
						testing_pointer1 = testing_pointer1->next;
					}
				}
				v = v->next_e;
			} // end while loop for v 
			u->color = "black";

			if (u->row == key_row && u->col == key_col)
			{
				arr[u->row][u->col] = "S";
			}
			else
			{
				arr[u->row][u->col] = "*";
			}

			if (u->row == goal_row && u->col == goal_col){
				arr[u->row][u->col] = "G";
				cout << "Final Path Cost: " << u->cost<<endl;
				cout << "Goal finded!\n";
				break;
			}

		}
	}
}
void creatingEdges(int edge_row, int edge_column, int cost, int edge_heuristic, Node* &ptr2)
{
	edge *E;
	E = new edge;
	E->edge_row = edge_row;
	E->edge_col = edge_column;
	E->cost_e = cost;
	E->heuristicValue_e = edge_heuristic;
	E->color = "white";        // white color represent that the vertex is neither expanded  nor visited.
	E->next_e = NULL;
	//E->d = cost;
	
	create_edges(ptr2->e_tail, ptr2->e_head, E);
}

bool search(int key_row, int key_col, Node* temp1)
{
	while (temp1 != NULL){
		if (temp1->row == key_row && temp1->col == key_col)
		{
			break;
		}
		temp1 = temp1->next;
	}
	if (temp1 == NULL)
	{
		return false;
	}
	if (temp1->cell_value == 0){
		return true;
	}
}

void display(string **arr)
{
	cout << "\nFinal Path\n\t  ";
	for (int i = 0; i < 14; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			cout << arr[i][j] << "  ";
		}
		cout << endl << "\t  ";
	}
}

int main()
{
	int t_row, t_col, size, ver, edg, d, vertex_row, vertex_col;
	Node *head = NULL, *tail = NULL;
	Node *ptr1 = head;
	int reading_value;
	int start_key_col, start_key_row, goal_key_col, goal_key_row;
	int right_col, cost;

	string** arr = new string*[14];        // declare a string array for display the path
	for (int i = 0; i < 14; i++)
	{
		arr[i] = new string[10];
	}

	ifstream in;


	in.open("file1.txt");
	if (!in.is_open()){
		cout << "File can not open\n";
	}
	else{
		in >> reading_value;
		t_col = reading_value;
		in >> reading_value;
		t_row = reading_value;
		in >> reading_value;
		start_key_col = reading_value;
		in >> reading_value;
		start_key_row = reading_value;
		in >> reading_value;
		goal_key_col = reading_value;
		in >> reading_value;
		goal_key_row = reading_value;
	}
	// making nodes

	srand(time(0));  // Initialize random number generator.
	
	for (int i = 0; i<t_row; i++)
	{
		for (int j = 0; j<t_col; j++)
		{
			vertex_row = i;
			vertex_col = j;

			// reading from file.
			in >> reading_value;
			if (reading_value == 0){

				ptr1 = new Node;
				ptr1->row = vertex_row;
				ptr1->col = vertex_col;
				ptr1->cell_value = reading_value;
				ptr1->heuristic_value = (rand() % 40) + 1;
				ptr1->color = "white";
				ptr1->d = 0;
				ptr1->e_tail = ptr1->e_head = NULL;
				ptr1->next = NULL;
				create_graph(head, tail, ptr1);
				arr[vertex_row][vertex_col] = "0";
			}
			else{
				arr[vertex_row][vertex_col] = "1";
			}
		} // end of inner for loop--columns
	}   // end of outer for loop--rows

	// making edges
	Node *ptr2 = head;
	while (ptr2 != NULL)
	{
		Node *temp = head;

		// create an edge for moving upward if possible
		int key_row = ptr2->row;
		key_row = key_row - 1;
		int key_col = ptr2->col;
		int key_heuristic;

		if (key_row >= 0){
			bool result = search(key_row, key_col, temp);
			if (result == true)
			{
				Node* ptr3 = head;
				while (ptr3 != NULL)
				{
					if (key_row == ptr3->row && key_col == ptr3->col)
					{
						key_heuristic = ptr3->heuristic_value;
						break;
					}
					ptr3 = ptr3->next;
				}
				creatingEdges(key_row, key_col, 1, key_heuristic, ptr2);
			}
		}

		// creating an edge for rightward move if possible
		key_row = ptr2->row;
		key_col = ptr2->col;
		key_col = key_col + 1;
		temp = head;
		if (key_col < t_col)
		{
			bool result = search(key_row, key_col, temp);
			if (result == true)
			{
				Node* ptr3 = head;
				while (ptr3 != NULL)
				{
					if (key_row == ptr3->row && key_col == ptr3->col)
					{
						key_heuristic = ptr3->heuristic_value;
						break;
					}
					ptr3 = ptr3->next;
				}
				creatingEdges(key_row, key_col, 3, key_heuristic, ptr2);
			}
		}

		// creating an edge for diagonally rightward move if possible
		key_row = ptr2->row;
		key_row = key_row - 1;
		key_col = ptr2->col;
		key_col = key_col + 1;
		temp = head;
		if (key_col < t_col && key_row >= 0)
		{
			bool result = search(key_row, key_col, temp);
			if (result == true)
			{
				Node* ptr3 = head;
				while (ptr3 != NULL)
				{
					if (key_row == ptr3->row && key_col == ptr3->col)
					{
						key_heuristic = ptr3->heuristic_value;
						break;
					}
					ptr3 = ptr3->next;
				}
				creatingEdges(key_row, key_col, 2, key_heuristic, ptr2);
			}
		}
		ptr2 = ptr2->next;
		//		cout << "\nNext node: ";
	}

	int key;
	Node *temp1 = head;
	cout << endl;

	Node *s = head;
	Node *temp2 = head;

	size = t_row * t_col;


	BestFirst_Search(head, s, size, temp2->e_head, start_key_row, start_key_col, goal_key_row, goal_key_col, arr);

	display(arr);
	for (int i = 0; i < 14; i++)
	{
		delete[] arr[i];
	}
	delete[] arr;

	system("pause");
	return 0;
}
