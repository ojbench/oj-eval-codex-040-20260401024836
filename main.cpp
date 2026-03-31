#include <bits/stdc++.h>
using namespace std;

template <typename T>
class DoublyLinkedList {
private:
    struct Node {
        T value;
        shared_ptr<Node> next;
        weak_ptr<Node> prev;
        Node(const T& val) : value(val), next(nullptr) {}
    };

    shared_ptr<Node> head;
    weak_ptr<Node> tail;
    size_t length;

public:
    class iterator {
    private:
        shared_ptr<Node> current;

    public:
        iterator(shared_ptr<Node> node) : current(std::move(node)) {}

        T& operator*() { return current->value; }

        iterator operator++(int) {
            iterator tmp = *this;
            if (current) current = current->next;
            return tmp;
        }

        bool operator==(const iterator& other) const { return current.get() == other.current.get(); }
        bool operator!=(const iterator& other) const { return !(*this == other); }
    };

    DoublyLinkedList() : head(nullptr), length(0) {}

    bool empty() const { return length == 0; }
    size_t size() const { return length; }

    void push_front(const T& value) {
        auto node = make_shared<Node>(value);
        node->next = head;
        if (head) {
            head->prev = node;
        } else {
            tail = node; // first node, also tail
        }
        head = node;
        ++length;
    }

    void push_back(const T& value) {
        auto node = make_shared<Node>(value);
        auto t = tail.lock();
        if (!t) {
            head = node;
        } else {
            t->next = node;
            node->prev = t;
        }
        tail = node;
        ++length;
    }

    optional<T> pop_front() {
        if (!head) return nullopt;
        auto node = head;
        T val = std::move(node->value);
        head = node->next;
        if (head) {
            head->prev.reset();
        } else {
            tail.reset();
        }
        --length;
        return val;
    }

    optional<T> pop_back() {
        if (empty()) return nullopt;
        auto t = tail.lock();
        T val = std::move(t->value);
        auto prev = t->prev.lock();
        if (prev) {
            prev->next.reset();
            tail = prev;
        } else {
            head.reset();
            tail.reset();
        }
        --length;
        return val;
    }

    iterator begin() { return iterator(head); }
    iterator end() { return iterator(nullptr); }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Protocol: simple interpreter
    // Commands:
    // PF x -> push_front x
    // PB x -> push_back x
    // POFF -> pop_front, print value or NONE
    // POBK -> pop_back, print value or NONE
    // ITR -> iterate from begin to end, print space-separated values, or EMPTY
    // S -> print size
    // Q -> quit
    // All values treated as long long

    DoublyLinkedList<long long> dll;
    string cmd;
    while (cin >> cmd) {
        if (cmd == "PF") {
            long long x; cin >> x; dll.push_front(x);
        } else if (cmd == "PB") {
            long long x; cin >> x; dll.push_back(x);
        } else if (cmd == "POFF") {
            auto v = dll.pop_front();
            if (v) cout << *v << "\n"; else cout << "NONE\n";
        } else if (cmd == "POBK") {
            auto v = dll.pop_back();
            if (v) cout << *v << "\n"; else cout << "NONE\n";
        } else if (cmd == "ITR") {
            if (dll.empty()) { cout << "EMPTY\n"; continue; }
            bool first = true;
            for (auto it = dll.begin(); it != dll.end(); it++) {
                if (!first) cout << ' ';
                first = false;
                cout << *it;
            }
            cout << "\n";
        } else if (cmd == "S") {
            cout << dll.size() << "\n";
        } else if (cmd == "Q") {
            break;
        }
    }
    return 0;
}

