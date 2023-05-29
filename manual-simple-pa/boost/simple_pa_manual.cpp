#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <fstream>
#include <iostream>
#include <string>
// #include <unordered_map>
#include <boost/unordered_map.hpp>

#include <set>

using namespace std;

long fsize(int fd) {
    struct stat stat;
    int res = fstat(fd, &stat);
    return stat.st_size;
}

void insert_to_2D(boost::unordered_map<string, set<string> *> *index,
                  string v1, string v2) {
    auto kv = index->find(v1);
    set<string> *entry2 = nullptr;
    if (kv == index->end()) {
        // new key
        auto newEntry2 = new set<string>();
        (*index)[v1] = newEntry2;
        entry2 = newEntry2;
    } else {
        entry2 = kv->second;
    }

    entry2->insert(v2);
}

void insert_to_3D(
    boost::unordered_map<string, boost::unordered_map<string, set<string> *> *>
        *index,
    string v1, string v2, string v3) {
    auto kv = index->find(v1);
    boost::unordered_map<string, set<string> *> *entry2 = nullptr;
    if (kv == index->end()) {
        auto newEntry2 = new boost::unordered_map<string, set<string> *>();
        (*index)[v1] = newEntry2;
        entry2 = newEntry2;
    } else {
        entry2 = kv->second;
    }

    insert_to_2D(entry2, v2, v3);
}

void parse_two_column(int fd,
                      boost::unordered_map<string, set<string> *> *index) {
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

void parse_three_column(
    int fd,
    boost::unordered_map<string, boost::unordered_map<string, set<string> *> *>
        *index,
    int order = 0) {
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

void print_2D(boost::unordered_map<string, set<string> *> *data) {
    for (auto kv : *data) {
        auto key = kv.first;
        for (auto value : *kv.second) {
            cout << "(" << key << ", " << value << ")\n";
        }
    }
}

void print_3D(
    boost::unordered_map<string, boost::unordered_map<string, set<string> *> *>
        *data) {
    for (auto kkv : *data) {
        auto k1 = kkv.first;
        for (auto kv : *kkv.second) {
            auto k2 = kv.first;
            for (auto v : *kv.second) {
                cout << "(" << k1 << ", " << k2 << ", " << v << ")\n";
            }
        }
    }
}

bool is_new_2D(boost::unordered_map<string, set<string> *> *data, string k,
               string v) {
    if (data->find(k) != data->end()) {
        if ((*data)[k]->find(v) != (*data)[k]->end()) {
            return false;
        }
    }
    return true;
}

bool is_new_3D(
    boost::unordered_map<string, boost::unordered_map<string, set<string> *> *>
        *data,
    string k1, string k2, string v) {
    if (data->find(k1) != data->end()) {
        if ((*data)[k1]->find(v) != (*data)[k1]->end()) {
            if ((*(*data)[k1])[k2]->find(v) != (*(*data)[k1])[k2]->end()) {
                return false;
            }
        }
    }
    return true;
}

// TODO: Can we do better?
void update_2D(boost::unordered_map<string, set<string> *> *data,
               boost::unordered_map<string, set<string> *> *new_data) {
    for (auto kv : *new_data) {
        auto k = kv.first;
        if (data->find(k) == data->end()) {
            (*data)[k] = new set<string>();
        }
        for (auto v : *kv.second) {
            (*data)[k]->insert(v);
        }
    }
}

void free_2D(boost::unordered_map<string, set<string> *> *data) {
    // deep free
    for (auto &kv : *data) {
        kv.second->clear();
    }
    data->clear();
}

int size_2D(boost::unordered_map<string, set<string> *> *data) {
    int count = 0;
    for (auto &kv: *data) {
        count += kv.second->size();
    }
    return count;
}

void save_to_file_2D(boost::unordered_map<string, set<string> *> *data,
                     string fname) {
    ofstream file;
    file.open(fname);

    // FILE *fptr = fopen(fname, "w");
    // for(auto &kv: *data) {
    //     auto k = kv.first;
    //     for(auto &v: *kv.second) {
    //         fprintf(fptr, "%s %s\n", k, v);
    //     }
    // }
    for (auto &kv : *data) {
        auto k = kv.first;
        for (auto &v : *kv.second) {
            file << k << "\t" << v << "\n";
        }
    }
    file.close();
}

int main() {
    // indexes to store data
    auto alloc = new boost::unordered_map<string, set<string> *>();
    auto assign = new boost::unordered_map<
        string, set<string> *>();  // we directly load PrimitiveLoad
                                             // into Assign
    auto load =
        new boost::unordered_map<string,
                          boost::unordered_map<string, set<string> *> *>();
    // Store is indexed in this order: base -> field -> source (this order
    // because base is the join key)
    auto store =
        new boost::unordered_map<string,
                          boost::unordered_map<string, set<string> *> *>();

    // varPointsTo(var:Variable, heap:Allocation)
    auto vp = new boost::unordered_map<string, set<string> *>();
    auto vp_delta = new boost::unordered_map<string, set<string> *>();
    auto vp_new = new boost::unordered_map<string, set<string> *>();

    // varPointsTo indexed by heap (required for recursice rule1)
    auto vp2 = new boost::unordered_map<string, set<string> *>();
    auto vp2_delta = new boost::unordered_map<string, set<string> *>();
    auto vp2_new = new boost::unordered_map<string, set<string> *>();

    // alias(x:Variable,y:Variable)
    auto alias = new boost::unordered_map<string, set<string> *>();
    auto alias_delta = new boost::unordered_map<string, set<string> *>();
    auto alias_new = new boost::unordered_map<string, set<string> *>();

    // Assign(source:Variable, destination:Variable)
    auto assign_delta =
        new boost::unordered_map<string, set<string> *>;
    auto assign_new = new boost::unordered_map<string, set<string> *>();

    // parse the data and load into Extentional relations
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

    // prcoess base rules
    // Add to both full and delta
    // VarPointsTo(var, heap) :- AssignAlloc(var, heap).
    for (auto &alloc_item : *alloc) {
        auto var = alloc_item.first;
        for (auto &obj : *alloc_item.second) {
            // vp index1
            insert_to_2D(vp, var, obj);
            insert_to_2D(vp_delta, var, obj);

            // vp index2
            insert_to_2D(vp2, obj, var);
            insert_to_2D(vp2_delta, obj, var);
        }
    }

    // recursive relations; fixed point loop
    int iter = 0;
    while (vp_delta->size() != 0 || alias_delta->size() != 0 ||
           assign_delta->size() != 0) {
        cout << "-------------iter " << iter << "-------------\n";
        cout << "vp_delta size = " << size_2D(vp_delta) << endl;
        cout << "alias_delta size = " << size_2D(alias_delta) << endl;
        cout << "assign_delta size = " << size_2D(assign_delta) << endl;
        cout << "-------------------------------\n";
        cout << "vp size = " << size_2D(vp) << endl;
        cout << "alias size = " << size_2D(alias) << endl;
        cout << "assign size = " << size_2D(assign) << endl;
        cout << "-------------------------------\n";
        iter++;
        /*
        Assign(var1, var2) :-
        Store(var1, instanceVar2, field),
        Alias(instanceVar2, instanceVar1),
        Load(instanceVar1, var2, field).
        */
        // for (auto &alias_kv : *alias_delta) {
        //     auto instanceVar2 = alias_kv.first;
        //     for (auto &instanceVar1 : *alias_kv.second) {
        //         auto store_iter = store->find(instanceVar2) != store->end()
        //                               ? (*store)[instanceVar2]
        //                               : nullptr;
        //         auto load_kvs = load->find(instanceVar1) != load->end()
        //                             ? (*load)[instanceVar1]
        //                             : nullptr;

        //         if (store_iter != nullptr && load_kvs != nullptr) {
        //             for (auto &store_kv : *store_iter) {
        //                 auto field = store_kv.first;

        //                 // iterate value pairs
        //                 for (auto &var1 : *store_kv.second) {
        //                     if (load_kvs->find(field) != load_kvs->end()) {
        //                         for (auto &var2 : *(*load_kvs)[field]) {
        //                             if (is_new_2D(assign, var1, var2)) {
        //                                 insert_to_2D(assign_new, var1, var2);
        //                             }
        //                         }
        //                     }
        //                 }
        //             }
        //         }
        //     }
        // }
        if (alias_delta->size() != 0) {
            // store is stored as instanceVar2, field, var1
            // load is stored as field, instanceVar1, var2
            for(auto &store_kkv: *store) {
                auto instanceVar2 = store_kkv.first;
                auto alias_iter = alias_delta->find(instanceVar2);
                if (alias_iter != alias_delta->end()) {
                    for(auto &store_kv: *store_kkv.second) {
                        auto field = store_kv.first;
                        auto load_iter = load->find(field);
                        if (load_iter != load->end()) {
                            for(auto &load_kv: *(load_iter->second)) {
                                auto instanceVar1 = load_kv.first;
                                if ((alias_iter->second)->find(instanceVar1) !=
                                    (alias_iter->second)->end()) {
                                        // means, var1 and var2 satisfies
                                        for(auto &var1: *store_kv.second) {
                                            for(auto &var2: *load_kv.second) {
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
            }
        }

        /*
       VarPointsTo(var1, heap) :-
        Assign(var2, var1),
        VarPointsTo(var2, heap).
       */
        // delta, stable
        // delta, delta
        for (auto &assign_kv : *assign_delta) {
            auto var2 = assign_kv.first;
            auto heap_iter =
                vp->find(var2) != vp->end() ? (*vp)[var2] : nullptr;
            auto delta_heap_iter = vp_delta->find(var2) != vp_delta->end()
                                       ? (*vp_delta)[var2]
                                       : nullptr;

            if (heap_iter != nullptr || delta_heap_iter != nullptr) {
                for (auto &var1 : *assign_kv.second) {
                    // delta, stable
                    if (heap_iter != nullptr) {
                        for (auto &heap : *heap_iter) {
                            if (is_new_2D(vp, var1, heap)) {
                                insert_to_2D(vp_new, var1, heap);
                                insert_to_2D(vp2_new, heap, var1);
                            }
                        }
                    }

                    // delta, delta
                    if (delta_heap_iter != nullptr) {
                        for (auto &heap : *delta_heap_iter) {
                            if (is_new_2D(vp, var1, heap)) {
                                insert_to_2D(vp_new, var1, heap);
                                insert_to_2D(vp2_new, heap, var1);
                            }
                        }
                    }
                }
            }
        }

        // stable, delta
        if (vp_delta->size() > 0) {
            for (auto &assign_kv : *assign) {
                auto var2 = assign_kv.first;
                auto delta_heap_iter = vp_delta->find(var2) != vp_delta->end()
                                           ? (*vp_delta)[var2]
                                           : nullptr;
                if (delta_heap_iter != nullptr) {
                    for (auto &var1 : *assign_kv.second) {
                        for (auto &heap : *delta_heap_iter) {
                            if (is_new_2D(vp, var1, heap)) {
                                insert_to_2D(vp_new, var1, heap);
                                insert_to_2D(vp2_new, heap, var1);
                            }
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
        for (auto &vp_delta_kv : *vp_delta) {
            auto instanceVar = vp_delta_kv.first;
            for (auto &instanceHeap : *vp_delta_kv.second) {
                // use index2
                // stable
                if (vp2->find(instanceHeap) != vp2->end()) {
                    for (auto &iVar : *(*vp2)[instanceHeap]) {
                        if (is_new_2D(alias, instanceVar, iVar)) {
                            insert_to_2D(alias_new, instanceVar, iVar);
                        }
                    }
                }

                // delta
                if (vp2_delta->find(instanceHeap) != vp2_delta->end()) {
                    for (auto &iVar : *(*vp2_delta)[instanceHeap]) {
                        if (is_new_2D(alias, instanceVar, iVar)) {
                            insert_to_2D(alias_new, instanceVar, iVar);
                        }
                    }
                }
            }
        }

        // stable, delta
        if (vp_delta->size() > 0) {
            for (auto &vp_kv : *vp) {
                auto instanceVar = vp_kv.first;
                for (auto &instanceHeap : *vp_kv.second) {
                    // delta
                    if (vp2_delta->find(instanceHeap) != vp2_delta->end()) {
                        for (auto &iVar : *(*vp2_delta)[instanceHeap]) {
                            if (is_new_2D(alias, instanceVar, iVar)) {
                                insert_to_2D(alias_new, instanceVar, iVar);
                            }
                        }
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

        // new <- []
        vp_new = new boost::unordered_map<string, set<string> *>();
        vp2_new = new boost::unordered_map<string, set<string> *>();
        alias_new = new boost::unordered_map<string, set<string> *>();
        assign_new = new boost::unordered_map<string, set<string> *>();
        cout << "Iteration Done\n";
    }
    // write the output to files
    save_to_file_2D(vp, "VarPointsTo.csv");
    save_to_file_2D(alias, "Alias.csv");
    save_to_file_2D(assign, "Assign.csv");
}