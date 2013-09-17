/*
 * Copyright (C) 2013 Google Inc. All rights reserved.
 * Copyright (C) 2013 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY GOOGLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL GOOGLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "SourceInfo.h"

#if ENABLE(MEDIA_STREAM)

#include <wtf/NeverDestroyed.h>

namespace WebCore {

PassRefPtr<SourceInfo> SourceInfo::create(PassRefPtr<TrackSourceInfo> trackSourceInfo)
{
    return adoptRef(new SourceInfo(trackSourceInfo));
}

SourceInfo::SourceInfo(PassRefPtr<TrackSourceInfo> trackSourceInfo)
    : m_trackSourceInfo(trackSourceInfo)
{
}

const AtomicString& SourceInfo::kind() const
{
    static NeverDestroyed<AtomicString> none("none", AtomicString::ConstructFromLiteral);
    static NeverDestroyed<AtomicString> audioKind("audio", AtomicString::ConstructFromLiteral);
    static NeverDestroyed<AtomicString> videoKind("video", AtomicString::ConstructFromLiteral);

    switch (m_trackSourceInfo->kind()) {
    case TrackSourceInfo::Audio:
        return audioKind;
    case TrackSourceInfo::Video:
        return videoKind;
    case TrackSourceInfo::None:
        return none;
    }

    ASSERT_NOT_REACHED();
    return emptyAtom;
}

const AtomicString& SourceInfo::facing() const
{
    static NeverDestroyed<AtomicString> userFacing("user", AtomicString::ConstructFromLiteral);
    static NeverDestroyed<AtomicString> environmentFacing("environment", AtomicString::ConstructFromLiteral);
    static NeverDestroyed<AtomicString> leftFacing("left", AtomicString::ConstructFromLiteral);
    static NeverDestroyed<AtomicString> rightFacing("right", AtomicString::ConstructFromLiteral);

    switch (m_trackSourceInfo->facing()) {
    case TrackSourceInfo::None:
        return emptyAtom;
    case TrackSourceInfo::User:
        return userFacing;
    case TrackSourceInfo::Environment:
        return environmentFacing;
    case TrackSourceInfo::Left:
        return leftFacing;
    case TrackSourceInfo::Right:
        return rightFacing;
    }

    ASSERT_NOT_REACHED();
    return emptyAtom;
}

} // namespace WebCore

#endif
