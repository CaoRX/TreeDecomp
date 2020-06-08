#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

using namespace std;

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