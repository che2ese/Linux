#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 100
#define MAX_MAZE_SIZE 100

typedef struct {
    int x;
    int y;
} Position;

typedef struct {
    Position queue[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

int maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
int visited[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
Position parent[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
int maze_size;
Position start, end;

// Queue functions
void init_queue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

int is_empty(Queue* q) {
    return q->front == q->rear;
}

void enqueue(Queue* q, Position pos) {
    if (q->rear == MAX_QUEUE_SIZE - 1) {
        printf("Queue is full\n");
        exit(1);
    }
    q->queue[++q->rear] = pos;
}

Position dequeue(Queue* q) {
    if (is_empty(q)) {
        printf("Queue is empty\n");
        exit(1);
    }
    return q->queue[++q->front];
}

// Direction vectors for moving up, down, left, right
int dx[] = { -1, 1, 0, 0 };
int dy[] = { 0, 0, -1, 1 };

// Function to check if a position is within the maze and not a wall
int is_valid(Position pos) {
    return pos.x >= 0 && pos.x < maze_size && pos.y >= 0 && pos.y < maze_size && maze[pos.x][pos.y] == 0;
}

// Function to print the current state of the maze
void print_current_maze(Position current) {
    printf("Current position: (%d, %d)\n", current.x, current.y);
    for (int i = 0; i < maze_size; i++) {
        for (int j = 0; j < maze_size; j++) {
            if (i == current.x && j == current.y) {
                printf("C ");
            } else if (maze[i][j] == 1) {
                printf("# ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Level order traversal to find the path in the maze
int level_order() {
    Queue q;
    init_queue(&q);
    enqueue(&q, start);
    visited[start.x][start.y] = 1;

    printf("Root node: (%d, %d)\n", start.x, start.y);
    while (!is_empty(&q)) {
        Position current = dequeue(&q);

        // If we reach the end, return success
        if (current.x == end.x && current.y == end.y) {
            return 1;
        }

        // Explore neighbors
        for (int i = 0; i < 4; i++) {
            Position next = { current.x + dx[i], current.y + dy[i] };
            if (is_valid(next) && !visited[next.x][next.y]) {
                enqueue(&q, next);
                visited[next.x][next.y] = 1;
                parent[next.x][next.y] = current;
                printf("Adding node: (%d, %d) as a child of node: (%d, %d)\n", next.x, next.y, current.x, current.y);
                print_current_maze(next);
            }
        }
    }
    return 0;
}

int main() {
    // Example maze input
    maze_size = 5;
    int maze_input[MAX_MAZE_SIZE][MAX_MAZE_SIZE] = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {1, 1, 0, 0, 0}
    };

    start = (Position){ 0, 0 };
    end = (Position){ 4, 4 };

    // Initialize the maze and visited arrays
    for (int i = 0; i < maze_size; i++) {
        for (int j = 0; j < maze_size; j++) {
            maze[i][j] = maze_input[i][j];
            visited[i][j] = 0;
            parent[i][j] = (Position){ -1, -1 }; // Initialize parent array
        }
    }

    // Find the path using level order traversal
    if (level_order()) {
        // Path found, final position will be printed last
    }
    else {
        printf("NO path found\n");
    }

    return 0;
}
