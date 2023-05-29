
#include "souffle/CompiledSouffle.h"

extern "C" {}

namespace souffle {
static const RamDomain RAM_BIT_SHIFT_MASK = RAM_DOMAIN_SIZE - 1;
struct t_btree_ii__0_1__11__10 {
    using t_tuple = Tuple<RamDomain, 2>;
    struct t_comparator_0 {
        int operator()(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]))
                       ? -1
                   : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0]))
                       ? 1
                       : ((ramBitCast<RamSigned>(a[1]) <
                           ramBitCast<RamSigned>(b[1]))
                              ? -1
                          : (ramBitCast<RamSigned>(a[1]) >
                             ramBitCast<RamSigned>(b[1]))
                              ? 1
                              : (0));
        }
        bool less(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) <
                    ramBitCast<RamSigned>(b[0])) ||
                   (ramBitCast<RamSigned>(a[0]) ==
                    ramBitCast<RamSigned>(b[0])) &&
                       ((ramBitCast<RamSigned>(a[1]) <
                         ramBitCast<RamSigned>(b[1])));
        }
        bool equal(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) ==
                    ramBitCast<RamSigned>(b[0])) &&
                   (ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1]));
        }
    };
    using t_ind_0 = btree_set<t_tuple, t_comparator_0>;
    t_ind_0 ind_0;
    using iterator = t_ind_0::iterator;
    struct context {
        t_ind_0::operation_hints hints_0_lower;
        t_ind_0::operation_hints hints_0_upper;
    };
    context createContext() { return context(); }
    bool insert(const t_tuple& t) {
        context h;
        return insert(t, h);
    }
    bool insert(const t_tuple& t, context& h) {
        if (ind_0.insert(t, h.hints_0_lower)) {
            return true;
        } else
            return false;
    }
    bool insert(const RamDomain* ramDomain) {
        RamDomain data[2];
        std::copy(ramDomain, ramDomain + 2, data);
        const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
        context h;
        return insert(tuple, h);
    }
    bool insert(RamDomain a0, RamDomain a1) {
        RamDomain data[2] = {a0, a1};
        return insert(data);
    }
    bool contains(const t_tuple& t, context& h) const {
        return ind_0.contains(t, h.hints_0_lower);
    }
    bool contains(const t_tuple& t) const {
        context h;
        return contains(t, h);
    }
    std::size_t size() const { return ind_0.size(); }
    iterator find(const t_tuple& t, context& h) const {
        return ind_0.find(t, h.hints_0_lower);
    }
    iterator find(const t_tuple& t) const {
        context h;
        return find(t, h);
    }
    range<iterator> lowerUpperRange_00(const t_tuple& /* lower */,
                                       const t_tuple& /* upper */,
                                       context& /* h */) const {
        return range<iterator>(ind_0.begin(), ind_0.end());
    }
    range<iterator> lowerUpperRange_00(const t_tuple& /* lower */,
                                       const t_tuple& /* upper */) const {
        return range<iterator>(ind_0.begin(), ind_0.end());
    }
    range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower,
                                                const t_tuple& upper,
                                                context& h) const {
        t_comparator_0 comparator;
        int cmp = comparator(lower, upper);
        if (cmp == 0) {
            auto pos = ind_0.find(lower, h.hints_0_lower);
            auto fin = ind_0.end();
            if (pos != fin) {
                fin = pos;
                ++fin;
            }
            return make_range(pos, fin);
        }
        if (cmp > 0) {
            return make_range(ind_0.end(), ind_0.end());
        }
        return make_range(ind_0.lower_bound(lower, h.hints_0_lower),
                          ind_0.upper_bound(upper, h.hints_0_upper));
    }
    range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower,
                                                const t_tuple& upper) const {
        context h;
        return lowerUpperRange_11(lower, upper, h);
    }
    range<t_ind_0::iterator> lowerUpperRange_10(const t_tuple& lower,
                                                const t_tuple& upper,
                                                context& h) const {
        t_comparator_0 comparator;
        int cmp = comparator(lower, upper);
        if (cmp > 0) {
            return make_range(ind_0.end(), ind_0.end());
        }
        return make_range(ind_0.lower_bound(lower, h.hints_0_lower),
                          ind_0.upper_bound(upper, h.hints_0_upper));
    }
    range<t_ind_0::iterator> lowerUpperRange_10(const t_tuple& lower,
                                                const t_tuple& upper) const {
        context h;
        return lowerUpperRange_10(lower, upper, h);
    }
    bool empty() const { return ind_0.empty(); }
    std::vector<range<iterator>> partition() const {
        return ind_0.getChunks(400);
    }
    void purge() { ind_0.clear(); }
    iterator begin() const { return ind_0.begin(); }
    iterator end() const { return ind_0.end(); }
    void printStatistics(std::ostream& o) const {
        o << " arity 2 direct b-tree index 0 lex-order [0,1]\n";
        ind_0.printStats(o);
    }
};
struct t_btree_ii__0_1__11 {
    using t_tuple = Tuple<RamDomain, 2>;
    struct t_comparator_0 {
        int operator()(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]))
                       ? -1
                   : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0]))
                       ? 1
                       : ((ramBitCast<RamSigned>(a[1]) <
                           ramBitCast<RamSigned>(b[1]))
                              ? -1
                          : (ramBitCast<RamSigned>(a[1]) >
                             ramBitCast<RamSigned>(b[1]))
                              ? 1
                              : (0));
        }
        bool less(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) <
                    ramBitCast<RamSigned>(b[0])) ||
                   (ramBitCast<RamSigned>(a[0]) ==
                    ramBitCast<RamSigned>(b[0])) &&
                       ((ramBitCast<RamSigned>(a[1]) <
                         ramBitCast<RamSigned>(b[1])));
        }
        bool equal(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) ==
                    ramBitCast<RamSigned>(b[0])) &&
                   (ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1]));
        }
    };
    using t_ind_0 = btree_set<t_tuple, t_comparator_0>;
    t_ind_0 ind_0;
    using iterator = t_ind_0::iterator;
    struct context {
        t_ind_0::operation_hints hints_0_lower;
        t_ind_0::operation_hints hints_0_upper;
    };
    context createContext() { return context(); }
    bool insert(const t_tuple& t) {
        context h;
        return insert(t, h);
    }
    bool insert(const t_tuple& t, context& h) {
        if (ind_0.insert(t, h.hints_0_lower)) {
            return true;
        } else
            return false;
    }
    bool insert(const RamDomain* ramDomain) {
        RamDomain data[2];
        std::copy(ramDomain, ramDomain + 2, data);
        const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
        context h;
        return insert(tuple, h);
    }
    bool insert(RamDomain a0, RamDomain a1) {
        RamDomain data[2] = {a0, a1};
        return insert(data);
    }
    bool contains(const t_tuple& t, context& h) const {
        return ind_0.contains(t, h.hints_0_lower);
    }
    bool contains(const t_tuple& t) const {
        context h;
        return contains(t, h);
    }
    std::size_t size() const { return ind_0.size(); }
    iterator find(const t_tuple& t, context& h) const {
        return ind_0.find(t, h.hints_0_lower);
    }
    iterator find(const t_tuple& t) const {
        context h;
        return find(t, h);
    }
    range<iterator> lowerUpperRange_00(const t_tuple& /* lower */,
                                       const t_tuple& /* upper */,
                                       context& /* h */) const {
        return range<iterator>(ind_0.begin(), ind_0.end());
    }
    range<iterator> lowerUpperRange_00(const t_tuple& /* lower */,
                                       const t_tuple& /* upper */) const {
        return range<iterator>(ind_0.begin(), ind_0.end());
    }
    range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower,
                                                const t_tuple& upper,
                                                context& h) const {
        t_comparator_0 comparator;
        int cmp = comparator(lower, upper);
        if (cmp == 0) {
            auto pos = ind_0.find(lower, h.hints_0_lower);
            auto fin = ind_0.end();
            if (pos != fin) {
                fin = pos;
                ++fin;
            }
            return make_range(pos, fin);
        }
        if (cmp > 0) {
            return make_range(ind_0.end(), ind_0.end());
        }
        return make_range(ind_0.lower_bound(lower, h.hints_0_lower),
                          ind_0.upper_bound(upper, h.hints_0_upper));
    }
    range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower,
                                                const t_tuple& upper) const {
        context h;
        return lowerUpperRange_11(lower, upper, h);
    }
    bool empty() const { return ind_0.empty(); }
    std::vector<range<iterator>> partition() const {
        return ind_0.getChunks(400);
    }
    void purge() { ind_0.clear(); }
    iterator begin() const { return ind_0.begin(); }
    iterator end() const { return ind_0.end(); }
    void printStatistics(std::ostream& o) const {
        o << " arity 2 direct b-tree index 0 lex-order [0,1]\n";
        ind_0.printStats(o);
    }
};

// ii indicates two columns, 1_0 indicates primary index order, 11, 10, 01 indicates there are indexes on both columns, only first, only second
struct t_btree_ii__1_0__0__11__10__01 {
    using t_tuple = Tuple<RamDomain, 2>;
    struct t_comparator_0 {
        // comparators are specialized for the given index order (i.e., 1_0 means (tuple[1], tuple[0]) is the comparator
        int operator()(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[1]) < ramBitCast<RamSigned>(b[1]))
                       ? -1
                   : (ramBitCast<RamSigned>(a[1]) > ramBitCast<RamSigned>(b[1]))
                       ? 1
                       : ((ramBitCast<RamSigned>(a[0]) <
                           ramBitCast<RamSigned>(b[0]))
                              ? -1
                          : (ramBitCast<RamSigned>(a[0]) >
                             ramBitCast<RamSigned>(b[0]))
                              ? 1
                              : (0));
        }
        bool less(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[1]) <
                    ramBitCast<RamSigned>(b[1])) ||
                   (ramBitCast<RamSigned>(a[1]) ==
                    ramBitCast<RamSigned>(b[1])) &&
                       ((ramBitCast<RamSigned>(a[0]) <
                         ramBitCast<RamSigned>(b[0])));
        }
        bool equal(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[1]) ==
                    ramBitCast<RamSigned>(b[1])) &&
                   (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0]));
        }
    };
    // this is the btree on both columns
    using t_ind_0 = btree_set<t_tuple, t_comparator_0>;
    t_ind_0 ind_0;
    struct t_comparator_1 {
        int operator()(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]))
                       ? -1
                   : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0]))
                       ? 1
                       : (0);
        }
        bool less(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]));
        }
        bool equal(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) == ramBitCast<RamSigned>(b[0]));
        }
    };
    // this is the btree on column 0 (we need a separate index because index on col1, col0 cannot be used for indexing col0)
    // note that the full tuple will be stored here regardless of the fact that we build an index on col0
    using t_ind_1 = btree_multiset<t_tuple, t_comparator_1>;
    t_ind_1 ind_1;
    using iterator = t_ind_0::iterator;
    struct context {
        t_ind_0::operation_hints hints_0_lower;
        t_ind_0::operation_hints hints_0_upper;
        t_ind_1::operation_hints hints_1_lower;
        t_ind_1::operation_hints hints_1_upper;
    };
    context createContext() { return context(); }
    bool insert(const t_tuple& t) {
        context h;
        return insert(t, h);
    }
    bool insert(const t_tuple& t, context& h) {
        if (ind_0.insert(t, h.hints_0_lower)) {
            ind_1.insert(t, h.hints_1_lower);
            return true;
        } else
            return false;
    }
    bool insert(const RamDomain* ramDomain) {
        RamDomain data[2];
        std::copy(ramDomain, ramDomain + 2, data);
        const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
        context h;
        return insert(tuple, h);
    }
    bool insert(RamDomain a0, RamDomain a1) {
        RamDomain data[2] = {a0, a1};
        return insert(data);
    }
    bool contains(const t_tuple& t, context& h) const {
        return ind_0.contains(t, h.hints_0_lower);
    }
    bool contains(const t_tuple& t) const {
        context h;
        return contains(t, h);
    }
    std::size_t size() const { return ind_0.size(); }
    iterator find(const t_tuple& t, context& h) const {
        return ind_0.find(t, h.hints_0_lower);
    }
    iterator find(const t_tuple& t) const {
        context h;
        return find(t, h);
    }
    range<iterator> lowerUpperRange_00(const t_tuple& /* lower */,
                                       const t_tuple& /* upper */,
                                       context& /* h */) const {
        return range<iterator>(ind_0.begin(), ind_0.end());
    }
    range<iterator> lowerUpperRange_00(const t_tuple& /* lower */,
                                       const t_tuple& /* upper */) const {
        return range<iterator>(ind_0.begin(), ind_0.end());
    }
    range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower,
                                                const t_tuple& upper,
                                                context& h) const {
        t_comparator_0 comparator;
        int cmp = comparator(lower, upper);
        if (cmp == 0) {
            auto pos = ind_0.find(lower, h.hints_0_lower);
            auto fin = ind_0.end();
            if (pos != fin) {
                fin = pos;
                ++fin;
            }
            return make_range(pos, fin);
        }
        if (cmp > 0) {
            return make_range(ind_0.end(), ind_0.end());
        }
        return make_range(ind_0.lower_bound(lower, h.hints_0_lower),
                          ind_0.upper_bound(upper, h.hints_0_upper));
    }
    range<t_ind_0::iterator> lowerUpperRange_11(const t_tuple& lower,
                                                const t_tuple& upper) const {
        context h;
        return lowerUpperRange_11(lower, upper, h);
    }
    range<t_ind_1::iterator> lowerUpperRange_10(const t_tuple& lower,
                                                const t_tuple& upper,
                                                context& h) const {
        t_comparator_1 comparator;
        int cmp = comparator(lower, upper);
        if (cmp > 0) {
            return make_range(ind_1.end(), ind_1.end());
        }
        return make_range(ind_1.lower_bound(lower, h.hints_1_lower),
                          ind_1.upper_bound(upper, h.hints_1_upper));
    }
    range<t_ind_1::iterator> lowerUpperRange_10(const t_tuple& lower,
                                                const t_tuple& upper) const {
        context h;
        return lowerUpperRange_10(lower, upper, h);
    }
    range<t_ind_0::iterator> lowerUpperRange_01(const t_tuple& lower,
                                                const t_tuple& upper,
                                                context& h) const {
        t_comparator_0 comparator;
        int cmp = comparator(lower, upper);
        if (cmp > 0) {
            return make_range(ind_0.end(), ind_0.end());
        }
        return make_range(ind_0.lower_bound(lower, h.hints_0_lower),
                          ind_0.upper_bound(upper, h.hints_0_upper));
    }
    range<t_ind_0::iterator> lowerUpperRange_01(const t_tuple& lower,
                                                const t_tuple& upper) const {
        context h;
        return lowerUpperRange_01(lower, upper, h);
    }
    bool empty() const { return ind_0.empty(); }
    std::vector<range<iterator>> partition() const {
        return ind_0.getChunks(400);
    }
    void purge() {
        ind_0.clear();
        ind_1.clear();
    }
    iterator begin() const { return ind_0.begin(); }
    iterator end() const { return ind_0.end(); }
    void printStatistics(std::ostream& o) const {
        o << " arity 2 direct b-tree index 0 lex-order [1,0]\n";
        ind_0.printStats(o);
        o << " arity 2 direct b-tree index 1 lex-order [0]\n";
        ind_1.printStats(o);
    }
};
struct t_btree_iii__0_2_1__101__111 {
    using t_tuple = Tuple<RamDomain, 3>;
    struct t_comparator_0 {
        int operator()(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]))
                       ? -1
                   : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0]))
                       ? 1
                       : ((ramBitCast<RamSigned>(a[2]) <
                           ramBitCast<RamSigned>(b[2]))
                              ? -1
                          : (ramBitCast<RamSigned>(a[2]) >
                             ramBitCast<RamSigned>(b[2]))
                              ? 1
                              : ((ramBitCast<RamSigned>(a[1]) <
                                  ramBitCast<RamSigned>(b[1]))
                                     ? -1
                                 : (ramBitCast<RamSigned>(a[1]) >
                                    ramBitCast<RamSigned>(b[1]))
                                     ? 1
                                     : (0)));
        }
        bool less(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) <
                    ramBitCast<RamSigned>(b[0])) ||
                   (ramBitCast<RamSigned>(a[0]) ==
                    ramBitCast<RamSigned>(b[0])) &&
                       ((ramBitCast<RamSigned>(a[2]) <
                         ramBitCast<RamSigned>(b[2])) ||
                        (ramBitCast<RamSigned>(a[2]) ==
                         ramBitCast<RamSigned>(b[2])) &&
                            ((ramBitCast<RamSigned>(a[1]) <
                              ramBitCast<RamSigned>(b[1]))));
        }
        bool equal(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) ==
                    ramBitCast<RamSigned>(b[0])) &&
                   (ramBitCast<RamSigned>(a[2]) ==
                    ramBitCast<RamSigned>(b[2])) &&
                   (ramBitCast<RamSigned>(a[1]) == ramBitCast<RamSigned>(b[1]));
        }
    };
    using t_ind_0 = btree_set<t_tuple, t_comparator_0>;
    t_ind_0 ind_0;
    using iterator = t_ind_0::iterator;
    struct context {
        t_ind_0::operation_hints hints_0_lower;
        t_ind_0::operation_hints hints_0_upper;
    };
    context createContext() { return context(); }
    bool insert(const t_tuple& t) {
        context h;
        return insert(t, h);
    }
    bool insert(const t_tuple& t, context& h) {
        if (ind_0.insert(t, h.hints_0_lower)) {
            return true;
        } else
            return false;
    }
    bool insert(const RamDomain* ramDomain) {
        RamDomain data[3];
        std::copy(ramDomain, ramDomain + 3, data);
        const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
        context h;
        return insert(tuple, h);
    }
    bool insert(RamDomain a0, RamDomain a1, RamDomain a2) {
        RamDomain data[3] = {a0, a1, a2};
        return insert(data);
    }
    bool contains(const t_tuple& t, context& h) const {
        return ind_0.contains(t, h.hints_0_lower);
    }
    bool contains(const t_tuple& t) const {
        context h;
        return contains(t, h);
    }
    std::size_t size() const { return ind_0.size(); }
    iterator find(const t_tuple& t, context& h) const {
        return ind_0.find(t, h.hints_0_lower);
    }
    iterator find(const t_tuple& t) const {
        context h;
        return find(t, h);
    }
    range<iterator> lowerUpperRange_000(const t_tuple& /* lower */,
                                        const t_tuple& /* upper */,
                                        context& /* h */) const {
        return range<iterator>(ind_0.begin(), ind_0.end());
    }
    range<iterator> lowerUpperRange_000(const t_tuple& /* lower */,
                                        const t_tuple& /* upper */) const {
        return range<iterator>(ind_0.begin(), ind_0.end());
    }
    range<t_ind_0::iterator> lowerUpperRange_101(const t_tuple& lower,
                                                 const t_tuple& upper,
                                                 context& h) const {
        t_comparator_0 comparator;
        int cmp = comparator(lower, upper);
        if (cmp > 0) {
            return make_range(ind_0.end(), ind_0.end());
        }
        return make_range(ind_0.lower_bound(lower, h.hints_0_lower),
                          ind_0.upper_bound(upper, h.hints_0_upper));
    }
    range<t_ind_0::iterator> lowerUpperRange_101(const t_tuple& lower,
                                                 const t_tuple& upper) const {
        context h;
        return lowerUpperRange_101(lower, upper, h);
    }
    range<t_ind_0::iterator> lowerUpperRange_111(const t_tuple& lower,
                                                 const t_tuple& upper,
                                                 context& h) const {
        t_comparator_0 comparator;
        int cmp = comparator(lower, upper);
        if (cmp == 0) {
            auto pos = ind_0.find(lower, h.hints_0_lower);
            auto fin = ind_0.end();
            if (pos != fin) {
                fin = pos;
                ++fin;
            }
            return make_range(pos, fin);
        }
        if (cmp > 0) {
            return make_range(ind_0.end(), ind_0.end());
        }
        return make_range(ind_0.lower_bound(lower, h.hints_0_lower),
                          ind_0.upper_bound(upper, h.hints_0_upper));
    }
    range<t_ind_0::iterator> lowerUpperRange_111(const t_tuple& lower,
                                                 const t_tuple& upper) const {
        context h;
        return lowerUpperRange_111(lower, upper, h);
    }
    bool empty() const { return ind_0.empty(); }
    std::vector<range<iterator>> partition() const {
        return ind_0.getChunks(400);
    }
    void purge() { ind_0.clear(); }
    iterator begin() const { return ind_0.begin(); }
    iterator end() const { return ind_0.end(); }
    void printStatistics(std::ostream& o) const {
        o << " arity 3 direct b-tree index 0 lex-order [0,2,1]\n";
        ind_0.printStats(o);
    }
};
struct t_btree_iii__0_1_2__111 {
    using t_tuple = Tuple<RamDomain, 3>;
    struct t_comparator_0 {
        int operator()(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) < ramBitCast<RamSigned>(b[0]))
                       ? -1
                   : (ramBitCast<RamSigned>(a[0]) > ramBitCast<RamSigned>(b[0]))
                       ? 1
                       : ((ramBitCast<RamSigned>(a[1]) <
                           ramBitCast<RamSigned>(b[1]))
                              ? -1
                          : (ramBitCast<RamSigned>(a[1]) >
                             ramBitCast<RamSigned>(b[1]))
                              ? 1
                              : ((ramBitCast<RamSigned>(a[2]) <
                                  ramBitCast<RamSigned>(b[2]))
                                     ? -1
                                 : (ramBitCast<RamSigned>(a[2]) >
                                    ramBitCast<RamSigned>(b[2]))
                                     ? 1
                                     : (0)));
        }
        bool less(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) <
                    ramBitCast<RamSigned>(b[0])) ||
                   (ramBitCast<RamSigned>(a[0]) ==
                    ramBitCast<RamSigned>(b[0])) &&
                       ((ramBitCast<RamSigned>(a[1]) <
                         ramBitCast<RamSigned>(b[1])) ||
                        (ramBitCast<RamSigned>(a[1]) ==
                         ramBitCast<RamSigned>(b[1])) &&
                            ((ramBitCast<RamSigned>(a[2]) <
                              ramBitCast<RamSigned>(b[2]))));
        }
        bool equal(const t_tuple& a, const t_tuple& b) const {
            return (ramBitCast<RamSigned>(a[0]) ==
                    ramBitCast<RamSigned>(b[0])) &&
                   (ramBitCast<RamSigned>(a[1]) ==
                    ramBitCast<RamSigned>(b[1])) &&
                   (ramBitCast<RamSigned>(a[2]) == ramBitCast<RamSigned>(b[2]));
        }
    };
    using t_ind_0 = btree_set<t_tuple, t_comparator_0>;
    t_ind_0 ind_0;
    using iterator = t_ind_0::iterator;
    struct context {
        t_ind_0::operation_hints hints_0_lower;
        t_ind_0::operation_hints hints_0_upper;
    };
    context createContext() { return context(); }
    bool insert(const t_tuple& t) {
        context h;
        return insert(t, h);
    }
    bool insert(const t_tuple& t, context& h) {
        if (ind_0.insert(t, h.hints_0_lower)) {
            return true;
        } else
            return false;
    }
    bool insert(const RamDomain* ramDomain) {
        RamDomain data[3];
        std::copy(ramDomain, ramDomain + 3, data);
        const t_tuple& tuple = reinterpret_cast<const t_tuple&>(data);
        context h;
        return insert(tuple, h);
    }
    bool insert(RamDomain a0, RamDomain a1, RamDomain a2) {
        RamDomain data[3] = {a0, a1, a2};
        return insert(data);
    }
    bool contains(const t_tuple& t, context& h) const {
        return ind_0.contains(t, h.hints_0_lower);
    }
    bool contains(const t_tuple& t) const {
        context h;
        return contains(t, h);
    }
    std::size_t size() const { return ind_0.size(); }
    iterator find(const t_tuple& t, context& h) const {
        return ind_0.find(t, h.hints_0_lower);
    }
    iterator find(const t_tuple& t) const {
        context h;
        return find(t, h);
    }
    range<iterator> lowerUpperRange_000(const t_tuple& /* lower */,
                                        const t_tuple& /* upper */,
                                        context& /* h */) const {
        return range<iterator>(ind_0.begin(), ind_0.end());
    }
    range<iterator> lowerUpperRange_000(const t_tuple& /* lower */,
                                        const t_tuple& /* upper */) const {
        return range<iterator>(ind_0.begin(), ind_0.end());
    }
    range<t_ind_0::iterator> lowerUpperRange_111(const t_tuple& lower,
                                                 const t_tuple& upper,
                                                 context& h) const {
        t_comparator_0 comparator;
        int cmp = comparator(lower, upper);
        if (cmp == 0) {
            auto pos = ind_0.find(lower, h.hints_0_lower);
            auto fin = ind_0.end();
            if (pos != fin) {
                fin = pos;
                ++fin;
            }
            return make_range(pos, fin);
        }
        if (cmp > 0) {
            return make_range(ind_0.end(), ind_0.end());
        }
        return make_range(ind_0.lower_bound(lower, h.hints_0_lower),
                          ind_0.upper_bound(upper, h.hints_0_upper));
    }
    range<t_ind_0::iterator> lowerUpperRange_111(const t_tuple& lower,
                                                 const t_tuple& upper) const {
        context h;
        return lowerUpperRange_111(lower, upper, h);
    }
    bool empty() const { return ind_0.empty(); }
    std::vector<range<iterator>> partition() const {
        return ind_0.getChunks(400);
    }
    void purge() { ind_0.clear(); }
    iterator begin() const { return ind_0.begin(); }
    iterator end() const { return ind_0.end(); }
    void printStatistics(std::ostream& o) const {
        o << " arity 3 direct b-tree index 0 lex-order [0,1,2]\n";
        ind_0.printStats(o);
    }
};

class Sf_output : public SouffleProgram {
   private:
    static inline bool regex_wrapper(const std::string& pattern,
                                     const std::string& text) {
        bool result = false;
        try {
            result = std::regex_match(text, std::regex(pattern));
        } catch (...) {
            std::cerr << "warning: wrong pattern provided for match(\""
                      << pattern << "\",\"" << text << "\").\n";
        }
        return result;
    }

   private:
    static inline std::string substr_wrapper(const std::string& str, size_t idx,
                                             size_t len) {
        std::string result;
        try {
            result = str.substr(idx, len);
        } catch (...) {
            std::cerr << "warning: wrong index position provided by substr(\"";
            std::cerr << str << "\"," << (int32_t)idx << "," << (int32_t)len
                      << ") functor.\n";
        }
        return result;
    }

   public:
    // -- initialize symbol table --
    SymbolTable symTable;  // -- initialize record table --
    RecordTable recordTable;
    // -- Table: @delta_Alias
    Own<t_btree_ii__0_1__11__10> rel_1_delta_Alias =
        mk<t_btree_ii__0_1__11__10>();
    // -- Table: @delta_Assign
    Own<t_btree_ii__0_1__11> rel_2_delta_Assign = mk<t_btree_ii__0_1__11>();
    // -- Table: @delta_VarPointsTo
    Own<t_btree_ii__1_0__0__11__10__01> rel_3_delta_VarPointsTo =
        mk<t_btree_ii__1_0__0__11__10__01>();
    // -- Table: @new_Alias
    Own<t_btree_ii__0_1__11__10> rel_4_new_Alias =
        mk<t_btree_ii__0_1__11__10>();
    // -- Table: @new_Assign
    Own<t_btree_ii__0_1__11> rel_5_new_Assign = mk<t_btree_ii__0_1__11>();
    // -- Table: @new_VarPointsTo
    Own<t_btree_ii__1_0__0__11__10__01> rel_6_new_VarPointsTo =
        mk<t_btree_ii__1_0__0__11__10__01>();
    // -- Table: Alias
    Own<t_btree_ii__0_1__11> rel_7_Alias = mk<t_btree_ii__0_1__11>();
    souffle::RelationWrapper<0, t_btree_ii__0_1__11, Tuple<RamDomain, 2>, 2, 0>
        wrapper_rel_7_Alias;
    // -- Table: Assign
    Own<t_btree_ii__0_1__11> rel_8_Assign = mk<t_btree_ii__0_1__11>();
    souffle::RelationWrapper<1, t_btree_ii__0_1__11, Tuple<RamDomain, 2>, 2, 0>
        wrapper_rel_8_Assign;
    // -- Table: AssignAlloc
    Own<t_btree_ii__0_1__11> rel_9_AssignAlloc = mk<t_btree_ii__0_1__11>();
    souffle::RelationWrapper<2, t_btree_ii__0_1__11, Tuple<RamDomain, 2>, 2, 0>
        wrapper_rel_9_AssignAlloc;
    // -- Table: Load
    Own<t_btree_iii__0_2_1__101__111> rel_10_Load =
        mk<t_btree_iii__0_2_1__101__111>();
    souffle::RelationWrapper<3, t_btree_iii__0_2_1__101__111,
                             Tuple<RamDomain, 3>, 3, 0>
        wrapper_rel_10_Load;
    // -- Table: PrimitiveAssign
    Own<t_btree_ii__0_1__11> rel_11_PrimitiveAssign = mk<t_btree_ii__0_1__11>();
    souffle::RelationWrapper<4, t_btree_ii__0_1__11, Tuple<RamDomain, 2>, 2, 0>
        wrapper_rel_11_PrimitiveAssign;
    // -- Table: Store
    Own<t_btree_iii__0_1_2__111> rel_12_Store = mk<t_btree_iii__0_1_2__111>();
    souffle::RelationWrapper<5, t_btree_iii__0_1_2__111, Tuple<RamDomain, 3>, 3,
                             0>
        wrapper_rel_12_Store;
    // -- Table: VarPointsTo
    Own<t_btree_ii__1_0__0__11__10__01> rel_13_VarPointsTo =
        mk<t_btree_ii__1_0__0__11__10__01>();
    souffle::RelationWrapper<6, t_btree_ii__1_0__0__11__10__01,
                             Tuple<RamDomain, 2>, 2, 0>
        wrapper_rel_13_VarPointsTo;

   public:
    Sf_output()
        : wrapper_rel_7_Alias(
              *rel_7_Alias, symTable, "Alias",
              std::array<const char*, 2>{{"s:Variable", "s:Variable"}},
              std::array<const char*, 2>{{"x", "y"}}),

          wrapper_rel_8_Assign(
              *rel_8_Assign, symTable, "Assign",
              std::array<const char*, 2>{{"s:Variable", "s:Variable"}},
              std::array<const char*, 2>{{"source", "destination"}}),

          wrapper_rel_9_AssignAlloc(
              *rel_9_AssignAlloc, symTable, "AssignAlloc",
              std::array<const char*, 2>{{"s:Variable", "s:Allocation"}},
              std::array<const char*, 2>{{"var", "heap"}}),

          wrapper_rel_10_Load(
              *rel_10_Load, symTable, "Load",
              std::array<const char*, 3>{
                  {"s:Variable", "s:Variable", "s:Field"}},
              std::array<const char*, 3>{{"base", "dest", "field"}}),

          wrapper_rel_11_PrimitiveAssign(
              *rel_11_PrimitiveAssign, symTable, "PrimitiveAssign",
              std::array<const char*, 2>{{"s:Variable", "s:Variable"}},
              std::array<const char*, 2>{{"source", "dest"}}),

          wrapper_rel_12_Store(
              *rel_12_Store, symTable, "Store",
              std::array<const char*, 3>{
                  {"s:Variable", "s:Variable", "s:Field"}},
              std::array<const char*, 3>{{"source", "base", "field"}}),

          wrapper_rel_13_VarPointsTo(
              *rel_13_VarPointsTo, symTable, "VarPointsTo",
              std::array<const char*, 2>{{"s:Variable", "s:Allocation"}},
              std::array<const char*, 2>{{"var", "heap"}}) {
        addRelation("Alias", &wrapper_rel_7_Alias, false, true);
        addRelation("Assign", &wrapper_rel_8_Assign, false, true);
        addRelation("AssignAlloc", &wrapper_rel_9_AssignAlloc, true, false);
        addRelation("Load", &wrapper_rel_10_Load, true, false);
        addRelation("PrimitiveAssign", &wrapper_rel_11_PrimitiveAssign, true,
                    false);
        addRelation("Store", &wrapper_rel_12_Store, true, false);
        addRelation("VarPointsTo", &wrapper_rel_13_VarPointsTo, false, true);
    }
    ~Sf_output() {}

   private:
    std::string inputDirectory;
    std::string outputDirectory;
    bool performIO;
    std::atomic<RamDomain> ctr{};

    std::atomic<size_t> iter{};
    void runFunction(std::string inputDirectoryArg = "",
                     std::string outputDirectoryArg = "",
                     bool performIOArg = false) {
        this->inputDirectory = inputDirectoryArg;
        this->outputDirectory = outputDirectoryArg;
        this->performIO = performIOArg;
        SignalHandler::instance()->set();
#if defined(_OPENMP)
        if (getNumThreads() > 0) {
            omp_set_num_threads(getNumThreads());
        }
#endif

        // -- query evaluation --
        {
            std::vector<RamDomain> args, ret;
            subroutine_0(args, ret);
        }
        {
            std::vector<RamDomain> args, ret;
            subroutine_1(args, ret);
        }
        {
            std::vector<RamDomain> args, ret;
            subroutine_2(args, ret);
        }
        {
            std::vector<RamDomain> args, ret;
            subroutine_3(args, ret);
        }
        {
            std::vector<RamDomain> args, ret;
            subroutine_4(args, ret);
        }

        // -- relation hint statistics --
        SignalHandler::instance()->reset();
    }

   public:
    void run() override { runFunction("", "", false); }

   public:
    void runAll(std::string inputDirectoryArg = "",
                std::string outputDirectoryArg = "") override {
        runFunction(inputDirectoryArg, outputDirectoryArg, true);
    }

   public:
    void printAll(std::string outputDirectoryArg = "") override {
        try {
            std::map<std::string, std::string> directiveMap(
                {{"IO", "file"},
                 {"attributeNames", "x\ty"},
                 {"name", "Alias"},
                 {"operation", "output"},
                 {"output-dir", "."},
                 {"params",
                  "{\"records\": {}, \"relation\": {\"arity\": 2, "
                  "\"auxArity\": 0, \"params\": [\"x\", \"y\"]}}"},
                 {"types",
                  "{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": "
                  "2, \"auxArity\": 0, \"types\": [\"s:Variable\", "
                  "\"s:Variable\"]}}"}});
            if (!outputDirectoryArg.empty()) {
                directiveMap["output-dir"] = outputDirectoryArg;
            }
            IOSystem::getInstance()
                .getWriter(directiveMap, symTable, recordTable)
                ->writeAll(*rel_7_Alias);
        } catch (std::exception& e) {
            std::cerr << e.what();
            exit(1);
        }
        try {
            std::map<std::string, std::string> directiveMap(
                {{"IO", "file"},
                 {"attributeNames", "source\tdestination"},
                 {"name", "Assign"},
                 {"operation", "output"},
                 {"output-dir", "."},
                 {"params",
                  "{\"records\": {}, \"relation\": {\"arity\": 2, "
                  "\"auxArity\": 0, \"params\": [\"source\", "
                  "\"destination\"]}}"},
                 {"types",
                  "{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": "
                  "2, \"auxArity\": 0, \"types\": [\"s:Variable\", "
                  "\"s:Variable\"]}}"}});
            if (!outputDirectoryArg.empty()) {
                directiveMap["output-dir"] = outputDirectoryArg;
            }
            IOSystem::getInstance()
                .getWriter(directiveMap, symTable, recordTable)
                ->writeAll(*rel_8_Assign);
        } catch (std::exception& e) {
            std::cerr << e.what();
            exit(1);
        }
        try {
            std::map<std::string, std::string> directiveMap(
                {{"IO", "file"},
                 {"attributeNames", "var\theap"},
                 {"name", "VarPointsTo"},
                 {"operation", "output"},
                 {"output-dir", "."},
                 {"params",
                  "{\"records\": {}, \"relation\": {\"arity\": 2, "
                  "\"auxArity\": 0, \"params\": [\"var\", \"heap\"]}}"},
                 {"types",
                  "{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": "
                  "2, \"auxArity\": 0, \"types\": [\"s:Variable\", "
                  "\"s:Allocation\"]}}"}});
            if (!outputDirectoryArg.empty()) {
                directiveMap["output-dir"] = outputDirectoryArg;
            }
            IOSystem::getInstance()
                .getWriter(directiveMap, symTable, recordTable)
                ->writeAll(*rel_13_VarPointsTo);
        } catch (std::exception& e) {
            std::cerr << e.what();
            exit(1);
        }
    }

   public:
    void loadAll(std::string inputDirectoryArg = "") override {
        try {
            std::map<std::string, std::string> directiveMap(
                {{"IO", "file"},
                 {"attributeNames", "source\tdest"},
                 {"fact-dir",
                  "."},
                 {"name", "PrimitiveAssign"},
                 {"operation", "input"},
                 {"params",
                  "{\"records\": {}, \"relation\": {\"arity\": 2, "
                  "\"auxArity\": 0, \"params\": [\"source\", \"dest\"]}}"},
                 {"types",
                  "{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": "
                  "2, \"auxArity\": 0, \"types\": [\"s:Variable\", "
                  "\"s:Variable\"]}}"}});
            if (!inputDirectoryArg.empty()) {
                directiveMap["fact-dir"] = inputDirectoryArg;
            }
            IOSystem::getInstance()
                .getReader(directiveMap, symTable, recordTable)
                ->readAll(*rel_11_PrimitiveAssign);
        } catch (std::exception& e) {
            std::cerr << "Error loading data: " << e.what() << '\n';
        }
        try {
            std::map<std::string, std::string> directiveMap(
                {{"IO", "file"},
                 {"attributeNames", "var\theap"},
                 {"fact-dir",
                  "."},
                 {"name", "AssignAlloc"},
                 {"operation", "input"},
                 {"params",
                  "{\"records\": {}, \"relation\": {\"arity\": 2, "
                  "\"auxArity\": 0, \"params\": [\"var\", \"heap\"]}}"},
                 {"types",
                  "{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": "
                  "2, \"auxArity\": 0, \"types\": [\"s:Variable\", "
                  "\"s:Allocation\"]}}"}});
            if (!inputDirectoryArg.empty()) {
                directiveMap["fact-dir"] = inputDirectoryArg;
            }
            IOSystem::getInstance()
                .getReader(directiveMap, symTable, recordTable)
                ->readAll(*rel_9_AssignAlloc);
        } catch (std::exception& e) {
            std::cerr << "Error loading data: " << e.what() << '\n';
        }
        try {
            std::map<std::string, std::string> directiveMap(
                {{"IO", "file"},
                 {"attributeNames", "source\tbase\tfield"},
                 {"fact-dir",
                  "."},
                 {"name", "Store"},
                 {"operation", "input"},
                 {"params",
                  "{\"records\": {}, \"relation\": {\"arity\": 3, "
                  "\"auxArity\": 0, \"params\": [\"source\", \"base\", "
                  "\"field\"]}}"},
                 {"types",
                  "{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": "
                  "3, \"auxArity\": 0, \"types\": [\"s:Variable\", "
                  "\"s:Variable\", \"s:Field\"]}}"}});
            if (!inputDirectoryArg.empty()) {
                directiveMap["fact-dir"] = inputDirectoryArg;
            }
            IOSystem::getInstance()
                .getReader(directiveMap, symTable, recordTable)
                ->readAll(*rel_12_Store);
        } catch (std::exception& e) {
            std::cerr << "Error loading data: " << e.what() << '\n';
        }
        try {
            std::map<std::string, std::string> directiveMap(
                {{"IO", "file"},
                 {"attributeNames", "base\tdest\tfield"},
                 {"fact-dir",
                  "."},
                 {"name", "Load"},
                 {"operation", "input"},
                 {"params",
                  "{\"records\": {}, \"relation\": {\"arity\": 3, "
                  "\"auxArity\": 0, \"params\": [\"base\", \"dest\", "
                  "\"field\"]}}"},
                 {"types",
                  "{\"ADTs\": {}, \"records\": {}, \"relation\": {\"arity\": "
                  "3, \"auxArity\": 0, \"types\": [\"s:Variable\", "
                  "\"s:Variable\", \"s:Field\"]}}"}});
            if (!inputDirectoryArg.empty()) {
                directiveMap["fact-dir"] = inputDirectoryArg;
            }
            IOSystem::getInstance()
                .getReader(directiveMap, symTable, recordTable)
                ->readAll(*rel_10_Load);
        } catch (std::exception& e) {
            std::cerr << "Error loading data: " << e.what() << '\n';
        }
    }

   public:
    void dumpInputs() override {
        try {
            std::map<std::string, std::string> rwOperation;
            rwOperation["IO"] = "stdout";
            rwOperation["name"] = "PrimitiveAssign";
            rwOperation["types"] =
                "{\"relation\": {\"arity\": 2, \"auxArity\": 0, \"types\": "
                "[\"s:Variable\", \"s:Variable\"]}}";
            IOSystem::getInstance()
                .getWriter(rwOperation, symTable, recordTable)
                ->writeAll(*rel_11_PrimitiveAssign);
        } catch (std::exception& e) {
            std::cerr << e.what();
            exit(1);
        }
        try {
            std::map<std::string, std::string> rwOperation;
            rwOperation["IO"] = "stdout";
            rwOperation["name"] = "AssignAlloc";
            rwOperation["types"] =
                "{\"relation\": {\"arity\": 2, \"auxArity\": 0, \"types\": "
                "[\"s:Variable\", \"s:Allocation\"]}}";
            IOSystem::getInstance()
                .getWriter(rwOperation, symTable, recordTable)
                ->writeAll(*rel_9_AssignAlloc);
        } catch (std::exception& e) {
            std::cerr << e.what();
            exit(1);
        }
        try {
            std::map<std::string, std::string> rwOperation;
            rwOperation["IO"] = "stdout";
            rwOperation["name"] = "Store";
            rwOperation["types"] =
                "{\"relation\": {\"arity\": 3, \"auxArity\": 0, \"types\": "
                "[\"s:Variable\", \"s:Variable\", \"s:Field\"]}}";
            IOSystem::getInstance()
                .getWriter(rwOperation, symTable, recordTable)
                ->writeAll(*rel_12_Store);
        } catch (std::exception& e) {
            std::cerr << e.what();
            exit(1);
        }
        try {
            std::map<std::string, std::string> rwOperation;
            rwOperation["IO"] = "stdout";
            rwOperation["name"] = "Load";
            rwOperation["types"] =
                "{\"relation\": {\"arity\": 3, \"auxArity\": 0, \"types\": "
                "[\"s:Variable\", \"s:Variable\", \"s:Field\"]}}";
            IOSystem::getInstance()
                .getWriter(rwOperation, symTable, recordTable)
                ->writeAll(*rel_10_Load);
        } catch (std::exception& e) {
            std::cerr << e.what();
            exit(1);
        }
    }

   public:
    void dumpOutputs() override {
        try {
            std::map<std::string, std::string> rwOperation;
            rwOperation["IO"] = "stdout";
            rwOperation["name"] = "Alias";
            rwOperation["types"] =
                "{\"relation\": {\"arity\": 2, \"auxArity\": 0, \"types\": "
                "[\"s:Variable\", \"s:Variable\"]}}";
            IOSystem::getInstance()
                .getWriter(rwOperation, symTable, recordTable)
                ->writeAll(*rel_7_Alias);
        } catch (std::exception& e) {
            std::cerr << e.what();
            exit(1);
        }
        try {
            std::map<std::string, std::string> rwOperation;
            rwOperation["IO"] = "stdout";
            rwOperation["name"] = "Assign";
            rwOperation["types"] =
                "{\"relation\": {\"arity\": 2, \"auxArity\": 0, \"types\": "
                "[\"s:Variable\", \"s:Variable\"]}}";
            IOSystem::getInstance()
                .getWriter(rwOperation, symTable, recordTable)
                ->writeAll(*rel_8_Assign);
        } catch (std::exception& e) {
            std::cerr << e.what();
            exit(1);
        }
        try {
            std::map<std::string, std::string> rwOperation;
            rwOperation["IO"] = "stdout";
            rwOperation["name"] = "VarPointsTo";
            rwOperation["types"] =
                "{\"relation\": {\"arity\": 2, \"auxArity\": 0, \"types\": "
                "[\"s:Variable\", \"s:Allocation\"]}}";
            IOSystem::getInstance()
                .getWriter(rwOperation, symTable, recordTable)
                ->writeAll(*rel_13_VarPointsTo);
        } catch (std::exception& e) {
            std::cerr << e.what();
            exit(1);
        }
    }

   public:
    SymbolTable& getSymbolTable() override { return symTable; }
    void executeSubroutine(std::string name, const std::vector<RamDomain>& args,
                           std::vector<RamDomain>& ret) override {
        if (name == "stratum_0") {
            subroutine_0(args, ret);
            return;
        }
        if (name == "stratum_1") {
            subroutine_1(args, ret);
            return;
        }
        if (name == "stratum_2") {
            subroutine_2(args, ret);
            return;
        }
        if (name == "stratum_3") {
            subroutine_3(args, ret);
            return;
        }
        if (name == "stratum_4") {
            subroutine_4(args, ret);
            return;
        }
        fatal("unknown subroutine");
    }
#ifdef _MSC_VER
#pragma warning(disable : 4100)
#endif  // _MSC_VER
    void subroutine_0(const std::vector<RamDomain>& args,
                      std::vector<RamDomain>& ret) {
        if (performIO) {
            try {
                std::map<std::string, std::string> directiveMap(
                    {{"IO", "file"},
                     {"attributeNames", "var\theap"},
                     {"fact-dir",
                      "/home/supun/phd/research/datalog/datalog/souffle-simple-pa/facts"},
                     {"name", "AssignAlloc"},
                     {"operation", "input"},
                     {"params",
                      "{\"records\": {}, \"relation\": {\"arity\": 2, "
                      "\"auxArity\": 0, \"params\": [\"var\", \"heap\"]}}"},
                     {"types",
                      "{\"ADTs\": {}, \"records\": {}, \"relation\": "
                      "{\"arity\": 2, \"auxArity\": 0, \"types\": "
                      "[\"s:Variable\", \"s:Allocation\"]}}"}});
                if (!inputDirectory.empty()) {
                    directiveMap["fact-dir"] = inputDirectory;
                }
                IOSystem::getInstance()
                    .getReader(directiveMap, symTable, recordTable)
                    ->readAll(*rel_9_AssignAlloc);
            } catch (std::exception& e) {
                std::cerr << "Error loading data: " << e.what() << '\n';
            }
        }
    }
#ifdef _MSC_VER
#pragma warning(default : 4100)
#endif  // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable : 4100)
#endif  // _MSC_VER
    void subroutine_1(const std::vector<RamDomain>& args,
                      std::vector<RamDomain>& ret) {
        if (performIO) {
            try {
                std::map<std::string, std::string> directiveMap(
                    {{"IO", "file"},
                     {"attributeNames", "base\tdest\tfield"},
                     {"fact-dir",
                      "/home/supun/phd/research/datalog/datalog/souffle-simple-pa/facts"},
                     {"name", "Load"},
                     {"operation", "input"},
                     {"params",
                      "{\"records\": {}, \"relation\": {\"arity\": 3, "
                      "\"auxArity\": 0, \"params\": [\"base\", \"dest\", "
                      "\"field\"]}}"},
                     {"types",
                      "{\"ADTs\": {}, \"records\": {}, \"relation\": "
                      "{\"arity\": 3, \"auxArity\": 0, \"types\": "
                      "[\"s:Variable\", \"s:Variable\", \"s:Field\"]}}"}});
                if (!inputDirectory.empty()) {
                    directiveMap["fact-dir"] = inputDirectory;
                }
                IOSystem::getInstance()
                    .getReader(directiveMap, symTable, recordTable)
                    ->readAll(*rel_10_Load);
            } catch (std::exception& e) {
                std::cerr << "Error loading data: " << e.what() << '\n';
            }
        }
    }
#ifdef _MSC_VER
#pragma warning(default : 4100)
#endif  // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable : 4100)
#endif  // _MSC_VER
    void subroutine_2(const std::vector<RamDomain>& args,
                      std::vector<RamDomain>& ret) {
        if (performIO) {
            try {
                std::map<std::string, std::string> directiveMap(
                    {{"IO", "file"},
                     {"attributeNames", "source\tdest"},
                     {"fact-dir",
                      "/home/supun/phd/research/datalog/datalog/souffle-simple-pa/facts"},
                     {"name", "PrimitiveAssign"},
                     {"operation", "input"},
                     {"params",
                      "{\"records\": {}, \"relation\": {\"arity\": 2, "
                      "\"auxArity\": 0, \"params\": [\"source\", \"dest\"]}}"},
                     {"types",
                      "{\"ADTs\": {}, \"records\": {}, \"relation\": "
                      "{\"arity\": 2, \"auxArity\": 0, \"types\": "
                      "[\"s:Variable\", \"s:Variable\"]}}"}});
                if (!inputDirectory.empty()) {
                    directiveMap["fact-dir"] = inputDirectory;
                }
                IOSystem::getInstance()
                    .getReader(directiveMap, symTable, recordTable)
                    ->readAll(*rel_11_PrimitiveAssign);
            } catch (std::exception& e) {
                std::cerr << "Error loading data: " << e.what() << '\n';
            }
        }
    }
#ifdef _MSC_VER
#pragma warning(default : 4100)
#endif  // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable : 4100)
#endif  // _MSC_VER
    void subroutine_3(const std::vector<RamDomain>& args,
                      std::vector<RamDomain>& ret) {
        if (performIO) {
            try {
                std::map<std::string, std::string> directiveMap(
                    {{"IO", "file"},
                     {"attributeNames", "source\tbase\tfield"},
                     {"fact-dir",
                      "/home/supun/phd/research/datalog/datalog/souffle-simple-pa/facts"},
                     {"name", "Store"},
                     {"operation", "input"},
                     {"params",
                      "{\"records\": {}, \"relation\": {\"arity\": 3, "
                      "\"auxArity\": 0, \"params\": [\"source\", \"base\", "
                      "\"field\"]}}"},
                     {"types",
                      "{\"ADTs\": {}, \"records\": {}, \"relation\": "
                      "{\"arity\": 3, \"auxArity\": 0, \"types\": "
                      "[\"s:Variable\", \"s:Variable\", \"s:Field\"]}}"}});
                if (!inputDirectory.empty()) {
                    directiveMap["fact-dir"] = inputDirectory;
                }
                IOSystem::getInstance()
                    .getReader(directiveMap, symTable, recordTable)
                    ->readAll(*rel_12_Store);
            } catch (std::exception& e) {
                std::cerr << "Error loading data: " << e.what() << '\n';
            }
        }
    }
#ifdef _MSC_VER
#pragma warning(default : 4100)
#endif  // _MSC_VER
#ifdef _MSC_VER
#pragma warning(disable : 4100)
#endif  // _MSC_VER
    void subroutine_4(const std::vector<RamDomain>& args,
                      std::vector<RamDomain>& ret) {
        SignalHandler::instance()->setMsg(R"_(Assign(var1,var2) :- 
   PrimitiveAssign(var1,var2).
in file /home/supun/phd/research/datalog/datalog/souffle-simple-pa/pointsto.dl [14:1-15:30])_");
        if (!(rel_11_PrimitiveAssign->empty())) {
            [&]() {
                CREATE_OP_CONTEXT(rel_8_Assign_op_ctxt,
                                  rel_8_Assign->createContext());
                CREATE_OP_CONTEXT(rel_11_PrimitiveAssign_op_ctxt,
                                  rel_11_PrimitiveAssign->createContext());
                for (const auto& env0 : *rel_11_PrimitiveAssign) {
                    Tuple<RamDomain, 2> tuple{
                        {ramBitCast(env0[0]), ramBitCast(env0[1])}};
                    rel_8_Assign->insert(tuple,
                                         READ_OP_CONTEXT(rel_8_Assign_op_ctxt));
                }
            }();
        }
        [&]() {
            CREATE_OP_CONTEXT(rel_2_delta_Assign_op_ctxt,
                              rel_2_delta_Assign->createContext());
            CREATE_OP_CONTEXT(rel_8_Assign_op_ctxt,
                              rel_8_Assign->createContext());
            for (const auto& env0 : *rel_8_Assign) {
                Tuple<RamDomain, 2> tuple{
                    {ramBitCast(env0[0]), ramBitCast(env0[1])}};
                rel_2_delta_Assign->insert(
                    tuple, READ_OP_CONTEXT(rel_2_delta_Assign_op_ctxt));
            }
        }();
        SignalHandler::instance()->setMsg(R"_(VarPointsTo(var,heap) :- 
   AssignAlloc(var,heap).
in file /home/supun/phd/research/datalog/datalog/souffle-simple-pa/pointsto.dl [21:1-22:26])_");
        if (!(rel_9_AssignAlloc->empty())) {
            [&]() {
                CREATE_OP_CONTEXT(rel_9_AssignAlloc_op_ctxt,
                                  rel_9_AssignAlloc->createContext());
                CREATE_OP_CONTEXT(rel_13_VarPointsTo_op_ctxt,
                                  rel_13_VarPointsTo->createContext());
                for (const auto& env0 : *rel_9_AssignAlloc) {
                    Tuple<RamDomain, 2> tuple{
                        {ramBitCast(env0[0]), ramBitCast(env0[1])}};
                    rel_13_VarPointsTo->insert(
                        tuple, READ_OP_CONTEXT(rel_13_VarPointsTo_op_ctxt));
                }
            }();
        }
        [&]() {
            CREATE_OP_CONTEXT(rel_3_delta_VarPointsTo_op_ctxt,
                              rel_3_delta_VarPointsTo->createContext());
            CREATE_OP_CONTEXT(rel_13_VarPointsTo_op_ctxt,
                              rel_13_VarPointsTo->createContext());
            for (const auto& env0 : *rel_13_VarPointsTo) {
                Tuple<RamDomain, 2> tuple{
                    {ramBitCast(env0[0]), ramBitCast(env0[1])}};
                rel_3_delta_VarPointsTo->insert(
                    tuple, READ_OP_CONTEXT(rel_3_delta_VarPointsTo_op_ctxt));
            }
        }();
        [&]() {
            CREATE_OP_CONTEXT(rel_7_Alias_op_ctxt,
                              rel_7_Alias->createContext());
            CREATE_OP_CONTEXT(rel_1_delta_Alias_op_ctxt,
                              rel_1_delta_Alias->createContext());
            for (const auto& env0 : *rel_7_Alias) {
                Tuple<RamDomain, 2> tuple{
                    {ramBitCast(env0[0]), ramBitCast(env0[1])}};
                rel_1_delta_Alias->insert(
                    tuple, READ_OP_CONTEXT(rel_1_delta_Alias_op_ctxt));
            }
        }();

        iter = 0;
        // std::cout << "store size = " << rel_12_Store->size()
        //           << std::endl;
        // std::cout << "load size = " << rel_10_Load->size()
        //           << std::endl;
        // std::cout << "assign size = " << rel_8_Assign->size()
        //           << std::endl;
        // std::cout << "alloc size = " << rel_9_AssignAlloc->size()
        //           << std::endl;

        for (;;) {
            SECTIONS_START;
            SECTION_START;
            SignalHandler::instance()->setMsg(R"_(Assign(var1,var2) :- 
   Store(var1,instanceVar2,field),
   Alias(instanceVar2,instanceVar1),
   Load(instanceVar1,var2,field).
in file /home/supun/phd/research/datalog/datalog/souffle-simple-pa/pointsto.dl [28:1-31:35])_");
            std::cout << "iter " << iter << std::endl;
            // std::cout << "-------------iter " << iter << "-------------\n";
            // std::cout << "delta_vp size = " <<
            // rel_3_delta_VarPointsTo->size()
            //           << std::endl;
            // std::cout << "delta_Alias size = " << rel_1_delta_Alias->size()
            //           << std::endl;
            // std::cout << "delta_assign size = " << rel_2_delta_Assign->size()
            //           << std::endl;
            // std::cout << "-------------------------------\n";
            // std::cout << "vp size = " << rel_13_VarPointsTo->size()
            //           << std::endl;
            // std::cout << "alias size = " << rel_7_Alias->size() << std::endl;
            // std::cout << "assign size = " << rel_8_Assign->size() <<
            // std::endl; std::cout << "-------------------------------\n";

            if (!(rel_10_Load->empty()) && !(rel_12_Store->empty()) &&
                !(rel_1_delta_Alias->empty())) {
                [&]() {
                    CREATE_OP_CONTEXT(rel_8_Assign_op_ctxt,
                                      rel_8_Assign->createContext());
                    CREATE_OP_CONTEXT(rel_5_new_Assign_op_ctxt,
                                      rel_5_new_Assign->createContext());
                    CREATE_OP_CONTEXT(rel_10_Load_op_ctxt,
                                      rel_10_Load->createContext());
                    CREATE_OP_CONTEXT(rel_12_Store_op_ctxt,
                                      rel_12_Store->createContext());
                    CREATE_OP_CONTEXT(rel_1_delta_Alias_op_ctxt,
                                      rel_1_delta_Alias->createContext());
                    for (const auto& env0 : *rel_12_Store) {
                        // TODO - isn't this do the below range search for same instanceVar again and again? iterating the set of set of instanceVar would be better in this case (specially if there are a lot of duplicated instanceVar values)
                        auto range = rel_1_delta_Alias->lowerUpperRange_10(
                            Tuple<RamDomain, 2>{
                                {ramBitCast(env0[1]),
                                 ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},
                            Tuple<RamDomain, 2>{
                                {ramBitCast(env0[1]),
                                 ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},
                            READ_OP_CONTEXT(rel_1_delta_Alias_op_ctxt));
                        for (const auto& env1 : range) {
                            auto range = rel_10_Load->lowerUpperRange_101(
                                Tuple<RamDomain, 3>{
                                    {ramBitCast(env1[1]),
                                     ramBitCast<RamDomain>(MIN_RAM_SIGNED),
                                     ramBitCast(env0[2])}},
                                Tuple<RamDomain, 3>{
                                    {ramBitCast(env1[1]),
                                     ramBitCast<RamDomain>(MAX_RAM_SIGNED),
                                     ramBitCast(env0[2])}},
                                READ_OP_CONTEXT(rel_10_Load_op_ctxt));
                            for (const auto& env2 : range) {
                                if (!(rel_8_Assign->contains(
                                        Tuple<RamDomain, 2>{
                                            {ramBitCast(env0[0]),
                                             ramBitCast(env2[1])}},
                                        READ_OP_CONTEXT(
                                            rel_8_Assign_op_ctxt)))) {
                                    Tuple<RamDomain, 2> tuple{
                                        {ramBitCast(env0[0]),
                                         ramBitCast(env2[1])}};
                                    rel_5_new_Assign->insert(
                                        tuple, READ_OP_CONTEXT(
                                                   rel_5_new_Assign_op_ctxt));
                                }
                            }
                        }
                    }
                }();
            }
            SECTION_END
            SECTION_START;
            SignalHandler::instance()->setMsg(R"_(VarPointsTo(var1,heap) :- 
   Assign(var2,var1),
   VarPointsTo(var2,heap).
in file /home/supun/phd/research/datalog/datalog/souffle-simple-pa/pointsto.dl [24:1-26:27])_");
            if (!(rel_2_delta_Assign->empty()) &&
                !(rel_13_VarPointsTo->empty())) {
                [&]() {
                    CREATE_OP_CONTEXT(rel_2_delta_Assign_op_ctxt,
                                      rel_2_delta_Assign->createContext());
                    CREATE_OP_CONTEXT(rel_3_delta_VarPointsTo_op_ctxt,
                                      rel_3_delta_VarPointsTo->createContext());
                    CREATE_OP_CONTEXT(rel_13_VarPointsTo_op_ctxt,
                                      rel_13_VarPointsTo->createContext());
                    CREATE_OP_CONTEXT(rel_6_new_VarPointsTo_op_ctxt,
                                      rel_6_new_VarPointsTo->createContext());
                    for (const auto& env0 : *rel_2_delta_Assign) {
                        auto range = rel_13_VarPointsTo->lowerUpperRange_10(
                            Tuple<RamDomain, 2>{
                                {ramBitCast(env0[0]),
                                 ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},
                            Tuple<RamDomain, 2>{
                                {ramBitCast(env0[0]),
                                 ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},
                            READ_OP_CONTEXT(rel_13_VarPointsTo_op_ctxt));
                        for (const auto& env1 : range) {
                            if (!(rel_3_delta_VarPointsTo->contains(
                                    Tuple<RamDomain, 2>{{ramBitCast(env0[0]),
                                                         ramBitCast(env1[1])}},
                                    READ_OP_CONTEXT(
                                        rel_3_delta_VarPointsTo_op_ctxt))) &&
                                !(rel_13_VarPointsTo->contains(
                                    Tuple<RamDomain, 2>{{ramBitCast(env0[1]),
                                                         ramBitCast(env1[1])}},
                                    READ_OP_CONTEXT(
                                        rel_13_VarPointsTo_op_ctxt)))) {
                                Tuple<RamDomain, 2> tuple{
                                    {ramBitCast(env0[1]), ramBitCast(env1[1])}};
                                rel_6_new_VarPointsTo->insert(
                                    tuple, READ_OP_CONTEXT(
                                               rel_6_new_VarPointsTo_op_ctxt));
                            }
                        }
                    }
                }();
            }
            SignalHandler::instance()->setMsg(R"_(VarPointsTo(var1,heap) :- 
   Assign(var2,var1),
   VarPointsTo(var2,heap).
in file /home/supun/phd/research/datalog/datalog/souffle-simple-pa/pointsto.dl [24:1-26:27])_");
            if (!(rel_8_Assign->empty()) &&
                !(rel_3_delta_VarPointsTo->empty())) {
                [&]() {
                    CREATE_OP_CONTEXT(rel_8_Assign_op_ctxt,
                                      rel_8_Assign->createContext());
                    CREATE_OP_CONTEXT(rel_3_delta_VarPointsTo_op_ctxt,
                                      rel_3_delta_VarPointsTo->createContext());
                    CREATE_OP_CONTEXT(rel_13_VarPointsTo_op_ctxt,
                                      rel_13_VarPointsTo->createContext());
                    CREATE_OP_CONTEXT(rel_6_new_VarPointsTo_op_ctxt,
                                      rel_6_new_VarPointsTo->createContext());
                    for (const auto& env0 : *rel_8_Assign) {
                        auto range =
                            rel_3_delta_VarPointsTo->lowerUpperRange_10(
                                Tuple<RamDomain, 2>{
                                    {ramBitCast(env0[0]),
                                     ramBitCast<RamDomain>(MIN_RAM_SIGNED)}},
                                Tuple<RamDomain, 2>{
                                    {ramBitCast(env0[0]),
                                     ramBitCast<RamDomain>(MAX_RAM_SIGNED)}},
                                READ_OP_CONTEXT(
                                    rel_3_delta_VarPointsTo_op_ctxt));
                        for (const auto& env1 : range) {
                            if (!(rel_13_VarPointsTo->contains(
                                    Tuple<RamDomain, 2>{{ramBitCast(env0[1]),
                                                         ramBitCast(env1[1])}},
                                    READ_OP_CONTEXT(
                                        rel_13_VarPointsTo_op_ctxt)))) {
                                Tuple<RamDomain, 2> tuple{
                                    {ramBitCast(env0[1]), ramBitCast(env1[1])}};
                                rel_6_new_VarPointsTo->insert(
                                    tuple, READ_OP_CONTEXT(
                                               rel_6_new_VarPointsTo_op_ctxt));
                            }
                        }
                    }
                }();
            }
            SECTION_END
            SECTION_START;
            SignalHandler::instance()->setMsg(R"_(Alias(instanceVar,iVar) :- 
   VarPointsTo(instanceVar,instanceHeap),
   VarPointsTo(iVar,instanceHeap).
in file /home/supun/phd/research/datalog/datalog/souffle-simple-pa/pointsto.dl [17:1-19:35])_");
            if (!(rel_3_delta_VarPointsTo->empty()) &&
                !(rel_13_VarPointsTo->empty())) {
                [&]() {
                    CREATE_OP_CONTEXT(rel_3_delta_VarPointsTo_op_ctxt,
                                      rel_3_delta_VarPointsTo->createContext());
                    CREATE_OP_CONTEXT(rel_13_VarPointsTo_op_ctxt,
                                      rel_13_VarPointsTo->createContext());
                    CREATE_OP_CONTEXT(rel_7_Alias_op_ctxt,
                                      rel_7_Alias->createContext());
                    CREATE_OP_CONTEXT(rel_4_new_Alias_op_ctxt,
                                      rel_4_new_Alias->createContext());
                    for (const auto& env0 : *rel_3_delta_VarPointsTo) {
                        auto range = rel_13_VarPointsTo->lowerUpperRange_01(
                            Tuple<RamDomain, 2>{
                                {ramBitCast<RamDomain>(MIN_RAM_SIGNED),
                                 ramBitCast(env0[1])}},
                            Tuple<RamDomain, 2>{
                                {ramBitCast<RamDomain>(MAX_RAM_SIGNED),
                                 ramBitCast(env0[1])}},
                            READ_OP_CONTEXT(rel_13_VarPointsTo_op_ctxt));
                        for (const auto& env1 : range) {
                            // TODO - why do we have this extra check (in delta_VarPointsTo). This doesn't break the
                            // semantics since if it is in delta, it will be considered and inserted in the next loop (in the stable, delta case)
                            // but why having this additional check? Is it because this is embaraisngly parallelizable whereas the insert is not
                            // so that minimizing double insertions by adding this additional check is worthwhile?
                            // specifically stable contains delta. So (for cases where same growing relation is used twice+ in a rule)
                            // both stable, delta case and delta, stable case computes delta, delta case (hence, it is done twice). Maybe I can do the same in parallel case?
                            if (!(rel_3_delta_VarPointsTo->contains(
                                    Tuple<RamDomain, 2>{{ramBitCast(env1[0]),
                                                         ramBitCast(env0[1])}},
                                    READ_OP_CONTEXT(
                                        rel_3_delta_VarPointsTo_op_ctxt))) &&
                                !(rel_7_Alias->contains(
                                    Tuple<RamDomain, 2>{{ramBitCast(env0[0]),
                                                         ramBitCast(env1[0])}},
                                    READ_OP_CONTEXT(rel_7_Alias_op_ctxt)))) {
                                Tuple<RamDomain, 2> tuple{
                                    {ramBitCast(env0[0]), ramBitCast(env1[0])}};
                                rel_4_new_Alias->insert(
                                    tuple,
                                    READ_OP_CONTEXT(rel_4_new_Alias_op_ctxt));
                            }
                        }
                    }
                }();
            }
            SignalHandler::instance()->setMsg(R"_(Alias(instanceVar,iVar) :- 
   VarPointsTo(instanceVar,instanceHeap),
   VarPointsTo(iVar,instanceHeap).
in file /home/supun/phd/research/datalog/datalog/souffle-simple-pa/pointsto.dl [17:1-19:35])_");
            if (!(rel_13_VarPointsTo->empty()) &&
                !(rel_3_delta_VarPointsTo->empty())) {
                [&]() {
                    CREATE_OP_CONTEXT(rel_3_delta_VarPointsTo_op_ctxt,
                                      rel_3_delta_VarPointsTo->createContext());
                    CREATE_OP_CONTEXT(rel_13_VarPointsTo_op_ctxt,
                                      rel_13_VarPointsTo->createContext());
                    CREATE_OP_CONTEXT(rel_7_Alias_op_ctxt,
                                      rel_7_Alias->createContext());
                    CREATE_OP_CONTEXT(rel_4_new_Alias_op_ctxt,
                                      rel_4_new_Alias->createContext());
                    for (const auto& env0 : *rel_13_VarPointsTo) {
                        auto range =
                            rel_3_delta_VarPointsTo->lowerUpperRange_01(
                                Tuple<RamDomain, 2>{
                                    {ramBitCast<RamDomain>(MIN_RAM_SIGNED),
                                     ramBitCast(env0[1])}},
                                Tuple<RamDomain, 2>{
                                    {ramBitCast<RamDomain>(MAX_RAM_SIGNED),
                                     ramBitCast(env0[1])}},
                                READ_OP_CONTEXT(
                                    rel_3_delta_VarPointsTo_op_ctxt));
                        for (const auto& env1 : range) {
                            if (!(rel_7_Alias->contains(
                                    Tuple<RamDomain, 2>{{ramBitCast(env0[0]),
                                                         ramBitCast(env1[0])}},
                                    READ_OP_CONTEXT(rel_7_Alias_op_ctxt)))) {
                                Tuple<RamDomain, 2> tuple{
                                    {ramBitCast(env0[0]), ramBitCast(env1[0])}};
                                rel_4_new_Alias->insert(
                                    tuple,
                                    READ_OP_CONTEXT(rel_4_new_Alias_op_ctxt));
                            }
                        }
                    }
                }();
            }
            SECTION_END
            SECTIONS_END;
            if (rel_5_new_Assign->empty() && rel_6_new_VarPointsTo->empty() &&
                rel_4_new_Alias->empty())
                break;
            [&]() {
                CREATE_OP_CONTEXT(rel_8_Assign_op_ctxt,
                                  rel_8_Assign->createContext());
                CREATE_OP_CONTEXT(rel_5_new_Assign_op_ctxt,
                                  rel_5_new_Assign->createContext());
                for (const auto& env0 : *rel_5_new_Assign) {
                    Tuple<RamDomain, 2> tuple{
                        {ramBitCast(env0[0]), ramBitCast(env0[1])}};
                    rel_8_Assign->insert(tuple,
                                         READ_OP_CONTEXT(rel_8_Assign_op_ctxt));
                }
            }();
            std::swap(rel_2_delta_Assign, rel_5_new_Assign);
            rel_5_new_Assign->purge();
            [&]() {
                CREATE_OP_CONTEXT(rel_13_VarPointsTo_op_ctxt,
                                  rel_13_VarPointsTo->createContext());
                CREATE_OP_CONTEXT(rel_6_new_VarPointsTo_op_ctxt,
                                  rel_6_new_VarPointsTo->createContext());
                for (const auto& env0 : *rel_6_new_VarPointsTo) {
                    Tuple<RamDomain, 2> tuple{
                        {ramBitCast(env0[0]), ramBitCast(env0[1])}};
                    rel_13_VarPointsTo->insert(
                        tuple, READ_OP_CONTEXT(rel_13_VarPointsTo_op_ctxt));
                }
            }();
            std::swap(rel_3_delta_VarPointsTo, rel_6_new_VarPointsTo);
            rel_6_new_VarPointsTo->purge();
            [&]() {
                CREATE_OP_CONTEXT(rel_7_Alias_op_ctxt,
                                  rel_7_Alias->createContext());
                CREATE_OP_CONTEXT(rel_4_new_Alias_op_ctxt,
                                  rel_4_new_Alias->createContext());
                for (const auto& env0 : *rel_4_new_Alias) {
                    Tuple<RamDomain, 2> tuple{
                        {ramBitCast(env0[0]), ramBitCast(env0[1])}};
                    rel_7_Alias->insert(tuple,
                                        READ_OP_CONTEXT(rel_7_Alias_op_ctxt));
                }
            }();
            std::swap(rel_1_delta_Alias, rel_4_new_Alias);
            rel_4_new_Alias->purge();
            iter++;
        }
        iter = 0;
        rel_2_delta_Assign->purge();
        rel_5_new_Assign->purge();
        rel_3_delta_VarPointsTo->purge();
        rel_6_new_VarPointsTo->purge();
        rel_1_delta_Alias->purge();
        rel_4_new_Alias->purge();
        if (performIO) {
            try {
                std::map<std::string, std::string> directiveMap(
                    {{"IO", "file"},
                     {"attributeNames", "source\tdestination"},
                     {"name", "Assign"},
                     {"operation", "output"},
                     {"output-dir", "."},
                     {"params",
                      "{\"records\": {}, \"relation\": {\"arity\": 2, "
                      "\"auxArity\": 0, \"params\": [\"source\", "
                      "\"destination\"]}}"},
                     {"types",
                      "{\"ADTs\": {}, \"records\": {}, \"relation\": "
                      "{\"arity\": 2, \"auxArity\": 0, \"types\": "
                      "[\"s:Variable\", \"s:Variable\"]}}"}});
                if (!outputDirectory.empty()) {
                    directiveMap["output-dir"] = outputDirectory;
                }
                IOSystem::getInstance()
                    .getWriter(directiveMap, symTable, recordTable)
                    ->writeAll(*rel_8_Assign);
            } catch (std::exception& e) {
                std::cerr << e.what();
                exit(1);
            }
        }
        if (performIO) {
            try {
                std::map<std::string, std::string> directiveMap(
                    {{"IO", "file"},
                     {"attributeNames", "var\theap"},
                     {"name", "VarPointsTo"},
                     {"operation", "output"},
                     {"output-dir", "."},
                     {"params",
                      "{\"records\": {}, \"relation\": {\"arity\": 2, "
                      "\"auxArity\": 0, \"params\": [\"var\", \"heap\"]}}"},
                     {"types",
                      "{\"ADTs\": {}, \"records\": {}, \"relation\": "
                      "{\"arity\": 2, \"auxArity\": 0, \"types\": "
                      "[\"s:Variable\", \"s:Allocation\"]}}"}});
                if (!outputDirectory.empty()) {
                    directiveMap["output-dir"] = outputDirectory;
                }
                IOSystem::getInstance()
                    .getWriter(directiveMap, symTable, recordTable)
                    ->writeAll(*rel_13_VarPointsTo);
            } catch (std::exception& e) {
                std::cerr << e.what();
                exit(1);
            }
        }
        if (performIO) {
            try {
                std::map<std::string, std::string> directiveMap(
                    {{"IO", "file"},
                     {"attributeNames", "x\ty"},
                     {"name", "Alias"},
                     {"operation", "output"},
                     {"output-dir", "."},
                     {"params",
                      "{\"records\": {}, \"relation\": {\"arity\": 2, "
                      "\"auxArity\": 0, \"params\": [\"x\", \"y\"]}}"},
                     {"types",
                      "{\"ADTs\": {}, \"records\": {}, \"relation\": "
                      "{\"arity\": 2, \"auxArity\": 0, \"types\": "
                      "[\"s:Variable\", \"s:Variable\"]}}"}});
                if (!outputDirectory.empty()) {
                    directiveMap["output-dir"] = outputDirectory;
                }
                IOSystem::getInstance()
                    .getWriter(directiveMap, symTable, recordTable)
                    ->writeAll(*rel_7_Alias);
            } catch (std::exception& e) {
                std::cerr << e.what();
                exit(1);
            }
        }
        if (performIO) rel_9_AssignAlloc->purge();
        if (performIO) rel_8_Assign->purge();
        if (performIO) rel_11_PrimitiveAssign->purge();
        if (performIO) rel_10_Load->purge();
        if (performIO) rel_12_Store->purge();
        if (performIO) rel_13_VarPointsTo->purge();
        if (performIO) rel_7_Alias->purge();
    }
#ifdef _MSC_VER
#pragma warning(default : 4100)
#endif  // _MSC_VER
};
SouffleProgram* newInstance_output() { return new Sf_output; }
SymbolTable* getST_output(SouffleProgram* p) {
    return &reinterpret_cast<Sf_output*>(p)->symTable;
}

#ifdef __EMBEDDED_SOUFFLE__
class factory_Sf_output : public souffle::ProgramFactory {
    SouffleProgram* newInstance() { return new Sf_output(); };

   public:
    factory_Sf_output() : ProgramFactory("output") {}
};
extern "C" {
factory_Sf_output __factory_Sf_output_instance;
}
}
#else
}
int main(int argc, char** argv) {
    try {
        souffle::CmdOptions opt(R"(pointsto.dl)", R"()", R"()", false, R"()",
                                1);
        if (!opt.parse(argc, argv)) return 1;
        souffle::Sf_output obj;
#if defined(_OPENMP)
        obj.setNumThreads(opt.getNumJobs());

#endif
        obj.runAll(opt.getInputFileDir(), opt.getOutputFileDir());
        return 0;
    } catch (std::exception& e) {
        souffle::SignalHandler::instance()->error(e.what());
    }
}

#endif
