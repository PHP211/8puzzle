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
	if(empRowCur > 0){
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
	if(empColCur > 0){
		result->emptyCol = empColCur + 1;
		result->emptyRow = empRowCur;
		result->eightpuzzle[empRowCur][empColCur] = state.eightpuzzle[empRowCur][empColCur + 1];
		result->eightpuzzle[empRowCur][empColCur + 1] = EMPTY;
		return 1;
	}
	return 0;
}

int calloperators(State state, State *result, int option){
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
	int heuristic;
}Node;

typedef struct{
	Node* Elements[MaxLength];
	int size;
}List;

void makeNull_List(List *list){
	list->size = 0;
}

int empty_List(List list){
	return list.size == 0;
}

int full_List(List list){
	return list.size == MaxLength;
}

Node* element_At(int p, List list){
	return list.Elements[p-1];
}

void push(Node* x, int p, List *list){
	if(!full_List(*list)){
		int q;
		for(q = list->size; q >= p; q--){
			list->Elements[q] = list->Elements[q-1];
		}
		list->Elements[p-1] = x;
		list->size++;
	}
	else printf("List is Full!\n");
}

void deletelist(int pos, List *list){
	if(empty_List(*list)){
		printf("List is empty!\n");
	}else if(pos < 1 || pos > list->size){
		printf("Position is not possible to delete!\n");
	}else{
		int i;
		for(i = pos-1; i < list->size; i++){
			list->Elements[i] = list->Elements[i+1];
		}
		list->size--;
	}
}

Node* find_State(State state, List list, int *pos){
	int i;
	for(i = 1; i <= list.size; i++){
		if(compareStates(element_At(i, list)->state, state)){
			*position = i;
			return element_At(i, list);
		}
	}
	return NULL;
}

void sort_List(List *list){
	int i, j;
	for(i = 0; i < list->size; i++){
		for(j = i+1; j < list->size; j++){
			if(list->Elements[i]->heuristic > list->Elements[j]->heuristic){
				Node* node = list->Elements[i];
				list->Elements[i] = list->Elements[j];
				list->Elements[j] = node;
			}
		}
	}
}

Node* BFS(State state, State goal){
	List open, close;
	makeNull_List(&open);
	makeNull_List(&close);
	Node* root = (Node*)malloc(sizeof(Node));
	root->state = state;
	root->parent = NULL;
	root->no_function = 0;
	root->heuristic = heuristic1(root->state, goal);
	push(root, open.size+1, &open);
	while(!empty_List(open)){
		Node* node = element_At(1, open);
		deletelist(1, &open);
		push(node, close.size+1, &close);
		if(goalcheck(node->state, goal)){
			return node;
		}
		int opt;
		for(opt = 1; opt <= 4; opt++){
			State newstate;
			newstate = node->state;
			if(calloperators(node->state, &newstate, opt)){
				Node* newnode = (Node*)malloc(sizeof(Node));
				newnode->state = newstate;
				newnode->no_function = opt;
				newnode->parent = node;
				newnode->heuristic = heuristic1(newstate, goal);
			}
		}
	}
}

int main(){
	State state, result;
	state.emptyCol = 1;
	state.emptyRow = 1;
	state.eightpuzzle[0][0] = 3;
	state.eightpuzzle[0][1] = 4;
	state.eightpuzzle[0][2] = 5;
	state.eightpuzzle[1][0] = 1;
	state.eightpuzzle[1][1] = 0;
	state.eightpuzzle[1][2] = 2;
	state.eightpuzzle[2][0] = 6;
	state.eightpuzzle[2][1] = 7;
	state.eightpuzzle[2][2] = 8;
	printf("Trang thai bat dau:\n");
	print_State(state);
	int opt;
	for(opt = 1; opt <= 4; opt++){
		calloperators(state, &result, opt);
		if(!compareStates(result, state)){
			printf("\nHanh dong %s thanh cong\n", action[opt]);
			print_State(result);
		} else {
			printf("\nHanh dong %s khong thanh cong\n", action[opt]);
		}
	}
	return 0;
}
