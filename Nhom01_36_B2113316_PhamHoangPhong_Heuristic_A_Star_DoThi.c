#include <stdio.h>
#include <stdlib.h>

#define MaxLength 500
#define MAX_VERTICES 5

typedef struct{
	int neighbor[MAX_VERTICES];
	int h;
}Vertices;

typedef struct{
	int num_vertices;
	Vertices v[MAX_VERTICES];
}Graph;

void initGraph(int num_vertices, Graph *G){
	G->num_vertices = num_vertices;
	int i, j;
	for(i = 0; i < num_vertices; i++){
		for(j = 0; j < num_vertices; j++){
			G->v[i].neighbor[j] = 0;
			G->v[i].h = 0;
		}
	}
}

const char name[] = {'A', 'B', 'C', 'D', 'G'};

typedef struct{
	int vertex;
}State;

void printState(State state){
	printf("%c", name[state.vertex]);
}

int compareStates(State a, State b){
	return a.vertex == b.vertex;
}

typedef struct Node{
	State state;
	struct Node *parent;
	int f;
	int g;
	int h;
}Node;

//List
typedef struct{
    Node* Elements[MaxLength];
    int size;
} List;

void makeNull_List(List *list){
    list->size = 0;
}

int empty_List(List list){
    return list.size == 0;
}

int full_List(List list){
    return list.size == MaxLength;
}

Node *element_at(int p, List list){
    return list.Elements[p-1];
}

void push_List(Node *x, int position, List *list){
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

void delete_List(int position, List *list){
    if (empty_List(*list))
        printf("List is empty!\n");
    else if (position < 1 || position > list->size)
        printf("Position is not possible to delete!\n");
    else{
        int i;
        for (i = position - 1; i < list->size; i++)
            list->Elements[i] = list->Elements[i + 1];
        list->size--;
    }
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

int goalcheck(State state, State goal){
	return compareStates(state, goal);
}

Node* A_Star(Graph G, State state, State goal){
    List Open_A_Star;
    List Close_A_Star;
    makeNull_List(&Open_A_Star);
    makeNull_List(&Close_A_Star);
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->g = 0;
    root->h = G.v[state.vertex].h; // lay h cua dinh state.vertex
    root->f = root->g + root->h;
    push_List(root, Open_A_Star.size + 1, &Open_A_Star);
    while (!empty_List(Open_A_Star)){
        Node* node = element_at(1, Open_A_Star);
        delete_List(1, &Open_A_Star);
        push_List(node, Close_A_Star.size + 1, &Close_A_Star);
        if (goalcheck(node->state, goal))
            return node;
        int opt;
        for (opt = 0; opt < G.num_vertices; opt++){
            State newstate;
            if (G.v[node->state.vertex].neighbor[opt] > 0){
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state.vertex = opt;
				newNode->parent = node;
                newNode->g = node->g + G.v[node->state.vertex].neighbor[opt];
                newNode->h = G.v[opt].h;
                newNode->f = newNode->g + newNode->h;
                int pos_Open, pos_Close;
                Node *nodeFoundOpen = find_State(newNode->state, Open_A_Star, &pos_Open);
                Node *nodeFoundClose = find_State(newNode->state, Close_A_Star, &pos_Close);
                if (nodeFoundOpen == NULL && nodeFoundClose == NULL)
                    push_List(newNode, Open_A_Star.size+1, &Open_A_Star);
                else if (nodeFoundOpen != NULL && nodeFoundOpen->g > newNode->g){
                    delete_List(pos_Open, &Open_A_Star);
                    push_List(newNode, pos_Open, &Open_A_Star);
                }
                else if (nodeFoundClose != NULL && nodeFoundClose->g > newNode->g){
                    delete_List(pos_Close, &Close_A_Star);
                    push_List(newNode, Open_A_Star.size+1, &Open_A_Star);
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
        printState(element_at(i, listPrint)->state);
        if(i != 1){
        	printf("->");
		}
        no_action++;
    }
}

int main(){
	int i, j;
	Graph graph;
	freopen("test.txt", "r", stdin);
	initGraph(MAX_VERTICES, &graph);
	for(i = 0; i < MAX_VERTICES; i++){
		int x;
		scanf("%d", &x);
		graph.v[i].h = x;
		for(j = 0; j < MAX_VERTICES; j++){
			scanf("%d", &x);
			graph.v[i].neighbor[j] = x;
		}
	}
	State A, G;
	A.vertex = 0;
	G.vertex = 4;
	Node* result = A_Star(graph, A, G);
	print_WaysToGetGoal(result);
	return 0;
}
