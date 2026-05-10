#include "thread_utils.h"

using namespace godot;

void CancellationToken::_bind_methods() {}
CancellationToken::CancellationToken() {}
CancellationToken::~CancellationToken() {}

bool CancellationToken::isCancellationRequested() const {
    return _isCancellationRequested;
}

void CancellationToken::requestCancel() {
    _isCancellationRequested = true;
}

void CancellationSource::_bind_methods() {}
CancellationSource::CancellationSource() {
    _token = memnew(CancellationToken);
}
CancellationSource::~CancellationSource() {}

void CancellationSource::cancel() {
    _token->requestCancel();
}

Ref<CancellationToken> CancellationSource::getToken() const {
    return _token;
}