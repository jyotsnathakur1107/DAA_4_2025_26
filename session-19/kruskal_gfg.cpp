class Solution {
public:

    int findParent(int node, vector<int>& parent) {
        if(parent[node] == node)
            return node;
        return parent[node] = findParent(parent[node], parent);
    }

    void unionBySize(int u, int v, vector<int>& parent, vector<int>& size) {
        u = findParent(u, parent);
        v = findParent(v, parent);

        if(u == v) return;

        if(size[u] < size[v]) {
            parent[u] = v;
            size[v] += size[u];
        } else {
            parent[v] = u;
            size[u] += size[v];
        }
    }

    int kruskalsMST(int V, vector<vector<int>>& edges) {

        sort(edges.begin(), edges.end(),
             [](vector<int>& a, vector<int>& b){
                 return a[2] < b[2];
             });

        vector<int> parent(V);
        vector<int> size(V, 1);  

        for(int i = 0; i < V; i++)
            parent[i] = i;

        int mstWeight = 0;

        for(auto &edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int w = edge[2];

            if(findParent(u, parent) != findParent(v, parent)) {
                mstWeight += w;
                unionBySize(u, v, parent, size);
            }
        }

        return mstWeight;
    }
};