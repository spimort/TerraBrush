#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H

#include <memory>

struct CancellationToken {
    std::shared_ptr<bool> isCancellationRequested = std::make_shared<bool>(false);
};

struct CancellationSource {
    CancellationToken token = CancellationToken();

    CancellationSource() {
        token = CancellationToken();
    }
    CancellationSource(CancellationToken &p_token) {
        token = p_token;
    }

    void cancel() {
        *token.isCancellationRequested = true;
    }
};
#endif