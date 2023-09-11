#include <stdio.h>
#include <stdlib.h>

#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define MaxLength 500

const char *action[] = {"First State", "Move Empty Cell UP", "Move Empty Cell DOWN", 
						"Move Empty Cell LEFT", "Move Empty Cell RIGHT"};
						
//khai bao state
typedef struct{
	int eightpuzzle[ROWS][COLS];
	int emptyRow;
	int emptyCol;
}State;

void print_State(State state){
	int row, col;
	printf("\n----------\n");
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++){
			printf("|%d ", state.eightpuzzle[row][col]);
		}
		printf("|\n");
	}
	printf("----------\n");
}

int compareStates(State state1, State state2){
	if(state1.emptyCol != state2.emptyCol || state1.emptyRow != state2.emptyRow){
		return 0;
	}
	int row, col;
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++){
			if(state1.eightpuzzle[row][col] != state2.eightpuzzle[row][col]){
				return 0;
			}
		}
	}
	return 1;
}

int goalcheck(State state, State goal){
	return compareStates(state, goal);
}

int up(State state, State *result){
	*result = state;
	int empRowCur = state.emptyRow;
	int empColCur = state.emptyCol;
	if(empRowCur > 0){
		result->emptyCol = empColCur;
		result->emptyRow = empRowCur - 1;
		result->eightpuzzle[empRowCur][empColCur] = state.eightpuzzle[empRowCur - 1][empColCur];
		result->eightpuzzle[empRowCur - 1][empColCur] = EMPTY;
		return 1;
	}
	return 0;
}

int down(State state, State *result){
	*result = state;
	int empRowCur = state.emptyRow;
	int empColCur = state.emptyCol;
	if(empRowCur < 2){
		result->emptyCol = empColCur;
		result->emptyRow = empRowCur + 1;
		result->eightpuzzle[empRowCur][empColCur] = state.eightpuzzle[empRowCur + 1][empColCur];
		result->eightpuzzle[empRowCur + 1][empColCur] = EMPTY;
		return 1;
	}
	return 0;
}

int left(State state, State *result){
	*result = state;
	int empRowCur = state.emptyRow;
	int empColCur = state.emptyCol;
	if(empColCur > 0){
		result->emptyCol = empColCur - 1;
		result->emptyRow = empRowCur;
		result->eightpuzzle[empRowCur][empColCur] = state.eightpuzzle[empRowCur][empColCur - 1];
		result->eightpuzzle[empRowCur][empColCur - 1] = EMPTY;
		return 1;
	}
	return 0;
}

int right(State state, State *result){
	*result = state;
	int empRowCur = state.emptyRow;
	int empColCur = state.emptyCol;
	if(empColCur < 2){
		result->emptyCol = empColCur + 1;
		result->emptyRow = empRowCur;
		result->eightpuzzle[empRowCur][empColCur] = state.eightpuzzle[empRowCur][empColCur + 1];
		result->eightpuzzle[empRowCur][empColCur + 1] = EMPTY;
		return 1;
	}
	return 0;
}

int call_operators(State state, State *result, int option){
	switch(option){
		case 1: return up(state, result);
		case 2: return down(state, result);
		case 3: return left(state, result);
		case 4: return right(state, result);
		default: printf("Operator Error!");
			return 0;
	}
}


//heuristic 1 - kiem tra so o sai khac so voi goal
int heuristic1(State state, State goal){
	int row, col, count = 0;
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++){
			if(state.eightpuzzle[row][col] != goal.eightpuzzle[row][col]){
				count++;
			}
		}
	}
	return count;
}

//heuristic 2 - Manhattan: dem so buoc de chuyen o sai ve dung vi tri
int heuristic2(State state, State goal){
	int count = 0;
	int row, col, rowg, colg;
	for(row = 0; row < ROWS; row++){
		for(col = 0; col < COLS; col++){
			if(state.eightpuzzle[row][col] != EMPTY){
				for(rowg = 0; rowg < ROWS; rowg++){
					for(colg = 0; colg < COLS; colg++){
						if(state.eightpuzzle[row][col] == goal.eightpuzzle[rowg][colg]){
							count += abs(row - rowg) + abs(col - colg);
							colg = COLS;//break loop
							rowg = ROWS;//break loop
						}
					}
				}
			}
		}
	}
	return count;
}

typedef struct Node{
	State state;
	struct Node* parent;
	int no_function;
	int f;
	int g;
	int h;
}Node;

typedef struct
{
    Node* Elements[MaxLength];
    int size;
} List;

void makeNull_List(List *list)
{
    list->size = 0;
}

int empty_List(List list)
{
    return list.size == 0;
}

int full_List(List list)
{
    return list.size == MaxLength;
}

Node *element_at(int p, List list)
{
    return list.Elements[p - 1];
}

void push_List(Node *x, int position, List *list)
{
    if (!full_List(*list))
    {
        int q;
        for (q = list->size; q >= position; q--)
            list->Elements[q] = list->Elements[q - 1];
        list->Elements[position - 1] = x;
        list->size++;
    }
    else
        printf("List is full!\n");
}

void delete_List(int position, List *list)
{
    if (empty_List(*list))
        printf("List is empty!\n");
    else if (position < 1 || position > list->size)
        printf("Position is not possible to delete!\n");
    else
    {
        int i;
        for (i = position - 1; i < list->size; i++)
            list->Elements[i] = list->Elements[i + 1];
        list->size--;
    }
}

Node* find_State(State state, List list, int *pos){
	int i;
	for(i = 1; i <= list.size; i++){
		if(compareStates(element_at(i, list)->state, state)){
			*pos = i;
			return element_at(i, list);
		}
	}
	return NULL;
}

void sort_List(List *list){
	int i, j;
	for(i = 0; i < list->size; i++){
		for(j = i+1; j < list->size; j++){
			if(list->Elements[i]->f > list->Elements[j]->f){
				Node* node = list->Elements[i];
				list->Elements[i] = list->Elements[j];
				list->Elements[j] = node;
			}
		}
	}
}

Node *A_Star(State state, State goal){
    List Open_A_Star;
    List Close_A_Star;
    makeNull_List(&Open_A_Star);
    makeNull_List(&Close_A_Star);
    Node *root = (Node *)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0;
    root->g = 0;
    root->h = heuristic2(root->state, goal);
    root->f = root->g + root->h;
    push_List(root, Open_A_Star.size + 1, &Open_A_Star);
    while (!empty_List(Open_A_Star))
    {
        Node *node = element_at(1, Open_A_Star);
        delete_List(1, &Open_A_Star);
        push_List(node, Close_A_Star.size + 1, &Close_A_Star);
        if (goalcheck(node->state, goal))
            return node;
        int opt;
        for (opt = 1; opt <= MAX_OPERATOR; opt++)
        {
            State newstate;
            if (call_operators(node->state, &newstate, opt))
            {
                Node *newNode = (Node *)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->parent = node;
                newNode->no_function = opt;
                newNode->g = node->g + 1;
                newNode->h = heuristic2(newstate, goal);
                newNode->f = newNode->g + newNode->h;
                int pos_Open, pos_Close;
                Node *nodeFoundOpen = find_State(newstate, Open_A_Star, &pos_Open);
                Node *nodeFoundClose = find_State(newstate, Close_A_Star, &pos_Close);
                if (nodeFoundOpen == NULL && nodeFoundClose == NULL)
                    push_List(newNode, Open_A_Star.size + 1, &Open_A_Star);
                else if (nodeFoundOpen != NULL && nodeFoundOpen->g > newNode->g)
                {
                    delete_List(pos_Open, &Open_A_Star);
                    push_List(newNode, pos_Open, &Open_A_Star);
                }
                else if (nodeFoundClose != NULL && nodeFoundClose->g > newNode->g)
                {
                    delete_List(pos_Close, &Close_A_Star);
                    push_List(newNode, Open_A_Star.size + 1, &Open_A_Star);
                }
                
            }
        }
        sort_List(&Open_A_Star);
    }
    return NULL;
}

void print_WaysToGetGoal(Node* node){
	List listPrint;
    makeNull_List(&listPrint);
    while (node->parent != NULL){
        push_List(node, listPrint.size + 1, &listPrint);
        node = node->parent;
    }
    push_List(node, listPrint.size + 1, &listPrint);
    int no_action = 0, i;
    for (i = listPrint.size; i > 0; i--){
        printf("\nAction %d: %s", no_action, action[element_at(i, listPrint)->no_function]);
        print_State(element_at(i, listPrint)->state);
        no_action++;
    }
}

int main(){
	State state;
	state.emptyCol = 1;
	state.emptyRow = 1;
	state.eightpuzzle[0][0] = 1;
	state.eightpuzzle[0][1] = 2;
	state.eightpuzzle[0][2] = 3;
	state.eightpuzzle[1][0] = 8;
	state.eightpuzzle[1][1] = 0;
	state.eightpuzzle[1][2] = 4;
	state.eightpuzzle[2][0] = 7;
	state.eightpuzzle[2][1] = 6;
	state.eightpuzzle[2][2] = 5;

	State goal;
	goal.emptyCol = 0;
	goal.emptyRow = 1;
	goal.eightpuzzle[0][0] = 2;
	goal.eightpuzzle[0][1] = 8;
	goal.eightpuzzle[0][2] = 1;
	goal.eightpuzzle[1][0] = 0;
	goal.eightpuzzle[1][1] = 4;
	goal.eightpuzzle[1][2] = 3;
	goal.eightpuzzle[2][0] = 7;
	goal.eightpuzzle[2][1] = 6;
	goal.eightpuzzle[2][2] = 5;
	
	Node *p = A_Star(state, goal);
	print_WaysToGetGoal(p);
	return 0;
}
