#include <iostream>
#include <string>
#include <queue>
using namespace std; 

struct Node {
  string value;
  int left = -1;
  int right = -1; 
} BT[101]; 

string tree = "r()"; 

// construct a tree 
void constructTree() {
  BT[1].value = "A"; 
  BT[1].left = 2;
  BT[1].right = 3;
  BT[2].value = "B";
  BT[2].left = 4;
  BT[2].right = 5;
  BT[3].value = "C";
  BT[3].left = 6;
  BT[3].right = 7;
  BT[4].value = "D";
  BT[5].value = "E";
  BT[6].value = "F";
  BT[7].value = "G";
}

// preorder traversal
void preorder(int node) {
  if(node == -1) return;

  cout << BT[node].value << " "; 
  preorder(BT[node].left);
  preorder(BT[node].right);
}

// inorder traversal
void inorder(int node) {
  if(node == -1) return;

  inorder(BT[node].left);
  cout << BT[node].value << " "; 
  inorder(BT[node].right);
}

// postorder traversal
void postorder(int node) {
  if(node == -1) return;

  postorder(BT[node].left);
  postorder(BT[node].right);
  cout << BT[node].value << " "; 
}


// level order traversal 
void bfs(int node) {
  queue<int> que; 
  que.push(node); 

  while(!que.empty()) {
    Node current_node = BT[que.front()];
    que.pop(); 

    // print out the current node 
    cout << current_node.value << " "; 
    
    // push left and right into the queue 
    if(current_node.left != -1)
      que.push(current_node.left);
    if(current_node.right != -1)
      que.push(current_node.right);
  }
}

// exercise 
int main() {
  // construct a tree 
  constructTree(); 
  int root = 1; 
  string tree = R"(
          A
        /   \
      B      C
    /  \    /  \
   D    E   F   G)"; 
   
  cout << "tree visualized";
  cout << tree << endl;

  // TODO: look at the 4 types of traversal below, and answer the following 2 questions/
  // 1. what are the difference in code?
  // 2. what are the difference in the result? (hint: bottom up, top down, or something else?)
  
  // preorder traversal
  cout << "pre-order traversal:   ";
  preorder(root); 
  cout << endl;
  
  // in_order traversal
  cout << "in-order traversal:    ";
  inorder(root); 
  cout << endl;
  
  // postorder traversal 
  cout << "post-order traversal:  ";
  postorder(root); 
  cout << endl;

  // levelorder traversal
  cout << "level-order traversal: "; 
  bfs(root);
  cout << endl; 
}