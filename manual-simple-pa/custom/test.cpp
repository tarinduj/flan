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

    // Node structure: each node includes a key, a value, and a boolean that represents if the node is occupied.
    struct Node
    {
        Key key;
        Value value;
        bool isOccupied;
    };

    // Capacity represents the size of the current array.
    int capacity;

    // Size represents the current total of stored keys.
    int size;

    // The array of Nodes.
    Node *array;

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
        Node *oldArray = array;
        capacity *= 2;
        size = 0;
        array = new Node[capacity];
        for (int i = 0; i < capacity; i++)
        {
            array[i].isOccupied = false;
        }
        for (int i = 0; i < oldCapacity; i++)
        {
            if (oldArray[i].isOccupied)
            {
                insert(oldArray[i].key, oldArray[i].value);
            }
        }
        delete[] oldArray;
    }

public:
    // Constructor for the hash map. Initializes the array and sets the initial capacity.
    HashMap() : capacity(INITIAL_CAPACITY), size(0)
    {
        array = new Node[capacity];
        for (int i = 0; i < capacity; i++)
        {
            array[i].isOccupied = false;
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
        delete[] array;
    }

    void insert(Key key, Value* value)
    {
        int index = hash(key);
        while (array[index].isOccupied && array[index].key != key)
        {
            index = (index + 1) % capacity;
        }
        if (!array[index].isOccupied)
        {
            size++;
        }
        array[index].key = key;
        array[index].value = value;
        array[index].isOccupied = true;

        // If the load factor exceeds the threshold, resize the hash map.
        if ((double)size / capacity > LOAD_FACTOR)
        {
            resize();
        }
    }

    // Function to find a key in the hash map and return its corresponding key-value pair.
    std::pair<Key, Value*>* find(Key key)
    {
        int index = hash(key);
        while (array[index].isOccupied)
        {
            if (array[index].key == key)
            {
                return new std::pair<Key, Value*>(array[index].key, array[index].value);
            }
            index = (index + 1) % capacity;
        }
        return nullptr; // Return nullptr if key does not exist.
    }

    // // Function to insert a key-value pair into the hash map using linear probing.
    // void insert(Key key, Value value)
    // {
    //     int index = hash(key);
    //     while (array[index].isOccupied && array[index].key != key)
    //     {
    //         index = (index + 1) % capacity;
    //     }
    //     if (!array[index].isOccupied)
    //     {
    //         size++;
    //     }
    //     array[index].key = key;
    //     array[index].value = value;
    //     array[index].isOccupied = true;

    //     // If the load factor exceeds the threshold, resize the hash map.
    //     if ((double)size / capacity > LOAD_FACTOR)
    //     {
    //         resize();
    //     }
    // }

    // // Function to find a key in the hash map and return its corresponding key-value pair.
    // std::pair<Key, Value>* find(Key key)
    // {
    //     int index = hash(key);
    //     while (array[index].isOccupied)
    //     {
    //         if (array[index].key == key)
    //         {
    //             return new std::pair<Key, Value>(array[index].key, array[index].value);
    //         }
    //         index = (index + 1) % capacity;
    //     }
    //     return nullptr; // Return nullptr if key does not exist.
    // }

    // Function to get capacity of the hash map.
    int getCapacity()
    {
        return capacity;
    }

    class iterator
    {
        private:
            int index;
            Node* nodeArray;
            int arraySize;

        public:
            iterator(int startIndex, Node* array, int size) : index(startIndex), nodeArray(array), arraySize(size)
            {
                while (index < arraySize && !nodeArray[index].isOccupied)
                {
                    index++;
                }
            }

            iterator& operator++()
            {
                do
                {
                    index++;
                } while (index < arraySize && !nodeArray[index].isOccupied);

                return *this;
            }

            bool operator!=(const iterator& other) const
            {
                return index != other.index;
            }

            std::pair<Key, Value> operator*() const
            {
                return {nodeArray[index].key, nodeArray[index].value};
            }
    };

    iterator begin()
    {
        return iterator(0, array, capacity);
    }

    iterator end()
    {
        return iterator(capacity, array, capacity);
    }
};

using HashMap2D = HashMap<string, string>;
using HashMap3D = HashMap<string, HashMap2D *>;

long fsize(int fd) {
    struct stat stat;
    int res = fstat(fd, &stat);
    return stat.st_size;
}

void insert_to_2D(HashMap2D *index, std::string v1, std::string v2) {
    if (index != nullptr) {
        index->insert(v1, v2);
    } else {
        throw std::runtime_error("HashMap2D pointer is nullptr");
    }
}

void insert_to_3D(HashMap3D *index, std::string v1, std::string v2, std::string v3) {
    if (index != nullptr) {
        std::pair<std::string, HashMap2D*>* kv = index->find(v1);
        HashMap2D *entry2 = nullptr;
        if (kv == nullptr) {
            auto newEntry2 = new HashMap2D();
            index->insert(v1, newEntry2);
            entry2 = newEntry2;
        } else {
            entry2 = kv->second;
        }

        insert_to_2D(entry2, v2, v3);
    } else {
        throw std::runtime_error("HashMap3D pointer is nullptr");
    }
}

void parse_two_column(int fd, HashMap2D *index) {
    int size = fsize(fd);
    char *data = (char *)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
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

void parse_three_column(int fd, HashMap3D *index, int order = 0) {
    int size = fsize(fd);
    char *data = (char *)mmap(0, size, PROT_READ, MAP_SHARED, fd, 0);
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
            // this is hardcoded for Store (where the second attribute is the
            // key)
            insert_to_3D(index, v2, v3, v1);
        } else if (order == 2) {
            // hardcoded for Load
            // insert_to_3D(index, v1, v3, v2); // for the first join order
            insert_to_3D(index, v3, v1, v2); // for the second join order
        }
    }
}

// void print_2D(HashMap2D *map) {
//     if (map != nullptr) {
//         for (int i = 0; i < map->getCapacity(); i++) {
//             auto node = (*map->array)[i];
//             while (node != nullptr) {
//                 std::cout << "(" << node->key << ", " << node->value << ")\n";
//                 node = node->next;
//             }
//         }
//     } else {
//         throw std::runtime_error("HashMap2D pointer is nullptr");
//     }
// }


// void print_3D(HashMap3D *map) {
//     if (map != nullptr) {
//         for (int i = 0; i < map->getCapacity(); i++) {
//             auto outerNode = (*map)[i];
//             while (outerNode != nullptr) {
//                 HashMap2D* innerMap = outerNode->value;
//                 for (int j = 0; j < innerMap->getCapacity(); j++) {
//                     auto innerNode = (*innerMap)[j];
//                     while (innerNode != nullptr) {
//                         std::cout << "(" << outerNode->key << ", " << innerNode->key << ", " << innerNode->value << ")\n";
//                         innerNode = innerNode->next;
//                     }
//                 }
//                 outerNode = outerNode->next;
//             }
//         }
//     } else {
//         throw std::runtime_error("HashMap3D pointer is nullptr");
//     }
// }

// bool is_new_2D(HashMap2D *data, string k,
//                string v) {
//     if (data->find(k) != nullptr) {
//         if ((*data)[k]->find(v) != (*data)[k]->end()) {
//             return false;
//         }
//     }
//     return true;
// }

// bool is_new_3D(
//     HashMap3D *data,
//     string k1, string k2, string v) {
//     if (data->find(k1) != data->end()) {
//         if ((*data)[k1]->find(v) != (*data)[k1]->end()) {
//             if ((*(*data)[k1])[k2]->find(v) != (*(*data)[k1])[k2]->end()) {
//                 return false;
//             }
//         }
//     }
//     return true;
// }



int main()
{
    auto vp = new HashMap2D();
    auto vp2 = new HashMap3D();

    // Add some key-value pairs to the hashmap.
    insert_to_2D(vp, "Apple", "Red");
    insert_to_2D(vp, "Banana", "Yellow");

    insert_to_3D(vp2, "Orchard", "Apple", "Red");

    // Fetch and print some values.
    std::cout << "Color of Apple: " << vp->find("Apple")->second << std::endl;
    std::cout << "Color of Banana: " << vp->find("Banana")->second << std::endl;
    
    std::cout << "Color of Apple: " << vp2->find("Orchard")->second->find("Apple")->second << std::endl;
    // return 0;
}

// int main() {
//     // Create a HashMap where each value is another HashMap
//     HashMap<std::string, HashMap<std::string, std::string>> vp;

//     // Create an inner HashMap
//     HashMap<std::string, std::string> innerMap;
//     innerMap.insert("inner_key1", "inner_value1");
//     innerMap.insert("inner_key2", "inner_value2");

//     // Add the inner HashMap to the outer HashMap
//     vp.insert("outer_key", innerMap);

//     // Accessing a value in the inner HashMap
//     std::string outerKey = "outer_key";
//     std::string innerKey = "inner_key1";
//     HashMap<std::string, std::string> retrievedInnerMap = *vp.find(outerKey);
//     if (!(*retrievedInnerMap.find(innerKey)).empty()) {
//         std::cout << "The value of the inner key is: " << *retrievedInnerMap.find(innerKey) << std::endl;
//     } else {
//         std::cout << "The inner key does not exist in the outer key's map." << std::endl;
//     }

//     return 0;
// }
