#include <stdio.h>
#include <stdlib.h>

void display_graph(int** graph, int n) {
/*
    @brief: display the graph
    @param: graph: the graph to be displayed
            n: the size of the graph
    @return: void
*/
    int i, j;
    printf("Graph:\n");

    // Print the column indexs
    printf("    ");
    for (i = 0; i < n; i++) {
        printf("%2d ", i);
    }
    printf("\n");

    // Print the horizontal line
    printf("    ");
    for (i = 0; i < n; i++) {
        printf("-- ", i);
    }
    printf("\n");

    // Print the graph
    for (i = 0; i < n; i++) {
        printf("%2d: ", i);
        for (j = 0; j < n; j++) {
            printf("%2d ", graph[i][j]);
        }
        printf("\n");
    }
}

void display_groups(int** groups, int n, int group_count) {
/*
    @brief: display the groups
    @param: groups: the groups to be displayed
            n: the size of the graph
            group_count: the number of groups
    @return: void
*/
    int i, j;
    printf("Groups count is : %d\n", group_count);
    for (i = 0; i < group_count; i++) {
        printf("Group %d: ", i+1);
        for (j = 0; j < n; j++) {
            if (groups[i][j] == 1) {
                printf("%d, ", j);
            }
        }
        printf("\n");
    }
}

int size_array(int* array, int n) {
/*
    @brief: find the size of the array which has end part filled with -1
    @param: array: the array to be traversed
    @return: size: the size of the array
*/
    int i=0;
    while (i < n && array[i++] != -1);
    return i-1;
}

int** read_dataToGraph(char* filename, int* size) {
/*
    @brief: read the data from the file and convert it to a graph
    @param: filename: the name of the file
            size: the size of the graph
    @return: graph: the graph
*/
    int i, j;
    // open the file
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: cannot open file %s\n", filename);
        exit(1);
    }
    // create the graph
    int n, m;
    fscanf(fp, "%d %d", &n, &m);
    int** graph = (int**)malloc(n*sizeof(int*));
    for (i = 0; i < n; i++) {
        graph[i] = (int*)malloc(n*sizeof(int));
        for (j = 0; j < n; j++) {
            graph[i][j] = 0;
        }
    }
    // read the data and fill the graph
    int u, v;
    for (i = 0; i < m; i++) {
        fscanf(fp, "%d %d", &u, &v);
        graph[u][v] = 1;
        graph[v][u] = 1;
    }
    fclose(fp);
    *size = n;
    return graph;
}

int** find_groups_with_bfs(int** graph, int n, int* group_count) {
/*
    @brief: traverse the graph using bfs
    @param: graph: the graph to be traversed
            n: the size of the graph
            start: the starting node
    @return: groups: the groups of the nodes
*/
    int i, j;
    // create the groups
    int** groups = (int**)malloc(n*sizeof(int*));
    for (i = 0; i < n; i++) {
        groups[i] = (int*)malloc(n*sizeof(int));
        for (j = 0; j < n; j++) {
            groups[i][j] = 0;
        }
    }
    // create the queue
    int* queue = (int*)malloc(n*sizeof(int));
    int queue_start = 0, queue_end = 0;
    // create the visited array
    int* visited = (int*)malloc(n*sizeof(int));
    for (i = 0; i < n; i++) {
        visited[i] = 0;
    }
    // traverse the graph
    int current_node;
    for (i = 0; i < n; i++) {   // for each node
        if (visited[i] == 0) {  // if the node is not visited
            // add the node to the queue
            queue[queue_end] = i;
            queue_end++;
            // mark the node as visited
            visited[i] = 1;
            // traverse the graph
            while (queue_start != queue_end) {  // while the queue is not empty
                // get the current node
                current_node = queue[queue_start];
                queue_start++;
                // add the node to the group
                groups[*group_count][current_node] = 1;
                // traverse the neighbours
                int j;
                for ( j = 0; j < n; j++) {
                    if (graph[current_node][j] == 1 && visited[j] == 0) {
                        // add the node to the queue
                        queue[queue_end] = j;
                        queue_end++;
                        // mark the node as visited
                        visited[j] = 1;
                    }
                }
            }
            // increase the group count
            *group_count = *group_count + 1;
        }
    }
    free(queue);
    free(visited);
    return groups;
}

int find_minimum_population(int** graph, int n) {
/*
    @brief: find the minimum population
    @param: graph: the graph to be traversed
            n: the size of the graph
    @return: minimum_population: the minimum population
*/
    int i, j, paths_count, group_count, size, minimum_population;

    // find the groups
    group_count = 0;
    int** groups = find_groups_with_bfs(graph, n, &group_count);
    // find the minimum population
    minimum_population = n;
    for (i = 0; i < group_count; i++) {
        size = 0;
        for (j = 0; j < n; j++) {
            if (groups[i][j] == 1) {
                size++;
            }
        }
        if (size < minimum_population) {
            minimum_population = size;
        }
    }
    return minimum_population;
}

int** find_shortest_paths(int** graph, int n, int* paths_count, int src, int dest) {
/*
    @brief: find the shortest paths from src node to dest node
    @example:
            This function returns a 2d array of shortest paths between the src node to dest node
            for example graph is 
            6 7
            0 1
            0 3
            1 4
            3 4
            1 2
            4 5
            2 5
            searching for node 0 to 5:
            {{0,1,2,5},{0,1,4,6},{0,3,4,5}}
            or another example
            searching for node 1 to 3:
            {{1,0,3},{1,4,3}}
    @param: graph: the graph to be traversed
            n: the size of the graph
            path_counts: the number of shortest paths from src node to all other nodes
            src: the source node
    @return: shortest_paths: the shortest paths from src node to all other nodes
*/
    int i, j, k, l, size, last_node, node;
    // create a copy of graph
    int** graph_copy = (int**)malloc(n*sizeof(int*));
    for (i = 0; i < n; i++) {
        graph_copy[i] = (int*)malloc(n*sizeof(int));
        for (j = 0; j < n; j++) {
            graph_copy[i][j] = graph[i][j];
        }
    }
    // create a queue of paths
    int** queue = (int**)malloc(n*n*sizeof(int*));
    for (i = 0; i < n*n; i++) {
        queue[i] = (int*)malloc((n+1)*sizeof(int));
        for (j = 0; j < n; j++) {
            queue[i][j] = -1;
        }
    }
    int queue_end = 0;
    // insert the src node to the queue
    int* path = (int*)malloc((n+1)*sizeof(int));
    for (i = 0; i < n; i++) {
        path[i] = -1;
    }
    path[0] = src;
    queue[queue_end++] = path;
    int end = 1;
    while (end)
    {
        // check end condition
        i = 0;
        // check if there is a path ends with dest in the queue
        while (i < queue_end && end) {
            size = size_array(queue[i], n);
            j = 0;
            while (j < size && end) {
                if (queue[i][j] == dest) {
                    end = 0;
                }
                j++;
            }
            i++;
        }
        if (end) {
            // temporary variables
            int temp = queue_end;
            int** temp_m = (int**)malloc(n*sizeof(int*));
            for (i = 0; i < n; i++) {
                temp_m[i] = (int*)malloc(n*sizeof(int));
                for (j = 0; j < n; j++) {
                    temp_m[i][j] = 0;
                }
            }
            // traverse the queue
            for (i = 0; i < temp; i++) {
                // get the path from the queue[i]
                int* path = (int*)malloc((n+1)*sizeof(int));
                for (j = 0; j < n; j++) {
                    path[j] = queue[i][j];
                }
                // get the last node of the path
                size = size_array(path, n);
                last_node = path[size-1];

                // traverse the neighbours of the last node
                for (j = 0; j < n; j++) {
                    if (graph_copy[last_node][j] == 1) {
                        // create a new path
                        int* new_path = (int*)malloc(n*sizeof(int));
                        for (k = 0; k < n; k++) {
                            new_path[k] = path[k];
                        }
                        size = size_array(path, n);
                        new_path[size] = j;

                        // save for remove operation
                        last_node = new_path[size-1];
                        temp_m[last_node][j] = 1;
                        temp_m[j][last_node] = 1;

                        // add the new path to the queue
                        queue[queue_end++] = new_path;
                    }
                }
            }
            // shift the queue to the left by temp
            for (i = 0; i < queue_end-temp; i++) {
                queue[i] = queue[i+temp];
            }
            queue_end -= temp;
            // remove the edges from the graph
            for (i = 0; i < n; i++) {
                for (j = i+1; j < n; j++) {
                    if (temp_m[i][j] == 1) {
                        graph_copy[i][j] = 0;
                        graph_copy[j][i] = 0;
                    }
                }
            }
        }
    }
    // create the shortest paths array
    int** shortest_paths = (int**)malloc(queue_end*sizeof(int*));
    for (i = 0; i < queue_end; i++) {
        shortest_paths[i] = (int*)malloc(n*sizeof(int));
        for (j = 0; j < n; j++) {
            shortest_paths[i][j] = -1;
        }
    }
    // get the count of paths ends with dest in the queue
    int count = 0;
    for (i = 0; i < queue_end; i++) {
        size = size_array(queue[i], n);
        if (queue[i][size-1] == dest) {
            for (j = 0; j < size; j++) {
                shortest_paths[count][j] = queue[i][j];
            }
            count++;
        }
    }
    *paths_count = count;

    free(path);
    free(queue);
    free(graph_copy);
    return shortest_paths;
}

void solution(int** graph, int n, int K, int T) {
/*
    @brief: find the edges having the highest edge betweenness centrality and cut them until the end condition which is
            the minimum population of the groups is less than T or the repetition count is greater than K
    @param: graph: the graph to be traversed
            n: the size of the graph
    @return: void
*/
    int i, j, k, l, o, paths_count, group_count, size, f, minimum_population, minimum_population_old, repetition_count=0;
    int** groups;
    minimum_population = find_minimum_population(graph, n);
    minimum_population_old = minimum_population;
    while (minimum_population > T && repetition_count < K) {    // while the end condition is not satisfied
        // create edge betweenness centrality int matrix
        int** edge_betweenness_centrality = (int**)malloc(n*sizeof(int*));
        for (i = 0; i < n; i++) {
            edge_betweenness_centrality[i] = (int*)malloc(n*sizeof(int));
            for (j = 0; j < n; j++) {
                edge_betweenness_centrality[i][j] = 0;
            }
        }

        for (o = 0; o < n; o++) {       // for each node src
            group_count=0;
            // traverse the graph using bfs
            groups = find_groups_with_bfs(graph, n, &group_count);
            // find the group which includes o
            i = 0;
            f = 0;
            while (i < group_count && f == 0) {
                if (groups[i][o] == 1) {
                    f = 1;
                }
                i++;
            }
            int* group = (int*)malloc(n*sizeof(int));
            for (j = 0; j < n; j++) {
                group[j] = groups[i-1][j];
            }

            // update edge betweenness centrality matrix for each node
            for (i = 0; i < n; i++) {    // for each node dest
                if (i != o) {   // if o and i are not the same node
                    f = 0;
                    // check if o and i are in the same group
                    if (group[i] == 1) {
                        f = 1;
                    }
                    if (f == 1) {   // if o and i are in the same group
                        // find the shortest paths between o and i
                        int** shortest_paths = find_shortest_paths(graph, n, &paths_count, o, i); // we are sending path_counts as well
                        // print the shortest paths
                        printf("Shortest Paths from %d to %d: ", o, i);
                        for (k = 0; k < paths_count; k++) {
                            printf("[ ");
                            size = size_array(shortest_paths[k], n);
                            for (l = 0; l < size-1; l++) {
                                printf("%d, ", shortest_paths[k][l]);
                                // update the edge betweenness centrality matrix
                                // method 1
                                edge_betweenness_centrality[shortest_paths[k][l]][shortest_paths[k][l+1]] += 1; // the reason of multiplying with 1000 is to avoid the float number problem
                                // method 2: cares more about important edges
                                // edge_betweenness_centrality[shortest_paths[k][l]][shortest_paths[k][l+1]] += (int) 1000.0*(1.0/paths_count); // the reason of multiplying with 1000 is to avoid the float number problem
                            }
                            printf("%d ],", shortest_paths[k][l]);
                        }
                        printf("\n");
                    }
                    else {
                        printf("No path from %d to %d\n", o, i);
                    }
                }
            }
        }
        int max_value=0;
        // print the edge betweenness centrality matrix
        printf("Edge Betweenness Centrality Matrix:\n");
        for (i = 0; i < n; i++) {
            for (j = i+1; j < n; j++) {
                if (edge_betweenness_centrality[i][j] > 0) {
                    // method 1
                    printf("Edge(%d, %d) : %2.2f\n", i, j, edge_betweenness_centrality[i][j]/2.0);     
                    // method 2: cares more about important edges
                    // printf("Edge(%d, %d) : %2.2f\n", i, j, edge_betweenness_centrality[i][j]/2000.0);   // the reason of dividing with 2000 is to avoid the float number problem and getting over repated edges
                }
                if (edge_betweenness_centrality[i][j] > max_value) {
                    max_value = edge_betweenness_centrality[i][j];
                }
            }
        }
        // find the edges having the highest edge betweenness centrality
        printf("Edges having the highest edge(s) betweenness centrality : ");
        printf ("\n");
        for (i = 0; i < n; i++) {
            for (j = i+1; j < n; j++) {
                if (edge_betweenness_centrality[i][j] == max_value) {
                    printf("(%d, %d) ", i, j);
                    printf ("is deleted");
                    printf ("\n");
                    // cut the edges having the highest edge betweenness centrality
                    graph[i][j] = 0;
                    graph[j][i] = 0;
                }
            }
        }
        printf("\n");

        minimum_population = find_minimum_population(graph, n);
        if (minimum_population_old == minimum_population) {
            repetition_count++;
        }
        else {
            repetition_count = 0;
        }
        minimum_population_old = minimum_population;
    }
}

int main() {
    // get the input from the user
    int K, T, size, group_count = 0, shortest_paths_count=0;
    printf("Iteration limit for remaning group count (K): ");
    scanf("%d", &K);
    printf("Minimum member limit for a group (T): ");
    scanf("%d", &T);

    // read the input file for graph
    int** graph = read_dataToGraph("input.txt", &size);
    // display the graph
    display_graph(graph, size);
    // do operations
    solution(graph, size, K, T);
    // find the groups
    int** groups = find_groups_with_bfs(graph, size, &group_count);
    // display the groups
    display_groups(groups, size, group_count);
    // display the graph
    display_graph(graph, size);

    free(graph);
    return 0;
}
