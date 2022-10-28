#include <iostream>
#include "timer.hpp"
#include "fast_allocator.hpp"

template<typename __allocator>
class segment_tree_t { 
    struct node_t {
        node_t(int64_t _sum = 0, uint64_t _left = 0, uint64_t _right = 0) 
                : sum(_sum), left(_left), right(_right) {}

        int64_t sum = 0;
        uint64_t left = 0;
        uint64_t right = 0;
        node_t* left_node = nullptr;
        node_t* right_node = nullptr;
    };

public:
    segment_tree_t(uint64_t _size) 
        : size_(_size), root_(0, 0, _size - 1) {
    }

    void apply(uint64_t _pos, int64_t _delta) {
        apply(_pos, _delta, &root_);
    }

    int64_t get_sum(uint64_t _left, uint64_t _right) {
        return get_sum(_left, _right, &root_);
    }

private:
    uint64_t get_mid(node_t* _node) {
        return _node->left + ((_node->right - _node->left) >> 1);
    }

    void init_childs(node_t* _node) {
        uint64_t mid = get_mid(_node);
        if (_node->right_node == nullptr) {
            _node->right_node = allocator_.allocate(1);
            allocator_.construct(_node->right_node, 0, mid + 1, _node->right);
        }

        if (_node->left_node == nullptr) {
            _node->left_node = allocator_.allocate(1);
            allocator_.construct(_node->left_node, 0, _node->left, mid);
        }
    }

    void apply(uint64_t _pos, int64_t _delta, node_t* _root) {
        if (_root->left == _root->right) {
            _root->sum += _delta;
        } else {
            init_childs(_root);
            uint64_t mid = get_mid(_root);
            if (_pos <= mid) {
                apply(_pos, _delta, _root->left_node);
            } else {
                apply(_pos, _delta, _root->right_node);
            }
            _root->sum = _root->left_node->sum + _root->right_node->sum;
        }
    }

    int64_t get_sum(uint64_t _left, uint64_t _right, node_t* _root) {
        if (_root->left == _left && _root->right == _right) {
            return _root->sum;
        } else {
            init_childs(_root);
            uint64_t mid = get_mid(_root);
            if (_right <= mid) {
                return get_sum(_left, _right, _root->left_node);
            } else if (mid < _left) {
                return get_sum(_left, _right, _root->right_node);
            } else {
                return get_sum(_left, mid, _root->left_node) +
                       get_sum(mid + 1, _right, _root->right_node);
            }
        }
    }

private:
    node_t root_;
    size_t size_{1};
    using allocator_t = typename __allocator::template rebind<node_t>::other;
    allocator_t allocator_;

};

signed main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    stat_timer_t timer;
    timer.start();

    uint64_t n, t;
    std::cin >> n >> t;

    // using allocator_t = std::allocator<uint64_t>;
    using allocator_t = fast_allocator_t<uint64_t>;
    segment_tree_t<allocator_t> tree(n);


    while (t--) {
        char q;
        std::cin >> q;
        if (q == '+') {
            uint64_t pos;
            int64_t delta;
            std::cin >> pos >> delta;
            tree.apply(pos - 1, delta);
        } else if (q == '?') {
            uint64_t left, right;
            std::cin >> left >> right;
            std::cout << tree.get_sum(left - 1, right - 1) << '\n';
        }
    }

    auto ts = timer.stop();
    std::cerr << "time: " << ts / 1'000'000'000 << " seconds, " << ts % 1'000'000'000 << " nanoseconds."<< '\n';

    return 0;
}