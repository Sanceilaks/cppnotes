#pragma once

#include <deque>
#include <functional>
#include <thread>

namespace executor {
	using callback_fn = std::function<void()>;

	namespace __internal {
		inline std::deque<callback_fn> callbacks;
	}

	inline void init_executor() {
		std::thread([]() {
			while (true) {
				if (!__internal::callbacks.empty()) {
					__internal::callbacks.back()();
					__internal::callbacks.pop_back();
				}
			}
		}).detach();
	}

	inline void execute(callback_fn fn) {
		__internal::callbacks.push_front(fn);
	}
}