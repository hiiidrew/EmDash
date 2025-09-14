#include <cassert>
#include <queue>
#include <string>

class MockSerialDriver {
public:
    void send(const std::string &data) {
        for (char c : data) {
            buffer_.push(c);
        }
    }
    std::string receive(std::size_t len) {
        std::string out;
        while (len-- && !buffer_.empty()) {
            out += buffer_.front();
            buffer_.pop();
        }
        return out;
    }
private:
    std::queue<char> buffer_;
};

int main() {
    MockSerialDriver drv;
    const std::string msg = "HELLO";
    drv.send(msg);
    std::string out = drv.receive(msg.size());
    assert(out == msg);
    return 0;
}
