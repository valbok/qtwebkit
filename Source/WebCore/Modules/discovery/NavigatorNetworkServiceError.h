/*
 * Copyright (C) Canon Inc. 2014
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions
 * are required to be met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Canon Inc. nor the names of 
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY CANON INC. AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CANON INC. AND ITS CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NavigatorNetworkServiceError_h
#define NavigatorNetworkServiceError_h

#include <wtf/PassRefPtr.h>
#include <wtf/RefCounted.h>

#if ENABLE(DISCOVERY)

namespace WebCore {

class NavigatorNetworkServiceError : public RefCounted<NavigatorNetworkServiceError> {
public:
    enum ErrorCode {
        PERMISSION_DENIED_ERR = 1,
        UNKNOWN_TYPE_PREFIX_ERR = 2
    };

    static PassRefPtr<NavigatorNetworkServiceError> create(ErrorCode code)
    {
        return adoptRef(new NavigatorNetworkServiceError(code));
    }

    virtual ~NavigatorNetworkServiceError() { }

    ErrorCode code() const { return m_code; }

private:
    explicit NavigatorNetworkServiceError(ErrorCode code) 
        : m_code(code) { }

    ErrorCode m_code;
};

} // namespace WebCore

#endif // ENABLE(DISCOVERY)

#endif // NavigatorNetworkServiceError_h