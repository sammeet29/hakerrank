#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
using namespace std;

struct Node{
    Node* next;
    Node* prev;
    int value;
    int key;
    Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
    Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{
	public:
		void print_list();

	protected: 
		map<int,Node*> mp; //map the key to the node in the linked list
		int cp;  //capacity
		Node* tail; // double linked list tail pointer
		Node* head; // double linked list head pointer
		virtual void set(int, int) = 0; //set function
		virtual int get(int) = 0; //get function
		
};

void Cache::print_list(){
	Node * curr = head;
	std::cout << "------------List-------------" << endl;
	while( curr != NULL ){
		std::cout << curr->key << " , " << curr->value << endl; 
		curr = curr->next;
	}
	std::cout << endl;
}

/*********************** My code ******************************/
class LRUCache:public Cache{
	public:
		void set(int,int);
		int get(int);
		LRUCache(int);

	private:
		void rearrange_cache(Node *, bool);
};

LRUCache::LRUCache(int lcapacity){
	cp = lcapacity;
}

void LRUCache:: set(int key, int value){
	// find key from map
	map<int, Node *>::iterator it = mp.find(key);

	// if found
	if(it != mp.end()){
		// update value
		it->second->value = value;

		// rearrange cache
		rearrange_cache(it->second, false);

	}else{  
		// else make new
		Node * new_node = new Node(key,value);
		
		// rearrange cache
		rearrange_cache(new_node, true);

		// add to map
		mp[key] = new_node;
	}
	
}

int LRUCache::get(int key){
	int value = -1;
	Node * node = NULL;
	map<int,Node *>::iterator it = mp.find(key);
	
	if(it != mp.end()){
		// get value
		node = it->second;
		value = node->value;

		//rearrange cache
		rearrange_cache(node, false);        
	}
	
	return (value);
}

void LRUCache::rearrange_cache(Node * node, bool new_key){
	if(new_key){
		// add to map
		int key = node->key;
		mp[key] = node;

		// if empty list make node tail
		if(head == NULL){            
			tail = node;
		}else{
					//else insert node at head
			node->next = head;
		}

		// make node head
		head = node;


			// if capacity > max_capacity
		if(mp.size() > cp){
			// delete the last node, move tail to prev
			Node * last_node = tail;
			tail = tail->prev;

			// delete last node from map
			mp.erase(last_node->key);

		}        

	}else{

		// we dont have to do anthing if node is head
		if(node != head) {
			if( node != tail ){
				node->next->prev = node->prev;

			}else if(node == tail){

				tail = node->prev;
			}

			node->prev->next = node->next;

			// move node to head
			node->prev = NULL;
			node->next = head;
			head = node;
		}
	}
	
}

/*****************************************************************/

int main() {
    int n, capacity,i;
    cin >> n >> capacity;
    LRUCache l(capacity);
    for(i=0;i<n;i++) {
        string command;
        cin >> command;
        if(command == "get") {
            int key;
            cin >> key;
            cout << l.get(key) << endl;
        } 
        else if(command == "set") {
            int key, value;
            cin >> key >> value;
            l.set(key,value);
        }
        l.print_list();
    }
    return 0;
}


