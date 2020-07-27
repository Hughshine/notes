#include <iostream>
using namespace std;
struct ListNode  {
    int data;
    ListNode* next;
    ListNode(int data, ListNode* next=NULL):data(data),next(next){}
};

struct List {
    ListNode* head;
    List(ListNode* head):head(head){}
};

List* genList(int cnt) {
    if(cnt == 0) {
        return new List(NULL);
    }

    int i = 0;
    ListNode *head = new ListNode(i++);
    ListNode *cur = head;
    while (--cnt > 0) {
        cur->next = new ListNode(i++);
        cur = cur->next;
    }
    List* list = new List(head);
    return list;
}

void prtList(List* list) {
    ListNode* head = list->head;
    while(head != NULL) {
        cout << head->data << " ";
        head = head->next;
    }
    cout << endl; 
}

// {list \overline{\gamma} (head)}
void insert(List* list, int val) {
    // if(list->head == NULL) {  
    //     list->head = new ListNode(val);
    // } else {
        ListNode* former = NULL;
        ListNode* cur = list->head;
        while (cur != NULL && val > cur->data)
        {
            former = cur;
            cur = cur->next;
        }
        if(former == NULL) {   // 要插入在第一个位置前
            list->head = new ListNode(val, cur);
        } else {  // 要插入在中间位置
            former->next = new ListNode(val, cur);
        } 
    // }
}
// {\exists \alpha, \beta. list \overline{\alpha\cdot val\cdot \beta} \wedge \alpha \cdot \beta = gamma}}

void insert2(List* list, int val) {
    if(list->head == NULL) {  
        list->head = new ListNode(val);
    } else if(list->head->data >= val){
        list->head = new ListNode(val, list->head);
    } else {
        ListNode* former = list->head;
        ListNode* cur = former->next;
        while(cur != NULL && val > cur->data){
            former = cur;
            cur = cur->next;
        }
        former->next = new ListNode(val, cur);
    }
}

int main() {
    List* list = genList(5);
    prtList(list);
    insert2(list, -1);
    prtList(list);
    insert2(list, 3);
    prtList(list);
    insert2(list, 10);
    prtList(list);
}