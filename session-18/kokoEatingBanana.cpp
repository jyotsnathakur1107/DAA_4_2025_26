class Solution {
public:
    long long isPossible(vector<int>& piles, int n){
        long long temp = 0;
        for(auto &i : piles){
            temp += (i + n - 1)/n;
        }
        return temp;
    }
    int minEatingSpeed(vector<int>& piles, int h) {
        int mn = 1;
        auto mx = *max_element(piles.begin(), piles.end());
        int ans = mx;
        while(mn <= mx){
            int mid = mn + (mx - mn)/2;
            long long hours_taken = isPossible(piles, mid);
            if(hours_taken <= (long long)h){
                ans = mid;
                mx = mid - 1;
            }else{
                mn = mid+1;
            }
        }
        return ans;
    }
};