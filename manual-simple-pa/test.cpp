#include <iostream>
#include <string>
#include <absl/container/flat_hash_map.h>

int main() {
  absl::flat_hash_map<int, std::string> myMap;
  
  // Insert some key-value pairs
  myMap.emplace(1, "One");
  myMap.emplace(2, "Two");
  myMap.emplace(3, "Three");
  
  // Access and print values
  std::cout << "Value for key 1: " << myMap[1] << std::endl;
  std::cout << "Value for key 2: " << myMap[2] << std::endl;
  std::cout << "Value for key 3: " << myMap[3] << std::endl;
  
  // Check if a key exists
  if (myMap.contains(4)) {
    std::cout << "Key 4 exists in the map." << std::endl;
  } else {
    std::cout << "Key 4 does not exist in the map." << std::endl;
  }
  
  return 0;
}