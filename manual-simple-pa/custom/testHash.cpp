// #include <fcntl.h>
// #include <sys/mman.h>
// #include <sys/stat.h>

// #include <fstream>

#include <string>
#include <cstdlib>
#include <iostream>

using namespace std;

// template <typename Key, typename Value>
// class HashMap
// {
// private:
//     // Initial capacity of the hash map.
//     int INITIAL_CAPACITY = 1024;

//     // Load factor that determines when to resize the map.
//     double LOAD_FACTOR = 0.75;

//     // Node structure: each node includes a key, a value, and a boolean that represents if the node is occupied.
//     struct Node
//     {
//         Key key;
//         Value value;
//         bool isOccupied;
//     };

//     // Capacity represents the size of the current array.
//     int capacity;

//     // Size represents the current total of stored keys.
//     int size;

//     // The array of Nodes.
//     Node *array;

//     // Hash function to map a key to an index in the array.
//     int hash(Key key)
//     {
//         return std::hash<Key>{}(key) % capacity;
//     }

//     // Function to resize the hash map when the load factor exceeds a certain threshold.
//     // Creates a new array with double the size, and rehashes all keys.
//     void resize()
//     {
//         int oldCapacity = capacity;
//         Node *oldArray = array;
//         capacity *= 2;
//         size = 0;
//         array = new Node[capacity];
//         for (int i = 0; i < capacity; i++)
//         {
//             array[i].isOccupied = false;
//         }
//         for (int i = 0; i < oldCapacity; i++)
//         {
//             if (oldArray[i].isOccupied)
//             {
//                 insert(oldArray[i].key, oldArray[i].value);
//             }
//         }
//         delete[] oldArray;
//     }

// public:
//     // Constructor for the hash map. Initializes the array and sets the initial capacity.
//     HashMap() : capacity(INITIAL_CAPACITY), size(0)
//     {
//         array = new Node[capacity];
//         for (int i = 0; i < capacity; i++)
//         {
//             array[i].isOccupied = false;
//         }
//     }



//     // Copy constructor: creates a deep copy of the HashMap.
//     HashMap(const HashMap<Key, Value>& src) : capacity(src.capacity), size(src.size), INITIAL_CAPACITY(src.INITIAL_CAPACITY), LOAD_FACTOR(src.LOAD_FACTOR) {
//         array = new Node*[capacity];
//         for (int i = 0; i < capacity; i++) {
//             if (src.array[i] != nullptr) {
//                 array[i] = new Node{src.array[i]->key, src.array[i]->value, nullptr};
//                 Node* srcNode = src.array[i]->next;
//                 Node* newNode = array[i];
//                 while (srcNode != nullptr) {
//                     newNode->next = new Node{srcNode->key, srcNode->value, nullptr};
//                     srcNode = srcNode->next;
//                     newNode = newNode->next;
//                 }
//             } else {
//                 array[i] = nullptr;
//             }
//         }
//     }

//     // Overloaded assignment operator: creates a deep copy of the HashMap.
//     HashMap<Key, Value>& operator=(const HashMap<Key, Value>& src) {
//         if (this == &src) {
//             return *this;
//         }

//         // Free the existing array
//         for (int i = 0; i < capacity; i++) {
//             delete array[i];
//         }
//         delete[] array;

//         // Copy the source HashMap
//         capacity = src.capacity;
//         size = src.size;
//         INITIAL_CAPACITY = src.INITIAL_CAPACITY;
//         LOAD_FACTOR = src.LOAD_FACTOR;
//         array = new Node*[capacity];
//         for (int i = 0; i < capacity; i++) {
//             if (src.array[i] != nullptr) {
//                 array[i] = new Node{src.array[i]->key, src.array[i]->value, nullptr};
//                 Node* srcNode = src.array[i]->next;
//                 Node* newNode = array[i];
//                 while (srcNode != nullptr) {
//                     newNode->next = new Node{srcNode->key, srcNode->value, nullptr};
//                     srcNode = srcNode->next;
//                     newNode = newNode->next;
//                 }
//             } else {
//                 array[i] = nullptr;
//             }
//         }
//         return *this;
//     }

//     // Destructor for the hash map. Deletes all allocated memory.
//     ~HashMap()
//     {
//         delete[] array;
//     }

//     // Function to insert a key-value pair into the hash map using linear probing.
//     void insert(Key key, Value value)
//     {
//         int index = hash(key);
//         while (array[index].isOccupied && array[index].key != key)
//         {
//             index = (index + 1) % capacity;
//         }
//         if (!array[index].isOccupied)
//         {
//             size++;
//         }
//         array[index].key = key;
//         array[index].value = value;
//         array[index].isOccupied = true;

//         // If the load factor exceeds the threshold, resize the hash map.
//         if ((double)size / capacity > LOAD_FACTOR)
//         {
//             resize();
//         }
//     }

//     // Function to find a key in the hash map and return its corresponding key-value pair.
//     std::pair<Key, Value>* find(Key key)
//     {
//         int index = hash(key);
//         while (array[index].isOccupied)
//         {
//             if (array[index].key == key)
//             {
//                 return new std::pair<Key, Value>(array[index].key, array[index].value);
//             }
//             index = (index + 1) % capacity;
//         }
//         return nullptr; // Return nullptr if key does not exist.
//     }
// };

// void insert_to_2D(HashMap<string, string>& index, string v1, string v2) {
//     index.insert(v1, v2);
// }



// int main()
// {
//     HashMap<string, string> vp;

//     // Add some key-value pairs to the hashmap.
//     insert_to_2D(vp, "Apple", "Red");
//     // vp.insert("Banana", "Yellow");
//     // vp.insert("Blueberry", "Blue");

//     // Fetch and print some values.
//     std::cout << "Color of Apple: " << vp.find("Apple")->second << std::endl;
//     std::cout << "Color of Banana: " << vp.find("Banana")->second << std::endl;
//     // std::cout << "Color of Blueberry: " << vp.find("Blueberry") << std::endl;

//     // // Try to fetch a value for a non-existent key.
//     // std::cout << "Color of Mango: " << vp.find("Mango") << std::endl;

//     // return 0;
// }

// // int main() {
// //     // Create a HashMap where each value is another HashMap
// //     HashMap<std::string, HashMap<std::string, std::string>> vp;

// //     // Create an inner HashMap
// //     HashMap<std::string, std::string> innerMap;
// //     innerMap.insert("inner_key1", "inner_value1");
// //     innerMap.insert("inner_key2", "inner_value2");

// //     // Add the inner HashMap to the outer HashMap
// //     vp.insert("outer_key", innerMap);

// //     // Accessing a value in the inner HashMap
// //     std::string outerKey = "outer_key";
// //     std::string innerKey = "inner_key1";
// //     HashMap<std::string, std::string> retrievedInnerMap = *vp.find(outerKey);
// //     if (!(*retrievedInnerMap.find(innerKey)).empty()) {
// //         std::cout << "The value of the inner key is: " << *retrievedInnerMap.find(innerKey) << std::endl;
// //     } else {
// //         std::cout << "The inner key does not exist in the outer key's map." << std::endl;
// //     }

// //     return 0;
// // }

template <typename Key, typename Value>
class HashMap {
private:
    int INITIAL_CAPACITY = 1024;
    double LOAD_FACTOR = 0.75;

    struct Node {
        Key key;
        Value value;
        bool isOccupied;
    };

    int capacity;
    int size;
    Node *array;

    int hash(Key key) {
        return std::hash<Key>{}(key) % capacity;
    }

    void resize() {
        int oldCapacity = capacity;
        Node *oldArray = array;
        capacity *= 2;
        size = 0;
        array = new Node[capacity];
        for (int i = 0; i < capacity; i++) {
            array[i].isOccupied = false;
        }
        for (int i = 0; i < oldCapacity; i++) {
            if (oldArray[i].isOccupied) {
                insert(oldArray[i].key, oldArray[i].value);
            }
        }
        delete[] oldArray;
    }

public:
    HashMap() : capacity(INITIAL_CAPACITY), size(0) {
        array = new Node[capacity];
        for (int i = 0; i < capacity; i++) {
            array[i].isOccupied = false;
        }
    }

    ~HashMap() {
        delete[] array;
    }

    void insert(Key key, Value value) {
        int index = hash(key);
        while (array[index].isOccupied && array[index].key != key) {
            index = (index + 1) % capacity;
        }
        if (!array[index].isOccupied) {
            size++;
        }
        array[index].key = key;
        array[index].value = value;
        array[index].isOccupied = true;

        if ((double)size / capacity > LOAD_FACTOR) {
            resize();
        }
    }

    std::pair<Key, Value>* find(Key key) {
        int index = hash(key);
        while (array[index].isOccupied) {
            if (array[index].key == key) {
                return new std::pair<Key, Value>(array[index].key, array[index].value);
            }
            index = (index + 1) % capacity;
        }
        return nullptr;
    }

    int getCapacity() {
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

int main() {
    HashMap<std::string, int> map;
map.insert("apple", 1);
map.insert("banana", 2);
map.insert("cherry", 3);

std::cout << "Color of Apple: " << map.find("apple")->second << std::endl;

for(auto it: map)
{
    std::cout << it.first << " -> " << it.second << '\n';
}

}