#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H

struct CancellationToken {
    bool isCancellationRequested;
};

struct CancellationSource {
    CancellationToken token;

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