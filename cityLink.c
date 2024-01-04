/**
 * @file cityLink.c
 * @brief This program receives as input a text file which contains the adjacency
 * matrix for a specific set of cities, and the indexes of two cities interpeting
 * the source city and the destination city. It then checks if there is a connection
 * (immediate or not) between the two cities and prints the transitive closure list,
 * with the according messages for each implementation. It works based on the getopt
 * library, allowing the user to choose what implementations to ask for.
 * @author Maria Chrysanthou
 * @bug no known bugs 
 * 
*/

#include <stdio.h>
#include <getopt.h>
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>

/**
 * @brief This function serves as the entry point for the program. It uses the getopt library with
 * its optag uses to process command-line arguments provided to the program, including options 
 * such as `-i`, `-r`, `-p`, and `-o`. It also handles error cases with the appropriate usage messages.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings representing the command-line arguments.
*/
void run(int argc, char *argv[]);

/**
 * @brief Dynamically allocates memory to create a square matrix and initializes all elements to zero.
 * 
 * This function allocates memory to create a square matrix of size N x N, where N is the number
 * of cities in the adjacency matrix. Each element of the matrix is initialized to zero, 
 * whereas the matrix is represented as a two-dimensional array of integers.
 * 
 * @return A pointer to the dynamically allocated 2D integer matrix, where each element is initialized to zero.
 */
int** createMatrix();

/**
 * @brief Frees the memory allocated for a two-dimensional integer matrix.
 * @param matrix A pointer to the two-dimensional integer matrix to be deallocated.
 * 
*/
void freeMatrix(int **matrix);

/**
 * @brief This function reads the adjacency matrix data from the provided input file and initializes
 * the city matrix used for graph representation. It also performs error checking for file reading.
 *
 * @param inputFile A pointer to the input file from which the adjacency matrix data is read.
 * 
*/
void readAdjacencyMatrix(FILE *inputFile);

/**
 * @brief Recursive function to find a path from a given source city to a 
 * given destination city and print the path if it exists.
 * 
 * @param source The source city given by the user.
 * @param destination The destination city given by the user.
 * @param visited An array representing the visited cities.
 * @param path An array representing the current path needed to be printed.
 * @param pathIndex The index of the current path.
 * @return 1 if a path is found, 0 if no path exists.
 */
int findPath( int source, int destination, int *visited, int *path, int pathIndex);

/**
 * @brief This function calculates the transitive closure of a directed graph represented
 * by the city matrix and prints the transitive closure either to an output file or with 
 * standard output, based on whichever function calls it (either -o or -p).
 *
 * @param cityMatrix A 2D integer array representing the adjacency matrix of the graph.
 * @param outputFile A pointer to the output file where the transitive closure is printed (use NULL for no file output).
 * @param printToFile An integer flag (0 or 1) indicating whether to print the closure to a file (1) or standard output (0).
*/
void calculateTransitiveClosure(int **cityMatrix, FILE *outputFile, int printToFile);

/**
 * @brief Implements the "-i" option given by the user by opening the input file and reading 
 * the adjacency matrix, then printing the adjacency matrix to the console.
 * 
 * @param filename A pointer to the filename string.
 */
void implementI (char **filename);

/**
 * @brief Implements the "-r" option by finding a path between two cities and 
 * printing the path if it exists. It uses recursion by calling the findPath 
 * function which calls itself whenever needed to calculate the path.
 * 
 * @param filename A pointer to the filename string.
*/
void implementR (char **filename);

/**
 * @brief Implements the "-p" option by calculating the transitive closure of 
 * the cityMatrix and printing it to the console, using the calculateTransitiveClosure
 * function.
 * @param filename A pointer to the filename string.
*/
void implementP (char **filename);

/**
 * @brief Implements the "-o" option by creating an output file, calculating the 
 * transitive closure, and then printing it to the file.
 * @param filename A pointer to the filename string.
*/
void implementO (char **filename);

int N; // The number of cities
int **cityMatrix; // The adjacency matrix

int main (int argc, char *argv[]){

    run(argc, argv);
    return 0;
}

 void run (int argc, char *argv[]){

    int option;
    char *filename = NULL;

    if (argc == 1) {
        fprintf(stderr, "No command line arguments given!\n");
        fprintf(stderr, "Usage: %s -i <filename> -r <source_city>,<destination_city> -p -o <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    while ((option = getopt(argc, argv, "i:r:po")) != -1) {
        switch (option) {
            case 'i':
                implementI(&filename);
                break;
            case 'r':
                implementR(&filename);
                break;
            case 'p':
                implementP(&filename);
                break;
            case 'o':
                implementO(&filename);
                break;
            case '?':
                fprintf(stderr, "Usage: %s -i <inputfile> [-r <source>,<destination> -p -o]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }


    // Check if the -i option was provided
    if (filename == NULL) {
        fprintf(stderr, "No input file given!\n");
        fprintf(stderr, "Usage: %s -i <filename> [-r <source_city>,<destination_city> -p -o <output_file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

// Function to allocate memory for a matrix and initialize it to zeros
int** createMatrix() {
    int i;
    int **matrix = (int **)malloc(N * sizeof(int *));
    for (i = 0; i < N; i++) {
        matrix[i] = (int *)calloc(N, sizeof(int));
    }
    return matrix;
}

// Function that frees a matrix
void freeMatrix(int **matrix) {
    int i;
    for (i = 0; i < N; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void readAdjacencyMatrix(FILE *inputFile) {
    if (fscanf(inputFile, "%d", &N) != 1) {
        fprintf(stderr, "Error: Failed to read the number of cities from the input file.\n");
        exit(EXIT_FAILURE);
    }

    // Create a dynamic 2D array to store the adjacency matrix
    cityMatrix = createMatrix();

    int i,j;
    // Read the adjacency matrix from the input file
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (fscanf(inputFile, "%d", &cityMatrix[i][j]) != 1) {
                fprintf(stderr, "Error: Failed to read the adjacency matrix from the input file.\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int findPath( int source, int destination, int *visited, int *path, int pathIndex) {
    // Mark the current city as visited
    visited[source] = 1;
    path[pathIndex] = source;
    pathIndex++;
    int i;

    // If the destination city is reached, print the path
    if (source == destination) {
        printf("Yes Path Exists!\n");
        for (i = 0; i < pathIndex; i++) {
            printf("%d", path[i]);
            if (i < pathIndex - 1) {
                printf("=>");
            }
        }
        printf("\n");
        return 1;
    }

     // Recur for all adjacent unvisited cities
     for (i = 0; i < N; i++) {
        if (!visited[i] && (cityMatrix[source][i])){
            if(findPath(i, destination, visited, path, pathIndex))
                return 1;
            
        }
    }

    // Backtrack: Unmark the current city as visited
    visited[source] = 0;
    return 0;
}


void implementI (char **filename) {
    *filename = optarg;
    
     // Open the input file for reading
    FILE *inputFile = fopen(*filename, "r");
    
    if (inputFile == NULL) {
        fprintf(stderr, "Error: Unable to open the input file for reading.\n");
        exit(EXIT_FAILURE);
    }

    readAdjacencyMatrix(inputFile);
    
    // Close the input file
    fclose(inputFile);

    int i,j;
    // Print the adjacency matrix
    printf("Neighbor table\n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d ", cityMatrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Free the dynamically allocated memory for the adjacency matrix
    freeMatrix(cityMatrix);
}


void implementR (char **filename) {
    int sourceCity = -1, destinationCity = -1;

    // sscanf(optarg, "%d,%d", &sourceCity, &destinationCity);
    if (sscanf(optarg, "%d,%d", &sourceCity, &destinationCity) != 2) {
        fprintf(stderr, "Invalid source and destination cities: %s\n", optarg);
        exit(EXIT_FAILURE);
    }

    // Open the input file for reading
    FILE *inputFile = fopen(*filename, "r");
    readAdjacencyMatrix(inputFile);

    int *visited = (int *)calloc(N, sizeof(int));
    int *path = (int *)malloc(N * sizeof(int));
    int pathIndex = 0;

    if (!findPath(sourceCity, destinationCity, visited, path, pathIndex)) 
        printf("No Path Exists!\n");

    free(visited);
    free(path);
}

void implementP (char **filename) {

    // Open the input file for reading
    FILE *inputFile = fopen(*filename, "r");
    readAdjacencyMatrix(inputFile);

    // Calculate the transitive closure
    printf("R* table\n");
    calculateTransitiveClosure(cityMatrix, NULL, 0);
}

void implementO (char **filename){

    char *outputfile = (char *)malloc(strlen(*filename) + 5); // 5 = length of "out-" plus null terminator
    strcpy(outputfile, "out-");
    strcat(outputfile, *filename);
   
    FILE *file = fopen(outputfile, "w+");

    if (file == NULL){
        fprintf(stderr, "Error opening the output file \n");
        exit(EXIT_FAILURE);
    }

    // Open the input file for reading
    FILE *inputFile = fopen(*filename, "r");
    readAdjacencyMatrix(inputFile);

    fprintf(file, "R* table\n");
    calculateTransitiveClosure(cityMatrix, file, 1);

    fclose(file);
    printf("Saving %s...\n", outputfile);
}


// Function to calculate the transitive closure
void calculateTransitiveClosure(int **cityMatrix, FILE *outputFile, int printToFile) {

     // Create a new matrix for the transitive closure (initialize it as a copy of the adjacency matrix)
    int** transitiveClosure = createMatrix();
   
    int i,j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            transitiveClosure[i][j] = cityMatrix[i][j];
        }
    }

    int u,w,v;
    // Print the transitive closure after initialization
    for (u = 0; u < N; u++) {
        for (w = 0; w < N; w++) {
            if (transitiveClosure[u][w] == 1) {
                if (printToFile)
                    fprintf(outputFile, "%d -> %d\n", u, w);
                else
                    printf("%d -> %d\n", u, w);
            }
        }
    }

    int **previous = createMatrix();

    int repeat = 1; // A flag to check for changes
    while (repeat) {
        repeat = 0; // Reset the flag

        // Copy the current transitive closure into previous
        for (u = 0; u < N; u++) {
            for (w = 0; w < N; w++) {
                previous[u][w] = transitiveClosure[u][w];
            }
        }

        for (u = 0; u < N; u++) {
            for (v = 0; v < N; v++) {
                if (previous[u][v]) {
                    for (w = 0; w < N; w++) {
                        if (cityMatrix[v][w] && !transitiveClosure[u][w] && u != w) {
                            transitiveClosure[u][w] = 1;
                            repeat = 1; // Set the flag to indicate a change

                            if (printToFile)
                                // Print the newly added connection
                                fprintf(outputFile, "%d -> %d\n", u, w);
                            else
                                printf("%d -> %d\n", u, w);

                        }
                    }
                }
            }
        }
    }

    // Free dynamically allocated memory
    freeMatrix(transitiveClosure);
    freeMatrix(previous);
}
