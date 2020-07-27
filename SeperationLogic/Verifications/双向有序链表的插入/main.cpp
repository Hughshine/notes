#include <iostream>
using namespace std;
struct BiListNode  {
    int data;
    BiListNode* next;
    BiListNode* former;
    BiListNode(int data, BiListNode* next=NULL, BiListNode* former=NULL):data(data),next(next),former(former){}
};

struct BiList {
    BiListNode* head;
    BiListNode* tail;
    BiList(BiListNode* head, BiListNode* tail):head(head),tail(tail){}
};

BiList* genList(int cnt) {
    if(cnt == 0) {
        return new BiList(NULL, NULL);
    }

    int i = 0;
    BiListNode *head = new BiListNode(i++);
    BiListNode *cur = head;
    while (--cnt > 0) {
        cur->next = new BiListNode(i++);
        cur->next->former = cur;
        cur = cur->next;
    }
    BiList* list = new BiList(head, cur);
    return list;
}

void prtList(BiList* list) {
    BiListNode* head = list->head;
    while(head != NULL) {
        cout << head->data << " ";
        head = head->next;
    }
    cout << endl;
    BiListNode* tail = list->tail;
    while(tail != NULL) {
        cout << tail->data << " ";
        tail = tail->former;
    }
    cout << endl;    
}

//  {list 以 NULL 开头结尾}
void insert(BiList* list, int val) {
    BiListNode prehead = BiListNode(INT_MIN, list->head, NULL);
    BiListNode* cur = &prehead;  // prehead 并没有真正加入list中，因为head->former没有指向它；其实我们需要的只是一个指针，prehead.next = head；
    while (cur->next != NULL && cur->next->data < val) {
        cur = cur->next;
    }
    // {cur->data < a && cur->next->data >= a && bilist list i,j * binode &prehead && lst prehead·list i}
    BiListNode* node = new BiListNode(val, cur->next, cur);
    // {cur->data < a && cur->next->data >= a && bilist list i,j * binode node * binode &prehead}
    if(cur == &prehead) {
        node->former = NULL;  
        list->head = node;
    }
    cur->next = node;
    if(node->next != NULL) {
        node->next->former = node;
    } else {
        list->tail = node;
    }
}


void insert2(BiList* list, int val) {
    if(list->head == NULL) {  // 构造只有单一节点的list
        list->head = new BiListNode(val);
        list->tail = list->head;
    } else if(list->head->data >= val) {  // 插入在最前
        BiListNode* newhead = new BiListNode(val, list->head);
        list->head->former = newhead;
        list->head = newhead;
    } else if(list->tail->data < val) {  // 插入在最后
        BiListNode *newtail = new BiListNode(val, NULL, list->tail);
        list->tail->next = newtail;
        list->tail= newtail;
    } else {  // 插入在中间
        BiListNode* former = list->head;
        BiListNode* cur = list->head->next;
        while(val > cur->data) {
            former = cur;
            cur = cur->next;
        }
        BiListNode* newnode = new BiListNode(val, cur, former);
        cur->former = newnode;
        former->next = newnode;
    }
}

int main() {
    BiList* list = genList(5);
    prtList(list);
    insert2(list, -1);
    prtList(list);
    insert2(list, 3);
    prtList(list);
    insert2(list, 10);
    prtList(list);
}