#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <iostream>
#include <vector>
#include <set>

#include "udg.hpp"

using namespace std;

class TDNode {
public:
    set<int> active, unactive;
    vector<TDNode *> childs;
    TDNode *parent;
    int n, activeN, unactiveN;
    bool onlyWidth;

    TDNode(bool _onlyWidth = false) {
        active.clear(); unactive.clear();
        parent = NULL;
        childs.clear();
        n = activeN = unactiveN = 0;
        onlyWidth = _onlyWidth;
    }
    void push(int x) {
        if (!onlyWidth) {
            active.insert(x);
        }
        n += 1;
        activeN += 1;
    }
    void makeUnactive(int x) {
        activeN -= 1;
        unactiveN += 1;
        if (onlyWidth) {
            return ;
        }
        set<int>::iterator actLoc = active.find(x);
        if (actLoc == active.end()) {
            cerr << "Error: node " << x << " to be made unactive not found!" << endl;
            return ;
        }
        active.erase(actLoc);
        unactive.insert(x);

        cout << x << " is made unactive" << endl;
    }

    void pushNode(TDNode *child) {
        n += child->activeN;
        activeN += child->activeN;
        child->parent = this;
        if (onlyWidth) {
            return ;
        }
        active.insert(child->active.begin(), child->active.end());
        childs.push_back(child);
    }
    int size() {
        return n;
    }

    void print() {
        if (onlyWidth) {
            cout << "Node(width = " << (n - 1) << ")" << endl;
            return ;
        }
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

    void dfs(vector<int> &res) {
        cout << "dfs at active = ";
        for (set<int>::iterator p = active.begin(); p != active.end(); ++p) {
            cout << (*p) << ' ';
        }
        cout << ", ";
        cout << "unactive = ";
        for (set<int>::iterator p = unactive.begin(); p != unactive.end(); ++p) {
            cout << (*p) << ' ';
        }
        cout << endl;
        for (int i = 0; i < childs.size(); ++i) {
            childs[i]->dfs(res);
        }
        res.insert(res.end(), unactive.begin(), unactive.end());
    }
};

class TreeDecomposition {
public:
    vector<TDNode *> vNodes;
    vector<TDNode *> tNodes;
    int width;
    int n;
    bool onlyWidth;

    TreeDecomposition(UndirectedGraph *g, bool _onlyWidth = false): onlyWidth(_onlyWidth) {
        n = g->n;
        vNodes = vector<TDNode *>(n);
        for (int i = 0; i < n; ++i) {
            vNodes[i] = new TDNode(onlyWidth);
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