/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* ans;
    ListNode* travel(ListNode* curr, ListNode* prev){
        if(curr == nullptr){
            return prev;
        }
        ListNode* temp = curr->next;
        curr->next=prev;
        prev=curr;
        curr=temp;
        return travel(curr,prev);
    }
    ListNode* reverseList(ListNode* head) {
        ListNode* prev = NULL;
        return travel(head, prev);
    }
};