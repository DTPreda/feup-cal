/*
 * Graph.h
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <map>
#include <iostream>

template <class T> class Edge;
template <class T> class Graph;
template <class T> class Vertex;


/****************** Provided structures  ********************/

template <class T>
class Vertex {
	T info;                // contents
	std::vector<Edge<T> > adj;  // list of outgoing edges
	bool visited;          // auxiliary field used by dfs and bfs
	bool processing;       // auxiliary field used by isDAG
	int indegree;          // auxiliary field used by topsort

	void addEdge(Vertex<T> *dest, double w);
	bool removeEdgeTo(Vertex<T> *d);
public:
	Vertex(T in);
	friend class Graph<T>;
};

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
class Graph {
    std::vector<Vertex<T> *> vertexSet;    // vertex set

	void dfsVisit(Vertex<T> *v,  std::vector<T> & res) const;
	Vertex<T> *findVertex(const T &in) const;
	bool dfsIsDAG(Vertex<T> *v) const;
public:
	int getNumVertex() const;
	bool addVertex(const T &in);
	bool removeVertex(const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	bool removeEdge(const T &sourc, const T &dest);
    std::vector<T> dfs() const;
    std::vector<T> bfs(const T &source) const;
    std::vector<T> topsort() const;
	int maxNewChildren(const T &source, T &inf) const;
	bool isDAG() const;
};

/****************** Provided constructors and functions ********************/

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template <class T>
Edge<T>::Edge(Vertex<T> *d, double w): dest(d), weight(w) {}


template <class T>
int Graph<T>::getNumVertex() const {
	return vertexSet.size();
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

/****************** 1a) addVertex ********************/

/*
 *  Adds a vertex with a given content/info (in) to a graph (this).
 *  Returns true if successful, and false if a vertex with that content already exists.
 */
template <class T>
bool Graph<T>::addVertex(const T &in) {
    if(this->findVertex(in) == NULL){
        auto vertex = new Vertex<T>(in);
        this->vertexSet.push_back(vertex);
        return true;
    }
    // HINT: use the findVertex function to check if a vertex already exists
    return false;
}

/****************** 1b) addEdge ********************/

/*
 * Adds an edge to a graph (this), given the contents of the source (sourc) and
 * destination (dest) vertices and the edge weight (w).
 * Returns true if successful, and false if the source or destination vertex does not exist.
 */
template <class T>
bool Graph<T>::addEdge(const T &sourc, const T &dest, double w) {
    Vertex<T>* source = this->findVertex(sourc);
    Vertex<T>* destination = this->findVertex(dest);
    if(source != NULL && destination != NULL){
        source->addEdge(destination, w);
        return true;
    }
    // HINT: use findVertex to obtain the actual vertices
    // HINT: use the next function to actually add the edge
    return false;
}

/*
 * Auxiliary function to add an outgoing edge to a vertex (this),
 * with a given destination vertex (d) and edge weight (w).
 */
template <class T>
void Vertex<T>::addEdge(Vertex<T> *d, double w) {
    this->adj.push_back(Edge<T>(d, w));
}


/****************** 1c) removeEdge ********************/

/*
 * Removes an edge from a graph (this).
 * The edge is identified by the source (sourc) and destination (dest) contents.
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Graph<T>::removeEdge(const T &sourc, const T &dest) {
    Vertex<T>* source = this->findVertex(sourc);
    Vertex<T>* destination = this->findVertex(dest);
    if(source != NULL && destination != NULL){;
        for(auto it = source->adj.begin(); it != source->adj.end(); it++) if((*it).dest == destination){
            source->adj.erase(it);
            return true;
        }
        return false;
    }
    // HINT: Use "findVertex" to obtain the actual vertices.
    // HINT: Use the next function to actually remove the edge.
    return false;
}

/*
 * Auxiliary function to remove an outgoing edge (with a given destination (d))
 * from a vertex (this).
 * Returns true if successful, and false if such edge does not exist.
 */
template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *d) {
    for(auto it = this->adj.begin(); it != this->adj.end(); it++){
        if((*it).dest == d) {
            this->adj.erase(it);
            return true;
        }
    }
    // HINT: use an iterator to scan the "adj" vector and then erase the edge.
    return false;
}


/****************** 1d) removeVertex ********************/

/*
 *  Removes a vertex with a given content (in) from a graph (this), and
 *  all outgoing and incoming edges.
 *  Returns true if successful, and false if such vertex does not exist.
 */
template <class T>
bool Graph<T>::removeVertex(const T &in) {
    // TODO (10 lines)
    Vertex<T>* vertex;
    if((vertex = this->findVertex(in)) == NULL)
        return false;
    vertex->adj.clear();
    for(auto it = this->vertexSet.begin(); it != vertexSet.end(); it++){
        (*it)->removeEdgeTo(vertex);
    }
    for (auto it = this->vertexSet.begin(); it != vertexSet.end(); it++){
        if((*it)->info == vertex->info){
            vertexSet.erase(it);
            break;
        }
    }
    // HINT: use an iterator to scan the "vertexSet" vector and then erase the vertex.
    // HINT: take advantage of "removeEdgeTo" to remove incoming edges.
    return true;
}


/****************** 2a) dfs ********************/

/*
 * Performs a depth-first search (dfs) in a graph (this).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
std::vector<T> Graph<T>::dfs() const {
    // TODO (7 lines)
    std::vector<T> res;
    for(int i = 0; i < this->vertexSet.size(); i++){
        if(!this->vertexSet.at(i)->visited){
            this->dfsVisit(vertexSet.at(i), res);
        }
    }
    return res;
}

/*
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Updates a parameter with the list of visited node contents.
 */
template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v, std::vector<T> & res) const {
    // TODO (7 lines)
    res.push_back(v->info);
    v->visited = true;
    for (int i = 0; i < v->adj.size(); ++i) {
        if(!v->adj.at(i).dest->visited) this->dfsVisit(v->adj.at(i).dest, res);
    }
}

/****************** 2b) bfs ********************/

/*
 * Performs a breadth-first search (bfs) in a graph (this), starting
 * from the vertex with the given source contents (source).
 * Returns a vector with the contents of the vertices by dfs order.
 * Follows the algorithm described in theoretical classes.
 */
template <class T>
std::vector<T> Graph<T>::bfs(const T & source) const {
    // TODO (22 lines)
    // HINT: Use the flag "visited" to mark newly discovered vertices .
    // HINT: Use the "queue<>" class to temporarily store the vertices.
    std::vector<T> res;
    Vertex<T>* src = this->findVertex(source);
    if(src == NULL) return res;
    std::queue<Vertex<T>> foundVertexes;
    foundVertexes.push(*src);
    src->visited = true;
    while(!foundVertexes.empty()){
        res.push_back(foundVertexes.front().info);
        for(int i = 0; i < foundVertexes.front().adj.size(); i++){
            if(!foundVertexes.front().adj.at(i).dest->visited){
                foundVertexes.front().adj.at(i).dest->visited = true;
                foundVertexes.push(*(foundVertexes.front().adj.at(i).dest));
            }
        }
        foundVertexes.pop();
    }
    return res;
}

/****************** 2c) toposort ********************/

/*
 * Performs a topological sorting of the vertices of a graph (this).
 * Returns a vector with the contents of the vertices by topological order.
 * If the graph has cycles, returns an empty vector.
 * Follows the algorithm described in theoretical classes.
 */

template<class T>
std::vector<T> Graph<T>::topsort() const {
    // TODO (26 lines)
    std::vector<T> res;
    for(int i = 0; i < this->vertexSet.size(); i++) this->vertexSet[i]->indegree = 0;
    for(int i = 0; i < this->vertexSet.size(); i++) {
        for(int j = 0; j < this->vertexSet[i]->adj.size(); j++){
            this->vertexSet[i]->adj[j].dest->indegree += 1;
        }
    }
    std::queue<Vertex<T>> c;
    for(int i = 0; i < this->vertexSet.size(); i++){
        if(this->vertexSet[i]->indegree == 0) c.push(*(this->vertexSet[i]));
    }
    std::vector<Vertex<T>> t;
    while(!c.empty()){
        t.push_back(c.front());
        for(int i = 0; i < c.front().adj.size(); i++){
            c.front().adj[i].dest->indegree -= 1;
            if(c.front().adj[i].dest->indegree == 0) c.push(*(c.front().adj[i].dest));
        }
        c.pop();
    }
    if(t.size() != this->vertexSet.size()) return res;
    for (int i = 0; i < t.size(); ++i) {
        res.push_back(t[i].info);
    }
    return res;
}

/****************** 3a) maxNewChildren (HOME WORK)  ********************/

/*
 * Performs a breadth-first search in a graph (this), starting
 * from the vertex with the given source contents (source).
 * During the search, determines the vertex that has a maximum number
 * of new children (adjacent not previously visited), and returns the
 * contents of that vertex (inf) and the number of new children (return value).
 */

template <class T>
int Graph<T>::maxNewChildren(const T & source, T &inf) const {
    // TODO (28 lines, mostly reused)
    Vertex<T>* src = this->findVertex(source);
    if(src == NULL) return 0;

    int ret = INT32_MIN, current;
    std::queue<Vertex<T>> foundVertexes;
    foundVertexes.push(*src);
    while(!foundVertexes.empty()){
        current = 0;
        for(int i = 0; i < foundVertexes.front().adj.size(); i++){
            if(!foundVertexes.front().adj.at(i).dest->visited){
                foundVertexes.front().adj.at(i).dest->visited = true;
                foundVertexes.push(*(foundVertexes.front().adj.at(i).dest));
                current += 1;
            }
        }
        if (current > ret){
            ret = current;
            inf = foundVertexes.front().info;
        }
        foundVertexes.pop();
    }
    return ret;
}

/****************** 3b) isDAG   (HOME WORK)  ********************/

/*
 * Performs a depth-first search in a graph (this), to determine if the graph
 * is acyclic (acyclic directed graph or DAG).
 * During the search, a cycle is found if an edge connects to a vertex
 * that is being processed in the the stack of recursive calls (see theoretical classes).
 * Returns true if the graph is acyclic, and false otherwise.
 */

template <class T>
bool Graph<T>::isDAG() const {
    // TODO (9 lines, mostly reused)
    for(int i = 0; i < this->vertexSet.size(); i++){
        if(dfsIsDAG(this->vertexSet.at(i))){
            return false;
        }
    }
    return true;
}

/**
 * Auxiliary function that visits a vertex (v) and its adjacent not yet visited, recursively.
 * Returns false (not acyclic) if an edge to a vertex in the stack is found.
 */
template <class T>
bool Graph<T>::dfsIsDAG(Vertex<T> *v) const {
    // TODO (12 lines, mostly reused)
    v->processing = true;
    for (int i = 0; i < v->adj.size(); ++i) {
        if(v->adj.at(i).dest->processing) return false;
        if(!v->adj.at(i).dest->visited) {
            v->adj.at(i).dest->visited = true;
            this->dfsIsDAG(v->adj.at(i).dest);
        }
    }
    v->processing = false;
    return true;
}

#endif /* GRAPH_H_ */
