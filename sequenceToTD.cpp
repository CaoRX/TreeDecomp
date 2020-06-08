#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

// Input: a graph, a sequence
// Output: a tree decomposition

class Edge {
public:
    int s, t, next;
    Edge(int _s, int _t, int _next): s(_s), t(_t), next(_next) {
    }
};

class UndirectedGraph {
public:
    vector<Edge> e;
    int edgeN;
    vector<int> h;
    int n;

    vector<int> f;

    void unionSetInit() {
        f = vector<int>(n);
        for (int i = 0; i < n; ++i) {
            f[i] = i;
        }
    }

    int gf(int x) {
        if (f[x] == x) {
            return x;
        }
        return (f[x] = gf(f[x]));
    }

    void linkAToB(int a, int b) {
        f[gf(a)] = b;
    }

    void setRoot(int u) {
        int rt = gf(u);
        f[u] = u;
        f[rt] = u;
    }

    UndirectedGraph(int _n): n(_n) {
        h = vector<int>(n, -1);
        edgeN = 0;
        e.clear();
        unionSetInit();
    }

    void addEdge(int u, int v) {
        e.push_back(Edge(u, v, h[u]));
        h[u] = edgeN++;
        e.push_back(Edge(v, u, h[v]));
        h[v] = edgeN++;
    }
    void addEdges(vector<vector<int> > &edges) {
        for (int i = 0; i < edges.size(); ++i) {
            addEdge(edges[i][0], edges[i][1]);
        }
        // for (auto edge: edges) {
        //     addEdge(edge[0], edge[1]);
        // }
    }
};

class TDNode {
public:
    set<int> active, unactive;
    vector<TDNode *> childs;
    TDNode *parent;

    TDNode() {
        active.clear(); unactive.clear();
        parent = NULL;
        childs.clear();
    }
    void push(int x) {
        active.insert(x);
    }
    void makeUnactive(int x) {
        set<int>::iterator actLoc = active.find(x);
        if (actLoc == active.end()) {
            cerr << "Error: node " << x << " to be made unactive not found!" << endl;
            return ;
        }
        active.erase(actLoc);
        unactive.insert(x);
    }

    void pushNode(TDNode *child) {
        active.insert(child->active.begin(), child->active.end());
        child->parent = this;
        childs.push_back(child);
    }
    int size() {
        return active.size() + unactive.size();
    }

    void print() {
        vector<int> res;
        res.clear();
        for (set<int>::iterator it = active.begin(); it != active.end(); ++it) {
            res.push_back(*it);
        }
        for (set<int>::iterator it = unactive.begin(); it != unactive.end(); ++it) {
            res.push_back(*it);
        }
        sort(res.begin(), res.end());
        for (int i = 0; i < res.size(); ++i) {
            cout << res[i] << ' ';
        }
        cout << endl;
    }
};

class TreeDecomposition {
public:
    vector<TDNode *> vNodes;
    vector<TDNode *> tNodes;
    int width;
    int n;

    TreeDecomposition(UndirectedGraph *g) {
        n = g->n;
        vNodes = vector<TDNode *>(n);
        for (int i = 0; i < n; ++i) {
            vNodes[i] = new TDNode();
            vNodes[i]->push(i);
        }

        tNodes = vector<TDNode *>(n, NULL);
        
        width = 0;
    }

    TDNode *getNode(int u, bool askVNode) {
        if (askVNode) {
            return vNodes[u];
        } else {
            return tNodes[u];
        }
    }
    void print() {
        cout << "width = " << width << endl;
    }
};

TreeDecomposition *getTDFromSequence(UndirectedGraph *g, vector<int> &seq) {
    TreeDecomposition *td = new TreeDecomposition(g);

    for (int i = 0; i < seq.size(); ++i) {
        int u = seq[i];
        cout << "consider " << u << endl;
        int rt = g->gf(u);
        bool singleRoot = true;

        for (int p = g->h[u]; p != -1; p = g->e[p].next) {
            int v = g->e[p].t;
            if (g->gf(v) != rt) {
                singleRoot = false;
                break;
            }
        }

        // cout << "singleRoot of " << u << " = " << singleRoot << endl;

        if (singleRoot) {
            if (rt == u) {
                td->vNodes[u]->makeUnactive(u);
            } else {
                td->tNodes[rt]->makeUnactive(u);
            }
            continue;
        }

        g->setRoot(u);
        td->tNodes[u] = new TDNode();
        td->tNodes[u]->pushNode(td->getNode(rt, rt == u));

        // cout << "finish copy node, now link nodes:" << endl;

        for (int p = g->h[u]; p != -1; p = g->e[p].next) {
            int v = g->e[p].t;
            int vF = g->gf(v);
            // cout << "v = " << v << ", vF = " << vF << endl;
            if (vF != u) {
                g->linkAToB(vF, u);
                td->tNodes[u]->pushNode(td->getNode(vF, vF == v));
            }
        }
        td->tNodes[u]->makeUnactive(u);
        td->width = max(td->width, td->tNodes[u]->size() - 1);
        cout << "build node ";
        td->tNodes[u]->print();
    }
    return td;
}

UndirectedGraph *getGraph() {
    int n, m, u, v;
    cin >> n;
    UndirectedGraph *g = new UndirectedGraph(n);
    cin >> m; 
    for (int i = 0; i < m; ++i) {
        cin >> u >> v;
        g->addEdge(u, v);
    }
    return g;
}

vector<int> getSequence() {
    int n;
    vector<int> res;
    cin >> n;
    res = vector<int>(n, 0);
    for (int i = 0; i < n; ++i) {
        cin >> res[i];
    }
    return res;
}

int main() {
    UndirectedGraph *g = getGraph(); 
    vector<int> seq = getSequence();
    TreeDecomposition *td = getTDFromSequence(g, seq);
    td->print();
    return 0;
}