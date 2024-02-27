#ifndef SRC_HOMEWORK_9_ASYNC_HPP
#define SRC_HOMEWORK_9_ASYNC_HPP

#include <cstddef>

namespace homework_9::async {

using handle_t = void*;

handle_t connect(std::size_t bulk);
void receive(handle_t handle, const char *data, std::size_t size);
void disconnect(handle_t handle);

}

#endif /* SRC_HOMEWORK_9_ASYNC_HPP */
