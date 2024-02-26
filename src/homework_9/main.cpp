#include "async.hpp"

int main(int, char *[]) {
  std::size_t bulk = 5;
  auto *h = homework_9::async::connect(bulk);
  auto *h2 = homework_9::async::connect(bulk);
  homework_9::async::receive(h, "1", 1);
  homework_9::async::receive(h2, "1\n", 2);
  homework_9::async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
  homework_9::async::receive(h, "b\nc\nd\n}\n89\n", 11);
  homework_9::async::disconnect(h);
  homework_9::async::disconnect(h2);
}