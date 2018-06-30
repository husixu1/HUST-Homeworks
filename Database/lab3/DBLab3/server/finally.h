#ifndef FINALLY_H
#define FINALLY_H

#include <functional>

/// @brief Scope guard. Do cleanup jobs.
class Finally {
private:
    std::function<void()> whenDeleted;
public:
    Finally(std::function<void()> whenDeleted) {
        this->whenDeleted = whenDeleted;
    }
    ~Finally() {
        whenDeleted();
    }
};

#endif // FINALLY_H
