#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <string>
#include <boost/unordered_map.hpp>

using namespace std;

long fsize(int fd) {
    struct stat stat;
    int res = fstat(fd, &stat);
    return stat.st_size;
}

void insert_to_2D(boost::unordered_map<string, string>& index, string v1, string v2) {
    auto kv = index.find(v1);
    if (kv == index.end()) {
        // new key
        index[v1] = v2;
    } else {
        // linear probing
        while (kv != index.end()) {
            if (kv->first == v1) {
                kv->second = v2;
                break;
            }
            ++kv;
        }
        if (kv == index.end()) {
            index[v1] = v2;
        }
    }
}

void insert_to_3D(boost::unordered_map<string, boost::unordered_map<string, string>>& index, string v1, string v2, string v3) {
    auto kv = index.find(v1);
    if (kv == index.end()) {
        index[v1] = boost::unordered_map<string, string>();
        index[v1][v2] = v3;
    } else {
        insert_to_2D(kv->second, v2, v3);
    }
}

void parse_two_column(int fd, boost::unordered_map<string, string>& index) {
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

void parse_three_column(int fd, boost::unordered_map<string, boost::unordered_map<string, string>>& index, int order = 0) {
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

void print_2D(boost::unordered_map<string, string>& data) {
    for (auto& kv : data) {
        auto key = kv.first;
        auto value = kv.second;
        cout << "(" << key << ", " << value << ")\n";
    }
}

void print_3D(boost::unordered_map<string, boost::unordered_map<string, string>>& data) {
    for (auto& kkv : data) {
        auto k1 = kkv.first;
        for (auto& kv : kkv.second) {
            auto k2 = kv.first;
            auto v = kv.second;
            cout << "(" << k1 << ", " << k2 << ", " << v << ")\n";
        }
    }
}

bool is_new_2D(boost::unordered_map<string, string>& data, string k, string v) {
    auto kv = data.find(k);
    if (kv != data.end()) {
        return kv->second != v;
    }
    return true;
}

bool is_new_3D(boost::unordered_map<string, boost::unordered_map<string, string>>& data, string k1, string k2, string v) {
    auto kv = data.find(k1);
    if (kv != data.end()) {
        auto& innerMap = kv->second;
        auto innerKv = innerMap.find(k2);
        if (innerKv != innerMap.end()) {
            return innerKv->second != v;
        }
    }
    return true;
}

void update_2D(boost::unordered_map<string, string>& data, boost::unordered_map<string, string>& new_data) {
    for (auto& kv : new_data) {
        auto k = kv.first;
        auto v = kv.second;
        data[k] = v;
    }
}

void free_2D(boost::unordered_map<string, string>& data) {
    data.clear();
}

int size_2D(boost::unordered_map<string, string>& data) {
    return data.size();
}

void save_to_file_2D(boost::unordered_map<string, string>& data, string fname) {
    ofstream file;
    file.open(fname);
    for (auto& kv : data) {
        auto k = kv.first;
        auto v = kv.second;
        file << k << "\t" << v << "\n";
    }
    file.close();
}

int main() {
    // start timer
    auto start = std::chrono::high_resolution_clock::now();
    auto load_factor = 0.5;

    // indexes to store data
    boost::unordered_map<string, string> alloc; alloc.load_factor(load_factor);
    boost::unordered_map<string, string> assign; assign.load_factor(load_factor);
    boost::unordered_map<string, boost::unordered_map<string, string>> load; load.load_factor(load_factor);
    boost::unordered_map<string, boost::unordered_map<string, string>> store; store.load_factor(load_factor);

    // varPointsTo(var:Variable, heap:Allocation)
    boost::unordered_map<string, string> vp; vp.load_factor(load_factor);
    boost::unordered_map<string, string> vp_delta; vp_delta.load_factor(load_factor);
    boost::unordered_map<string, string> vp_new; vp_new.load_factor(load_factor);

    // varPointsTo indexed by heap (required for recursive rule1)
    boost::unordered_map<string, string> vp2; vp2.load_factor(load_factor);
    boost::unordered_map<string, string> vp2_delta; vp2_delta.load_factor(load_factor);
    boost::unordered_map<string, string> vp2_new; vp2_new.load_factor(load_factor);

    // alias(x:Variable,y:Variable)
    boost::unordered_map<string, string> alias; alias.load_factor(load_factor);
    boost::unordered_map<string, string> alias_delta; alias_delta.load_factor(load_factor);
    boost::unordered_map<string, string> alias_new; alias_new.load_factor(load_factor);

    // Assign(source:Variable, destination:Variable)
    boost::unordered_map<string, string> assign_delta; assign_delta.load_factor(load_factor);
    boost::unordered_map<string, string> assign_new; assign_new.load_factor(load_factor);

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
    for (auto& alloc_item : alloc) {
        auto var = alloc_item.first;
        auto heap = alloc_item.second;
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
            for (auto& store_kkv : store) {
                auto instanceVar2 = store_kkv.first;
                auto alias_iter = alias_delta.find(instanceVar2);
                if (alias_iter != alias_delta.end()) {
                    for (auto& store_kv : store_kkv.second) {
                        auto field = store_kv.first;
                        auto load_iter = load.find(field);
                        if (load_iter != load.end()) {
                            for (auto& load_kv : load_iter->second) {
                                auto instanceVar1 = load_kv.first;
                                if (alias_iter->second == instanceVar1) {
                                    // means, var1 and var2 satisfy the conditions
                                    auto var1 = store_kv.second;
                                    auto var2 = load_kv.second;
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
        for (auto& assign_kv : assign_delta) {
            auto var2 = assign_kv.first;
            auto heap = vp[var2];
            auto delta_heap = vp_delta[var2];

            if (!heap.empty() || !delta_heap.empty()) {
                for (auto& var1 : assign_kv.second) {
                    // delta, stable
                    if (!heap.empty()) {
                        if (is_new_2D(vp, var1, heap)) {
                            insert_to_2D(vp_new, var1, heap);
                            insert_to_2D(vp2_new, heap, var1);
                        }
                    }

                    // delta, delta
                    if (!delta_heap.empty()) {
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
            for (auto& assign_kv : assign) {
                auto var2 = assign_kv.first;
                auto delta_heap = vp_delta[var2];
                if (!delta_heap.empty()) {
                    for (auto& var1 : assign_kv.second) {
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
        for (auto& vp_delta_kv : vp_delta) {
            auto instanceVar = vp_delta_kv.first;
            auto instanceHeap = vp_delta_kv.second;
            // use index2
            // stable
            auto iVar = vp2[instanceHeap];
            auto delta_iVar = vp2_delta[instanceHeap];
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
            for (auto& vp_kv : vp) {
                auto instanceVar = vp_kv.first;
                auto instanceHeap = vp_kv.second;
                // delta
                auto delta_iVar = vp2_delta[instanceHeap];
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

    // write the output to files
    save_to_file_2D(vp, "VarPointsTo.csv");
    save_to_file_2D(alias, "Alias.csv");
    save_to_file_2D(assign, "Assign.csv");

    // end timer
    auto end = chrono::steady_clock::now();
    cout << "Time elapsed: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;
}
