# girvan-newman-algorithm
girvan newman in C


Social network structures are graphs that connect individuals through friendship relationships. This project aims to identify groups of individuals with strong connections in a social network graph. Within the scope of the project, it is necessary to identify communities and the nodes that constitute these communities in a given social network graph. The goal is to identify the strongest connections and separate the communities based on the given graph.


The solution to the problem is implemented using Breadth-First Search (BFS) method in the C environment. The BFS algorithm is preferred for calculating the shortest path between two nodes due to its capability of performing a wide search.

The first step in the solution process is to find the shortest paths between nodes using the BFS algorithm on the graph matrix read from the input file. During this stage, a special value is assigned to the edges when the shortest distances are found. As the graph is traversed using the BFS method, the value of the edges passed through is incremented each time to determine the shortest paths. With this method, the strongest connections within the graph are identified.

Subsequently, the found strongest connection (for example: edge 2-3) is removed, resulting in a situation where these strong connections are disconnected. Then, a traversal operation is performed on the updated matrix using the BFS method for community detection. Thus, community detection (connected-component) is achieved.

The value T obtained from the user represents the number of components obtained after strong connections are disconnected. It is sufficient for at least one of the obtained components to meet the value of T after the connections are disconnected, and the program terminates in this case. With this method, the identification of communities by removing strong connections in the social network structure is successfully achieved.
