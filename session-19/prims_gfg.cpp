class Solution {
public:
    int spanningTree(int V, vector<vector<int>>& edges) {
        
        vector<vector<pair<int,int>>> adj(V);
        
        for(auto &edge : edges){
            int u = edge[0];
            int v = edge[1];
            int w = edge[2];
            
            adj[u].push_back({v, w});
            adj[v].push_back({u, w});
        }
        
        priority_queue<pair<int,int>, 
                       vector<pair<int,int>>, 
                       greater<pair<int,int>>> pq;
        
        vector<bool> visited(V, false);
        
        pq.push({0, 0});  
        
        int sum = 0;
        
        while(!pq.empty()){
            auto top = pq.top();
            pq.pop();
            
            int weight = top.first;
            int node = top.second;
            
            if(visited[node]) continue;
            
            visited[node] = true;
            sum += weight;
            
            for(auto &it : adj[node]){
                int adjNode = it.first;
                int adjWeight = it.second;
                
                if(!visited[adjNode]){
                    pq.push({adjWeight, adjNode});
                }
            }
        }
        
        return sum;
    }
};