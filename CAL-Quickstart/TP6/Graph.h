/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <iostream>
#include <queue>
#include <list>
#include <limits>
#include <cmath>
#include <algorithm>
#include "MutablePriorityQueue.h"


template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

template <class T>
class Vertex {
    T info;						// content of the vertex
    std::vector<Edge<T> > adj;		// outgoing edges

    double dist = 0;
    Vertex<T> *path = NULL;
    int queueIndex = 0; 		// required by MutablePriorityQueue

    bool visited = false;		// auxiliary field
    bool processing = false;	// auxiliary field

    void addEdge(Vertex<T> *dest, double w);

public:
    Vertex(T in);
    T getInfo() const;
    double getDist() const;
    Vertex *getPath() const;

    bool operator<(Vertex<T> & vertex) const; // // required by MutablePriorityQueue
    friend class Graph<T>;
    friend class MutablePriorityQueue<Vertex<T>>;
};


template <class T>
Vertex<T>::Vertex(T in): info(in) {}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
    adj.push_back(Edge<T>(d, w));
}

template <class T>
bool Vertex<T>::operator<(Vertex<T> & vertex) const {
    return this->dist < vertex.dist;
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
double Vertex<T>::getDist() const {
    return this->dist;
}

template <class T>
Vertex<T> *Vertex<T>::getPath() const {
    return this->path;
}

/********************** Edge  ****************************/

template <class T>
class Edge {
    Vertex<T> * dest;      // destination vertex
    double weight;         // edge weight
public:
    Edge(Vertex<T> *d, double w);
    friend class Graph<T>;
    friend class Vertex<T>;
};

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


/*************************** Graph  **************************/

template <class T>
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set

public:
    Vertex<T> *findVertex(const T &in) const;
    bool addVertex(const T &in);
    bool addEdge(const T &sourc, const T &dest, double w);
    int getNumVertex() const;
    std::vector<Vertex<T> *> getVertexSet() const;

    // Fp06 - single source
    void unweightedShortestPath(const T &s);    //TODO...
    void dijkstraShortestPath(const T &s);      //TODO...
    void bellmanFordShortestPath(const T &s);   //TODO...
    std::vector<T> getPath(const T &origin, const T &dest) const;   //TODO...

    // Fp06 - all pairs
    void floydWarshallShortestPath();   //TODO...
    std::vector<T> getfloydWarshallPath(const T &origin, const T &dest) const;   //TODO...

};

template <class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template <class T>
std::vector<Vertex<T> *> Graph<T>::getVertexSet() const {
    return vertexSet;
}

/*
 * Auxiliary function to find a vertex with a given content.
 */
template <class T>
Vertex<T> * Graph<T>::findVertex(const T &in) const {
    for (auto v : vertexSet)
        if (v->info == in)
            return v;
    return NULL;
}

/*
 *  Adds a vertex with a given content or info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if ( findVertex(in) != NULL)
        return false;
    vertexSet.push_back(new Vertex<T>(in));
    return true;
}

/*
 * Adds an edge to a graph (this), given the contents of the source and
 * destination vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == NULL || v2 == NULL)
        return false;
    v1->addEdge(v2,w);
    return true;
}


/**************** Single Source Shortest Path algorithms ************/

template<class T>
void Graph<T>::unweightedShortestPath(const T &orig) {
    for(auto& vert: this->vertexSet){
        vert->dist = INF;
        vert->path = NULL;
        if(vert->info == orig) vert->dist = 0;
    }

    std::queue<Vertex<T>*> queue;
    queue.push(this->findVertex(orig));
    while(!queue.empty()){
        Vertex<T>* current = queue.front();
        queue.pop();

        for(auto& edge: current->adj){
            if(edge.dest->dist == INF){
                edge.dest->dist = current->dist + 1;
                edge.dest->path = current;
                queue.push(edge.dest);
            }
        }
    }
}


template<class T>
void Graph<T>::dijkstraShortestPath(const T &origin) {
    for (auto& vert: this->vertexSet) {
        vert->dist = INF;
        vert->path = NULL;
        if (vert->info == origin) vert->dist = 0;
    }

    MutablePriorityQueue<Vertex<T>> heap;
    heap.insert(this->findVertex(origin));

    while (!heap.empty()) {
        Vertex<T>* current = heap.extractMin();

        for (auto& edge: current->adj) {
            if (edge.dest->dist == INF) {
                edge.dest->dist = current->dist + edge.weight;
                edge.dest->path = current;
                heap.insert(edge.dest);
            } else if (edge.dest->dist > current->dist + edge.weight) {
                edge.dest->dist = current->dist + edge.weight;
                edge.dest->path = current;
                heap.decreaseKey(edge.dest);
            }
        }
    }

}


template<class T>
void Graph<T>::bellmanFordShortestPath(const T &orig) {
    for (auto& vert: this->vertexSet) {
        vert->dist = INF;
        vert->path = NULL;
        if (vert->info == orig) vert->dist = 0;
    }

    MutablePriorityQueue<Vertex<T>> heap;
    heap.insert(this->findVertex(orig));

    for(int i = 1; i < this->getNumVertex(); i++){
        for(auto& vertex: this->vertexSet){
            for(auto& edge: vertex->adj){
                if(edge.dest->dist > vertex->dist + edge.weight){
                    edge.dest->dist = vertex->dist + edge.weight;
                    edge.dest->path = vertex;
                }
            }
        }
    }
    for(auto& vertex: this->vertexSet){
        for(auto& edge: vertex->adj){
            if(edge.dest->dist > vertex->dist + edge.weight){
                std::cout << "Graph has negative cycles" << std::endl;
                exit(1);
            }
        }
    }
}


template<class T>
std::vector<T> Graph<T>::getPath(const T &origin, const T &dest) const{
    std::vector<T> res;
    Vertex<T>* current = findVertex(dest);
    if(current == NULL){
        return std::vector<T>();
    }
    while(current->info != origin){
        res.push_back(current->info);
        if(current->path == NULL){
            return std::vector<T>();
        }
        current = findVertex(current->path->info);
        if(current == NULL){
            return std::vector<T>();
        }
    }
    res.push_back(origin);
    std::reverse(res.begin(), res.end());
    return res;
}



/**************** All Pairs Shortest Path  ***************/

template<class T>
void Graph<T>::floydWarshallShortestPath() {
    std::vector<std::vector<double>> distances(this->vertexSet.size(), std::vector<double>(this->vertexSet.size(), 0));
    std::vector<std::vector<T>> predecessors(this->vertexSet.size(), std::vector<T>(this->vertexSet.size(), T()));
    std::vector<std::vector<double>> weights(this->vertexSet.size(), std::vector<double>(this->vertexSet.size(), 0));

    for(auto& vertex: this->vertexSet){
        for(int i = 0; i < this->vertexSet.size(); i++) weights[vertex->info - 1][i] = INF;
        for(auto& edge: vertex->adj){
            if(vertex->info == edge.dest->info)
                weights[vertex->info - 1][edge.dest->info - 1] = 0;
            else
                weights[vertex->info - 1][edge.dest->info - 1] = edge.weight;
        }
    }

    for(int k = 0; k < this->vertexSet.size(); k++){
        for(int i = 0; i < this->vertexSet.size(); i++){
            for(int j = 0; j < this->vertexSet.size(); j++){
                if(k == 0){
                    distances[vertexSet.at(i)->info - 1][vertexSet.at(j)->info - 1] = weights[vertexSet.at(i)->info - 1][vertexSet.at(j)->info - 1];
                    predecessors[vertexSet.at(i)->info - 1][vertexSet.at(j)->info - 1] = -1;
                } else {
                    if(distances[vertexSet.at(i)->info - 1][vertexSet.at(j)->info - 1] > distances[vertexSet.at(i)->info - 1][vertexSet.at(k)->info - 1] + distances[vertexSet.at(k)->info - 1][vertexSet.at(j)->info - 1]){
                        distances[vertexSet.at(i)->info - 1][vertexSet.at(j)->info - 1] = distances[vertexSet.at(i)->info - 1][vertexSet.at(k)->info - 1] + distances[vertexSet.at(k)->info - 1][vertexSet.at(j)->info - 1];
                        predecessors[vertexSet.at(k)->info][vertexSet.at(j)->info] = i;
                    }
                }
            }
        }
    }

    for(int i = 0; i < this->vertexSet.size(); i++){
        for(int j = 0; j < this->vertexSet.size(); j++) {
                
        }
    }
}

template<class T>
std::vector<T> Graph<T>::getfloydWarshallPath(const T &orig, const T &dest) const{
    std::vector<T> res;
    // TODO implement this
    return res;
}


#endif /* GRAPH_H_ */
