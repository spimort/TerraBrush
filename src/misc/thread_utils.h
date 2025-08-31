#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H

struct CancellationToken {
    bool isCancellationRequested = false;
};

struct CancellationSource {
    CancellationToken token = CancellationToken();

    CancellationSource() {
        token = CancellationToken();
    }
    CancellationSource(CancellationToken p_token) {
        token = p_token;
    }

    void cancel() {
        token.isCancellationRequested = true;
    }
};
#endif