#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

#include "udg.hpp"
#include "TD.hpp"

using namespace std;

// Input: a graph, a sequence
// Output: a tree decomposition

TreeDecomposition *getTDFromSequence(UndirectedGraph *g, vector<int> &seq, bool onlyWidth = false) {
    TreeDecomposition *td = new TreeDecomposition(g, onlyWidth);

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
            td->getNode(rt, rt == u)->makeUnactive(u);
            // if (rt == u) {
            //     td->vNodes[u]->makeUnactive(u);
            // } else {
            //     td->tNodes[rt]->makeUnactive(u);
            // }
            continue;
        }

        g->setRoot(u);
        td->tNodes[u] = new TDNode(onlyWidth);
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

vector<int> TDToSequence(TreeDecomposition *td) {
    if (td->onlyWidth) {
        cerr << "Error: cannot generate sequence from onlyWidth tree decomposition. Exit." << endl;
        return vector<int>();
    }
    vector<int> res;
    res.clear();
    for (int i = 0; i < td->tNodes.size(); ++i) {
        if (td->tNodes[i] && !td->tNodes[i]->parent) {
            td->tNodes[i]->dfs(res);
        }
    }
    for (int i = 0; i < td->vNodes.size(); ++i) {
        if (td->vNodes[i] && !td->vNodes[i]->parent) {
            td->vNodes[i]->dfs(res);
        }
    }
    return res;
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
    TreeDecomposition *td = getTDFromSequence(g, seq, false);
    td->print();
    vector<int> newSequence = TDToSequence(td);
    for (int i = 0; i < newSequence.size(); ++i) {
        cout << newSequence[i] << ' ';
    }
    cout << endl;
    cout << newSequence.size() << endl;

    g->resetTDStatus();
    TreeDecomposition *ntd = getTDFromSequence(g, seq, true);

    cout << "original width = " << td->width << endl;
    cout << "new width = " << ntd->width << endl;

    return 0;
}