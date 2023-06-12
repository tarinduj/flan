#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <fstream>

#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

template <typename Key, typename Value>
class HashMap
{
private:
    // Initial capacity of the hash map.
    int INITIAL_CAPACITY = 1024;

    // Load factor that determines when to resize the map.
    double LOAD_FACTOR = 0.75;

    // Node structure: each node includes a key, a value, and a pointer to the next node.
    struct Node
    {
        Key key;
        Value value;
        Node *next;
    };

    // Capacity represents the size of the current array.
    int capacity;

    // Size represents the current total of stored keys.
    int size;

    // The array of pointers, each pointing to the first node of the linked list at each slot.
    Node **array;

    // Hash function to map a key to an index in the array.
    int hash(Key key)
    {
        return std::hash<Key>{}(key) % capacity;
    }

    // Function to resize the hash map when the load factor exceeds a certain threshold.
    // Creates a new array with double the size, and rehashes all keys.
    void resize()
    {
        int oldCapacity = capacity;
        Node **oldArray = array;
        capacity *= 2;
        size = 0;
        array = new Node *[capacity];
        for (int i = 0; i < capacity; i++)
        {
            array[i] = NULL;
        }
        for (int i = 0; i < oldCapacity; i++)
        {
            Node *node = oldArray[i];
            while (node)
            {
                insert(node->key, node->value);
                Node *temp = node;
                node = node->next;
                delete temp;
            }
        }
        delete[] oldArray;
    }

public:
    // Constructor for the hash map. Initializes the array and sets the initial capacity.
    HashMap() : capacity(INITIAL_CAPACITY), size(0)
    {
        array = new Node *[capacity];
        for (int i = 0; i < capacity; i++)
        {
            array[i] = NULL;
        }
    }

    // Copy constructor: creates a deep copy of the HashMap.
    HashMap(const HashMap<Key, Value>& src) : capacity(src.capacity), size(src.size), INITIAL_CAPACITY(src.INITIAL_CAPACITY), LOAD_FACTOR(src.LOAD_FACTOR) {
        array = new Node*[capacity];
        for (int i = 0; i < capacity; i++) {
            if (src.array[i] != nullptr) {
                array[i] = new Node{src.array[i]->key, src.array[i]->value, nullptr};
                Node* srcNode = src.array[i]->next;
                Node* newNode = array[i];
                while (srcNode != nullptr) {
                    newNode->next = new Node{srcNode->key, srcNode->value, nullptr};
                    srcNode = srcNode->next;
                    newNode = newNode->next;
                }
            } else {
                array[i] = nullptr;
            }
        }
    }

    // Overloaded assignment operator: creates a deep copy of the HashMap.
    HashMap<Key, Value>& operator=(const HashMap<Key, Value>& src) {
        if (this == &src) {
            return *this;
        }

        // Free the existing array
        for (int i = 0; i < capacity; i++) {
            delete array[i];
        }
        delete[] array;

        // Copy the source HashMap
        capacity = src.capacity;
        size = src.size;
        INITIAL_CAPACITY = src.INITIAL_CAPACITY;
        LOAD_FACTOR = src.LOAD_FACTOR;
        array = new Node*[capacity];
        for (int i = 0; i < capacity; i++) {
            if (src.array[i] != nullptr) {
                array[i] = new Node{src.array[i]->key, src.array[i]->value, nullptr};
                Node* srcNode = src.array[i]->next;
                Node* newNode = array[i];
                while (srcNode != nullptr) {
                    newNode->next = new Node{srcNode->key, srcNode->value, nullptr};
                    srcNode = srcNode->next;
                    newNode = newNode->next;
                }
            } else {
                array[i] = nullptr;
            }
        }
        return *this;
    }

    // Destructor for the hash map. Deletes all allocated memory.
    ~HashMap()
    {
        for (int i = 0; i < capacity; i++)
        {
            delete array[i];
        }
        delete[] array;
    }

    // Function to insert a key-value pair into the hash map.
    void insert(Key key, Value value)
    {
        int index = hash(key);
        Node *node = array[index];
        // If the key already exists in the hash map, update its value.
        while (node)
        {
            if (node->key == key)
            {
                node->value = value;
                return;
            }
            node = node->next;
        }
        // If the key does not exist in the hash map, add a new node at the beginning of the linked list at the hashed index.
        node = new Node;
        node->key = key;
        node->value = value;
        node->next = array[index];
        array[index] = node;
        size++;
        // If the load factor exceeds the threshold, resize the hash map.
        if ((float)size / capacity > LOAD_FACTOR)
        {
            resize();
        }
    }

    // Function to find a key in the hash map and return its corresponding value.
    Value* find(Key key)
    {
        int index = hash(key);
        Node *node = array[index];
        while (node) {
            if (node->key == key)
            {
                return &(node->value);
            }
            node = node->next;
        }
        return nullptr; // Return nullptr if key does not exist.
    }
};

long fsize(int fd) {
    struct stat stat;
    int res = fstat(fd, &stat);
    return stat.st_size;
}

void insert_to_2D(HashMap<string, string>& index, string v1, string v2) {
    string* kv = index.find(v1);
    if (kv == nullptr) {
        // new key
        index.insert(v1, v2);
    } else {
        // linear probing
        while (kv != nullptr) {
            if (*kv == v1) {
                *(kv+1) = v2;
                break;
            }
            kv += 2;
        }
        if (kv == nullptr) {
            index.insert(v1, v2);
        }
    }
}

void insert_to_3D(HashMap<string, HashMap<string, string>>& index, string v1, string v2, string v3) {
    HashMap<string, string>* kv = index.find(v1);
    if (kv == nullptr) {
        index.insert(v1, HashMap<string, string>());
        index.find(v1)->second.insert(v2, v3);
    } else {
        insert_to_2D(kv->second, v2, v3);
    }
}

void parse_two_column(int fd, HashMap<string, string>& index) {
    int size = fsize(fd);
    char* data = (char*)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
    int i = 0;
    while (i < size) {
        int start = i;
        while (data[i] != '\t') {
            i++;
        }
        string src = string(&data[start], i - start);
        i++;
        start = i;
        while (data[i] != '\n' && data[i] != '\000' && i < size) {
            i++;
        }
        string dest = string(&data[start], i - start);
        i++;
        // add to index
        insert_to_2D(index, src, dest);
    }
}

void parse_three_column(int fd, HashMap<string, HashMap<string, string>>& index, int order = 0) {
    int size = fsize(fd);
    char* data = (char*)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
    int i = 0;
    while (i < size) {
        int start = i;
        while (data[i] != '\t') {
            i++;
        }
        string v1 = string(&data[start], i - start);
        i++;

        start = i;
        while (data[i] != '\t') {
            i++;
        }
        string v2 = string(&data[start], i - start);
        i++;

        start = i;
        while (data[i] != '\n' && data[i] != '\000' && i < size) {
            i++;
        }
        string v3 = string(&data[start], i - start);
        i++;

        // add to index
        if (order == 0) {
            insert_to_3D(index, v1, v2, v3);
        } else if (order == 1) {
            // this is hardcoded for Store (where the second attribute is the key)
            insert_to_3D(index, v2, v3, v1);
        } else if (order == 2) {
            // hardcoded for Load
            // insert_to_3D(index, v1, v3, v2); // for the first join order
            insert_to_3D(index, v3, v1, v2); // for the second join order
        }
    }
}

void print_2D(HashMap<string, string>& data) {
    for (size_t i = 0; i < data.size(); i += 2) {
        cout << "(" << *(data.find(i)) << ", " << *(data.find(i + 1)) << ")\n";
    }
}

void print_3D(HashMap<string, HashMap<string, string>>& data) {
    for (size_t i = 0; i < data.size(); i += 2) {
        string k1 = *(data.find(i));
        HashMap<string, string>& innerMap = *(data.find(i + 1));
        for (size_t j = 0; j < innerMap.size(); j += 2) {
            string k2 = *(innerMap.find(j));
            string v = *(innerMap.find(j + 1));
            cout << "(" << k1 << ", " << k2 << ", " << v << ")\n";
        }
    }
}

bool is_new_2D(HashMap<string, string>& data, string k, string v) {
    string* kv = data.find(k);
    if (kv != nullptr) {
        return *(kv+1) != v;
    }
    return true;
}

bool is_new_3D(HashMap<string, HashMap<string, string>>& data, string k1, string k2, string v) {
    HashMap<string, string>* kv = data.find(k1);
    if (kv != nullptr) {
        HashMap<string, string>& innerMap = *kv;
        string* innerKv = innerMap.find(k2);
        if (innerKv != nullptr) {
            return *(innerKv+1) != v;
        }
    }
    return true;
}

void update_2D(HashMap<string, string>& data, HashMap<string, string>& new_data) {
    for (size_t i = 0; i < new_data.size(); i += 2) {
        data.insert(*(new_data.find(i)), *(new_data.find(i + 1)));
    }
}

void free_2D(HashMap<string, string>& data) {
    data = HashMap<string, string>(data.size() / 2);
}

int size_2D(HashMap<string, string>& data) {
    return data.size() / 2;
}

void save_to_file_2D(HashMap<string, string>& data, string fname) {
    ofstream file;
    file.open(fname);
    for (size_t i = 0; i < data.size(); i += 2) {
        file << *(data.find(i)) << "\t" << *(data.find(i + 1)) << "\n";
    }
    file.close();
}

int main() {
    // start timer
    auto start = std::chrono::high_resolution_clock::now();
    auto load_factor = 0.5;

    // indexes to store data
    HashMap<string, string> alloc;
    HashMap<string, string> assign;
    HashMap<string, HashMap<string, string>> load;
    HashMap<string, HashMap<string, string>> store;

    // varPointsTo(var:Variable, heap:Allocation)
    HashMap<string, string> vp;
    HashMap<string, string> vp_delta;
    HashMap<string, string> vp_new;

    // varPointsTo indexed by heap (required for recursive rule1)
    HashMap<string, string> vp2;
    HashMap<string, string> vp2_delta;
    HashMap<string, string> vp2_new;

    // alias(x:Variable,y:Variable)
    HashMap<string, string> alias;
    HashMap<string, string> alias_delta;
    HashMap<string, string> alias_new;

    // Assign(source:Variable, destination:Variable)
    HashMap<string, string> assign_delta;
    HashMap<string, string> assign_new;

    // parse the data and load into Extensional relations
    // alloc
    int alloc_fd = open("../../../souffle-simple-pa/facts/AssignAlloc.facts", 0);
    parse_two_column(alloc_fd, alloc);

    int assign_fd = open("../../../souffle-simple-pa/facts/PrimitiveAssign.facts", 0);
    parse_two_column(assign_fd, assign);

    int load_fd = open("../../../souffle-simple-pa/facts/Load.facts", 0);
    parse_three_column(load_fd, load, 2);

    int store_fd = open("../../../souffle-simple-pa/facts/Store.facts", 0);
    parse_three_column(store_fd, store, 1);

    // assign_delta = assign in the first iter
    update_2D(assign_delta, assign);

    // process base rules
    // Add to both full and delta
    // VarPointsTo(var, heap) :- AssignAlloc(var, heap).
    for (size_t i = 0; i < alloc.size(); i += 2) {
        string var = *(alloc.find(i));
        string heap = *(alloc.find(i + 1));
        // vp index1
        insert_to_2D(vp, var, heap);
        insert_to_2D(vp_delta, var, heap);

        // vp index2
        insert_to_2D(vp2, heap, var);
        insert_to_2D(vp2_delta, heap, var);
    }

    // recursive relations; fixed point loop
    int iter = 0;
    while (vp_delta.size() != 0 || alias_delta.size() != 0 || assign_delta.size() != 0) {
        iter++;

        /*
        Assign(var1, var2) :-
        Store(var1, instanceVar2, field),
        Alias(instanceVar2, instanceVar1),
        Load(instanceVar1, var2, field).
        */
        if (alias_delta.size() != 0) {
            for (size_t i = 0; i < store.size(); i += 2) {
                string instanceVar2 = *(store.find(i));
                string* alias_iter = alias_delta.find(instanceVar2);
                if (alias_iter != nullptr) {
                    for (size_t j = 0; j < store.find(i + 1)->size(); j += 2) {
                        string field = *(store.find(i + 1)->find(j));
                        HashMap<string, string>* load_iter = load.find(field);
                        if (load_iter != nullptr) {
                            for (size_t k = 0; k < load_iter->size(); k += 2) {
                                string instanceVar1 = *(load_iter->find(k));
                                if (*alias_iter == instanceVar1) {
                                    // means, var1 and var2 satisfy the conditions
                                    string var1 = *(store.find(i + 1)->find(j + 1));
                                    string var2 = *(load_iter->find(k + 1));
                                    if (is_new_2D(assign, var1, var2)) {
                                        insert_to_2D(assign_new, var1, var2);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        /*
        VarPointsTo(var1, heap) :-
        Assign(var2, var1),
        VarPointsTo(var2, heap).
        */
        // delta, stable
        // delta, delta
        for (size_t i = 0; i < assign_delta.size(); i += 2) {
            string var2 = *(assign_delta.find(i));
            string heap = *(vp.find(var2));
            string delta_heap = *(vp_delta.find(var2));

            if (!heap.empty() || !delta_heap.empty()) {
                for (size_t j = 0; j < assign_delta.find(i + 1)->size(); j += 2) {
                    // delta, stable
                    if (!heap.empty()) {
                        string var1 = *(assign_delta.find(i + 1)->find(j));
                        if (is_new_2D(vp, var1, heap)) {
                            insert_to_2D(vp_new, var1, heap);
                            insert_to_2D(vp2_new, heap, var1);
                        }
                    }

                    // delta, delta
                    if (!delta_heap.empty()) {
                        string var1 = *(assign_delta.find(i + 1)->find(j));
                        if (is_new_2D(vp, var1, delta_heap)) {
                            insert_to_2D(vp_new, var1, delta_heap);
                            insert_to_2D(vp2_new, delta_heap, var1);
                        }
                    }
                }
            }
        }

        // stable, delta
        if (!vp_delta.empty()) {
            for (size_t i = 0; i < assign.size(); i += 2) {
                string var2 = *(assign.find(i));
                string delta_heap = *(vp_delta.find(var2));
                if (!delta_heap.empty()) {
                    for (size_t j = 0; j < assign.find(i + 1)->size(); j += 2) {
                        string var1 = *(assign.find(i + 1)->find(j));
                        if (is_new_2D(vp, var1, delta_heap)) {
                            insert_to_2D(vp_new, var1, delta_heap);
                            insert_to_2D(vp2_new, delta_heap, var1);
                        }
                    }
                }
            }
        }

        /*
        Alias(instanceVar, iVar) :-
        VarPointsTo(instanceVar, instanceHeap),
        VarPointsTo(iVar, instanceHeap).
        */
        // delta, stable
        // delta, delta
        for (size_t i = 0; i < vp_delta.size(); i += 2) {
            string instanceVar = *(vp_delta.find(i));
            string instanceHeap = *(vp_delta.find(i + 1));
            // use index2
            // stable
            string iVar = *(vp2.find(instanceHeap));
            string delta_iVar = *(vp2_delta.find(instanceHeap));
            if (!iVar.empty()) {
                if (is_new_2D(alias, instanceVar, iVar)) {
                    insert_to_2D(alias_new, instanceVar, iVar);
                }
            }

            // delta
            if (!delta_iVar.empty()) {
                if (is_new_2D(alias, instanceVar, delta_iVar)) {
                    insert_to_2D(alias_new, instanceVar, delta_iVar);
                }
            }
        }

        // stable, delta
        if (!vp_delta.empty()) {
            for (size_t i = 0; i < vp.size(); i += 2) {
                string instanceVar = *(vp.find(i));
                string instanceHeap = *(vp.find(i + 1));
                // delta
                string delta_iVar = *(vp2_delta.find(instanceHeap));
                if (!delta_iVar.empty()) {
                    if (is_new_2D(alias, instanceVar, delta_iVar)) {
                        insert_to_2D(alias_new, instanceVar, delta_iVar);
                    }
                }
            }
        }

        // relation update business
        // vp, vp2, alias, assign
        // stable += delta
        update_2D(vp, vp_new);
        update_2D(vp2, vp2_new);
        update_2D(alias, alias_new);
        update_2D(assign, assign_new);

        // free memory of deltas
        free_2D(vp_delta);
        free_2D(vp2_delta);
        free_2D(alias_delta);
        free_2D(assign_delta);

        // delta <- new
        vp_delta = vp_new;
        vp2_delta = vp2_new;
        alias_delta = alias_new;
        assign_delta = assign_new;
    }


    std::cout << "Hey hey " << iter << endl;

    // write the output to files
    save_to_file_2D(vp, "VarPointsTo.csv");
    save_to_file_2D(alias, "Alias.csv");
    save_to_file_2D(assign, "Assign.csv");

    // end timer
    auto end = chrono::steady_clock::now();
    cout << "Time elapsed: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;
}
