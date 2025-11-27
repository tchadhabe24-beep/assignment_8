#include <iostream>
#include <vector>
#include <limits>
using namespace std;


struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int val) : data(val), left(nullptr), right(nullptr) {}
};


void preorder(Node* root) {
    if (root == nullptr) return;
    cout << root->data << " ";
    preorder(root->left);
    preorder(root->right);
}

void inorder(Node* root) {
    if (root == nullptr) return;
    inorder(root->left);
    cout << root->data << " ";
    inorder(root->right);
}

void postorder(Node* root) {
    if (root == nullptr) return;
    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";
}



// Insert into BST (no duplicates)
Node* bstInsert(Node* root, int key) {
    if (root == nullptr) {
        return new Node(key);
    }
    if (key < root->data) {
        root->left = bstInsert(root->left, key);
    } else if (key > root->data) {
        root->right = bstInsert(root->right, key);
    } else {
        cout << "Duplicate key " << key << " not inserted.\n";
    }
    return root;
}

// Search (recursive)
Node* bstSearchRec(Node* root, int key) {
    if (root == nullptr || root->data == key)
        return root;
    if (key < root->data)
        return bstSearchRec(root->left, key);
    else
        return bstSearchRec(root->right, key);
}

// Search (non-recursive / iterative)
Node* bstSearchIter(Node* root, int key) {
    Node* curr = root;
    while (curr != nullptr) {
        if (key == curr->data) return curr;
        else if (key < curr->data) curr = curr->left;
        else curr = curr->right;
    }
    return nullptr;
}

// Minimum element in BST
Node* bstMin(Node* root) {
    if (root == nullptr) return nullptr;
    Node* curr = root;
    while (curr->left != nullptr)
        curr = curr->left;
    return curr;
}

// Maximum element in BST
Node* bstMax(Node* root) {
    if (root == nullptr) return nullptr;
    Node* curr = root;
    while (curr->right != nullptr)
        curr = curr->right;
    return curr;
}

// Inorder Successor (using root and key)
Node* inorderSuccessor(Node* root, int key) {
    Node* target = bstSearchIter(root, key);
    if (target == nullptr) return nullptr;

    // Case 1: right subtree exists -> min of right subtree
    if (target->right != nullptr) {
        return bstMin(target->right);
    }

    // Case 2: no right subtree -> climb from root and track successor
    Node* succ = nullptr;
    Node* ancestor = root;
    while (ancestor != target) {
        if (key < ancestor->data) {
            succ = ancestor;
            ancestor = ancestor->left;
        } else {
            ancestor = ancestor->right;
        }
    }
    return succ;
}

// Inorder Predecessor (using root and key)
Node* inorderPredecessor(Node* root, int key) {
    Node* target = bstSearchIter(root, key);
    if (target == nullptr) return nullptr;

    // Case 1: left subtree exists -> max of left subtree
    if (target->left != nullptr) {
        return bstMax(target->left);
    }

    // Case 2: no left subtree -> climb from root and track predecessor
    Node* pred = nullptr;
    Node* ancestor = root;
    while (ancestor != target) {
        if (key > ancestor->data) {
            pred = ancestor;
            ancestor = ancestor->right;
        } else {
            ancestor = ancestor->left;
        }
    }
    return pred;
}

// Delete node from BST
Node* bstDelete(Node* root, int key) {
    if (root == nullptr) return nullptr;

    if (key < root->data) {
        root->left = bstDelete(root->left, key);
    } else if (key > root->data) {
        root->right = bstDelete(root->right, key);
    } else {
        // Found node to delete
        if (root->left == nullptr && root->right == nullptr) {
            // Leaf
            delete root;
            return nullptr;
        } else if (root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        } else {
            // Two children: replace with inorder successor
            Node* succ = bstMin(root->right);
            root->data = succ->data;
            root->right = bstDelete(root->right, succ->data);
        }
    }
    return root;
}

// Maximum depth of BST
int maxDepth(Node* root) {
    if (root == nullptr) return 0;
    int leftDepth = maxDepth(root->left);
    int rightDepth = maxDepth(root->right);
    return 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
}

// Minimum depth of BST
int minDepth(Node* root) {
    if (root == nullptr) return 0;
    // If one subtree is null, we must take the other
    if (root->left == nullptr && root->right == nullptr) return 1;
    if (root->left == nullptr) return 1 + minDepth(root->right);
    if (root->right == nullptr) return 1 + minDepth(root->left);

    int leftDepth = minDepth(root->left);
    int rightDepth = minDepth(root->right);
    return 1 + (leftDepth < rightDepth ? leftDepth : rightDepth);
}


bool isBSTUtil(Node* root, long long minVal, long long maxVal) {
    if (root == nullptr) return true;
    if (root->data <= minVal || root->data >= maxVal) return false;
    return isBSTUtil(root->left, minVal, root->data) &&
           isBSTUtil(root->right, root->data, maxVal);
}

bool isBST(Node* root) {
    return isBSTUtil(root,
                     numeric_limits<long long>::min(),
                     numeric_limits<long long>::max());
}



void heapify(int arr[], int n, int i, bool isMinHeap) {
    int extreme = i; // largest for max-heap, smallest for min-heap
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    // For max-heap: choose largest; for min-heap: choose smallest
    if (l < n) {
        if (!isMinHeap && arr[l] > arr[extreme]) extreme = l;
        if (isMinHeap && arr[l] < arr[extreme]) extreme = l;
    }
    if (r < n) {
        if (!isMinHeap && arr[r] > arr[extreme]) extreme = r;
        if (isMinHeap && arr[r] < arr[extreme]) extreme = r;
    }

    if (extreme != i) {
        swap(arr[i], arr[extreme]);
        heapify(arr, n, extreme, isMinHeap);
    }
}

void heapSort(int arr[], int n, bool increasing) {
    bool isMinHeap = !increasing; // increasing -> max-heap; decreasing -> min-heap

    // Build heap
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i, isMinHeap);
    }

    // Extract elements one by one
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);  // Move current root to end
        heapify(arr, i, 0, isMinHeap);
    }

    // For decreasing order using min-heap, array is currently increasing,
    // so we reverse it
    if (!increasing) {
        for (int i = 0; i < n / 2; i++) {
            swap(arr[i], arr[n - 1 - i]);
        }
    }
}


class PriorityQueueHeap {
private:
    vector<int> heap; // max-heap

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return 2 * i + 1; }
    int right(int i) { return 2 * i + 2; }

    void heapifyDown(int i) {
        int n = heap.size();
        int largest = i;
        int l = left(i);
        int r = right(i);

        if (l < n && heap[l] > heap[largest]) largest = l;
        if (r < n && heap[r] > heap[largest]) largest = r;

        if (largest != i) {
            swap(heap[i], heap[largest]);
            heapifyDown(largest);
        }
    }

    void heapifyUp(int i) {
        while (i != 0 && heap[parent(i)] < heap[i]) {
            swap(heap[i], heap[parent(i)]);
            i = parent(i);
        }
    }

public:
    bool empty() const { return heap.empty(); }

    void push(int x) {
        heap.push_back(x);
        heapifyUp(heap.size() - 1);
    }

    int top() {
        if (heap.empty()) {
            cout << "Priority queue is empty!\n";
            return -1;
        }
        return heap[0];
    }

    void pop() {
        if (heap.empty()) {
            cout << "Priority queue is empty!\n";
            return;
        }
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) heapifyDown(0);
    }

    void print() {
        cout << "Heap elements (level order): ";
        for (int x : heap) cout << x << " ";
        cout << "\n";
    }
};


Node* createSampleBinaryTree() {
    // Not necessarily a BST
    Node* root = new Node(10);
    root->left = new Node(5);
    root->right = new Node(20);
    root->left->left = new Node(1);
    root->left->right = new Node(15); // 15 on left side violates BST rule
    return root;
}


// MENUS & MAIN
// ==========================
void bstMenu(Node*& root) {
    int choice;
    do {
        cout << "\n--- BST MENU ---\n";
        cout << "1. Insert element\n";
        cout << "2. Delete element\n";
        cout << "3. Search (recursive)\n";
        cout << "4. Search (iterative)\n";
        cout << "5. Find minimum\n";
        cout << "6. Find maximum\n";
        cout << "7. Inorder successor\n";
        cout << "8. Inorder predecessor\n";
        cout << "9. Traversals (Pre/In/Post)\n";
        cout << "10. Max depth\n";
        cout << "11. Min depth\n";
        cout << "0. Back to main menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        int key;
        Node* temp;

        switch (choice) {
        case 1:
            cout << "Enter value to insert: ";
            cin >> key;
            root = bstInsert(root, key);
            break;
        case 2:
            cout << "Enter value to delete: ";
            cin >> key;
            root = bstDelete(root, key);
            break;
        case 3:
            cout << "Enter key to search (recursive): ";
            cin >> key;
            temp = bstSearchRec(root, key);
            if (temp) cout << "Found.\n"; else cout << "Not found.\n";
            break;
        case 4:
            cout << "Enter key to search (iterative): ";
            cin >> key;
            temp = bstSearchIter(root, key);
            if (temp) cout << "Found.\n"; else cout << "Not found.\n";
            break;
        case 5:
            temp = bstMin(root);
            if (temp) cout << "Minimum = " << temp->data << "\n";
            else cout << "Tree is empty.\n";
            break;
        case 6:
            temp = bstMax(root);
            if (temp) cout << "Maximum = " << temp->data << "\n";
            else cout << "Tree is empty.\n";
            break;
        case 7:
            cout << "Enter key for inorder successor: ";
            cin >> key;
            temp = inorderSuccessor(root, key);
            if (temp) cout << "Inorder successor = " << temp->data << "\n";
            else cout << "No successor (maybe key not found or it's the largest).\n";
            break;
        case 8:
            cout << "Enter key for inorder predecessor: ";
            cin >> key;
            temp = inorderPredecessor(root, key);
            if (temp) cout << "Inorder predecessor = " << temp->data << "\n";
            else cout << "No predecessor (maybe key not found or it's the smallest).\n";
            break;
        case 9:
            cout << "Preorder: ";
            preorder(root);
            cout << "\nInorder: ";
            inorder(root);
            cout << "\nPostorder: ";
            postorder(root);
            cout << "\n";
            break;
        case 10:
            cout << "Max depth = " << maxDepth(root) << "\n";
            break;
        case 11:
            cout << "Min depth = " << minDepth(root) << "\n";
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

void heapSortMenu() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;
    if (n <= 0) return;
    int* arr = new int[n];
    cout << "Enter elements: ";
    for (int i = 0; i < n; i++) cin >> arr[i];

    int choice;
    cout << "1. Increasing order\n";
    cout << "2. Decreasing order\n";
    cout << "Enter choice: ";
    cin >> choice;
    bool increasing = (choice == 1);

    heapSort(arr, n, increasing);

    cout << "Sorted array: ";
    for (int i = 0; i < n; i++) cout << arr[i] << " ";
    cout << "\n";

    delete[] arr;
}

void priorityQueueMenu() {
    PriorityQueueHeap pq;
    int choice;
    do {
        cout << "\n--- PRIORITY QUEUE (Max-Heap) MENU ---\n";
        cout << "1. Insert (push)\n";
        cout << "2. Get max (top)\n";
        cout << "3. Delete max (pop)\n";
        cout << "4. Print heap\n";
        cout << "0. Back to main menu\n";
        cout << "Enter choice: ";
        cin >> choice;

        int x;
        switch (choice) {
        case 1:
            cout << "Enter value to insert: ";
            cin >> x;
            pq.push(x);
            break;
        case 2:
            cout << "Max element = " << pq.top() << "\n";
            break;
        case 3:
            pq.pop();
            break;
        case 4:
            pq.print();
            break;
        case 0:
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

int main() {
    Node* bstRoot = nullptr;

    int choice;
    do {
        cout << "\n=========== MAIN MENU ===========\n";
        cout << "1. BST operations (insert/delete/search/succ/pred/depth/traversal)\n";
        cout << "2. Check if a binary tree is BST (using a sample tree)\n";
        cout << "3. Heapsort (increasing/decreasing)\n";
        cout << "4. Priority Queue using Heap\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            bstMenu(bstRoot);
            break;
        case 2: {
            Node* root = createSampleBinaryTree();
            cout << "Inorder of sample tree: ";
            inorder(root);
            cout << "\nIs this tree a BST? ";
            if (isBST(root)) cout << "YES\n";
            else cout << "NO\n";
            break;
        }
        case 3:
            heapSortMenu();
            break;
        case 4:
            priorityQueueMenu();
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
