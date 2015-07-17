/*
 * Copyright (C) 2007, 2009 Apple Inc.  All rights reserved.
 * Copyright (C) 2007 Collabora Ltd. All rights reserved.
 * Copyright (C) 2007 Alp Toker <alp@atoker.com>
 * Copyright (C) 2009, 2010 Igalia S.L
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * aint with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef MediaPlayerPrivateGStreamer_h
#define MediaPlayerPrivateGStreamer_h
#if ENABLE(VIDEO) && USE(GSTREAMER)

#include "GRefPtrGStreamer.h"
#include "MediaPlayerPrivateGStreamerBase.h"
#include "Timer.h"

#include <glib.h>
#include <gst/gst.h>
#include <gst/pbutils/install-plugins.h>

#if ENABLE(VIDEO_TRACK)
#include "AudioTrackPrivateGStreamer.h"
#include "InbandMetadataTextTrackPrivateGStreamer.h"
#include "InbandTextTrackPrivateGStreamer.h"
#include "VideoTrackPrivateGStreamer.h"
#endif

#include <wtf/Forward.h>

#if ENABLE(VIDEO_TRACK) && USE(GSTREAMER_MPEGTS)
#include <wtf/text/AtomicStringHash.h>
#endif

#if ENABLE(ENCRYPTED_MEDIA) || ENABLE(ENCRYPTED_MEDIA_V2)
#include <wtf/threads/BinarySemaphore.h>
#endif

typedef struct _GstBuffer GstBuffer;
typedef struct _GstMessage GstMessage;
typedef struct _GstElement GstElement;

namespace WebCore {

class MediaPlayerPrivateGStreamer : public MediaPlayerPrivateGStreamerBase {
public:
    ~MediaPlayerPrivateGStreamer();
    static void registerMediaEngine(MediaEngineRegistrar);
    void handleMessage(GstMessage*);
    void handleSyncMessage(GstMessage*);

    static MediaPlayer::SupportsType supportsType(const String& type, const String& codecs, const KURL&);

    void handlePluginInstallerResult(GstInstallPluginsReturn);

    bool hasVideo() const { return m_hasVideo; }
    bool hasAudio() const { return m_hasAudio; }

    void load(const String &url);
#if ENABLE(MEDIA_SOURCE)
    void load(const String& url, MediaSourcePrivateClient*);
#endif
    void commitLoad();
    void cancelLoad();

    void prepareToPlay();
    void play();
    void pause();

    bool paused() const;
    bool seeking() const;

    float duration() const;
    float currentTime() const;
    void seek(float);

    void setReadyState(MediaPlayer::ReadyState state);

    void setRate(float);
    void setPreservesPitch(bool);

    void setPreload(MediaPlayer::Preload);
    void fillTimerFired(Timer<MediaPlayerPrivateGStreamer>*);

    PassOwnPtr<PlatformTimeRanges> buffered() const;
    float maxTimeSeekable() const;
    bool didLoadingProgress() const;
    unsigned long long totalBytes() const;
    float maxTimeLoaded() const;

    void loadStateChanged();
    void timeChanged();
    void didEnd();
    void durationChanged();
    void loadingFailed(MediaPlayer::NetworkState);

    void videoChanged();
    void audioChanged();
    void notifyPlayerOfVideo();
    void notifyPlayerOfAudio();

#if ENABLE(VIDEO_TRACK)
    void textChanged();
    void notifyPlayerOfText();

    void newTextSample();
    void notifyPlayerOfNewTextSample();
#endif

    void sourceChanged();
    GstElement* audioSink() const;

    void setAudioStreamProperties(GObject*);

    void simulateAudioInterruption();

#if ENABLE(ENCRYPTED_MEDIA_V2)
    void needKey(RefPtr<Uint8Array>);
    void keyAdded();
#endif

#if ENABLE(ENCRYPTED_MEDIA) || ENABLE(ENCRYPTED_MEDIA_V2)
    void signalDRM();
#endif

    static void notifyDurationChanged(MediaPlayerPrivateGStreamer* instance);
    virtual bool isLiveStream() const { return m_isStreaming; }
#if ENABLE(MEDIA_SOURCE)
    void notifyAppendComplete();
#endif

private:
    MediaPlayerPrivateGStreamer(MediaPlayer*);

    static PassRefPtr<MediaPlayerPrivateInterface> create(MediaPlayer*);

    static void getSupportedTypes(HashSet<String>&);

#if ENABLE(ENCRYPTED_MEDIA) || ENABLE(ENCRYPTED_MEDIA_V2)
    static MediaPlayer::SupportsType extendedSupportsType(const String& type, const String& codecs, const String& keySystem, const KURL&);
    static void needKeyEventFromMain(void *invocation);
#endif
    static bool supportsKeySystem(const String& keySystem, const String& mimeType);

    static bool isAvailable();

    void updateAudioSink();
    void createAudioSink();

    float playbackPosition() const;

    void cacheDuration();
    void updateStates();
    void asyncStateChangeDone();

    void createGSTPlayBin();
    bool changePipelineState(GstState);

    bool loadNextLocation();
    void mediaLocationChanged(GstMessage*);

    void setDownloadBuffering();
    void processBufferingStats(GstMessage*);

#if ENABLE(VIDEO_TRACK) && USE(GSTREAMER_MPEGTS)
    void processMpegTsSection(GstMpegtsSection*);
#endif
#if ENABLE(VIDEO_TRACK)
    void processTableOfContents(GstMessage*);
    void processTableOfContentsEntry(GstTocEntry*, GstTocEntry* parent);
#endif

    bool doSeek(gint64 position, float rate, GstSeekFlags seekType);
    void updatePlaybackRate();

    virtual String engineDescription() const { return "GStreamer"; }
    virtual bool didPassCORSAccessCheck() const;

#if ENABLE(ENCRYPTED_MEDIA_V2)
    PassOwnPtr<CDMSession> createSession(const String&);
    CDMSession* m_cdmSession;
#endif

#if ENABLE(MEDIA_SOURCE)
    // TODO: Implement
    virtual unsigned long totalVideoFrames() { return 0; }
    virtual unsigned long droppedVideoFrames() { return 0; }
    virtual unsigned long corruptedVideoFrames() { return 0; }
    virtual MediaTime totalFrameDelay() { return MediaTime::zeroTime(); }
    virtual GRefPtr<GstCaps> currentDemuxerCaps() const OVERRIDE;
#endif

private:
    GRefPtr<GstElement> m_playBin;
    GRefPtr<GstElement> m_source;
#if ENABLE(VIDEO_TRACK)
    GRefPtr<GstElement> m_textAppSink;
    GRefPtr<GstPad> m_textAppSinkPad;
#endif
    float m_seekTime;
    bool m_changingRate;
    float m_endTime;
    mutable bool m_isStreaming;
    GstStructure* m_mediaLocations;
    int m_mediaLocationCurrentIndex;
    bool m_resetPipeline;
    bool m_paused;
    bool m_playbackRatePause;
    bool m_seeking;
    bool m_seekIsPending;
    float m_timeOfOverlappingSeek;
    bool m_canFallBackToLastFinishedSeekPosition;
    bool m_buffering;
    float m_playbackRate;
    float m_lastPlaybackRate;
    bool m_errorOccured;
    mutable gfloat m_mediaDuration;
    bool m_downloadFinished;
    Timer<MediaPlayerPrivateGStreamer> m_fillTimer;
    float m_maxTimeLoaded;
    int m_bufferingPercentage;
    MediaPlayer::Preload m_preload;
    bool m_delayingLoad;
    bool m_mediaDurationKnown;
    mutable float m_maxTimeLoadedAtLastDidLoadingProgress;
    bool m_volumeAndMuteInitialized;
    bool m_hasVideo;
    bool m_hasAudio;
    guint m_audioTimerHandler;
    guint m_videoTimerHandler;
    guint m_textTimerHandler;
    GRefPtr<GstElement> m_webkitAudioSink;
    mutable unsigned long long m_totalBytes;
    KURL m_url;
    bool m_preservesPitch;
    GstState m_requestedState;
    GRefPtr<GstElement> m_autoAudioSink;
    bool m_missingPlugins;
#if ENABLE(VIDEO_TRACK)
    Vector<RefPtr<AudioTrackPrivateGStreamer> > m_audioTracks;
    Vector<RefPtr<InbandTextTrackPrivateGStreamer> > m_textTracks;
    Vector<RefPtr<VideoTrackPrivateGStreamer> > m_videoTracks;
    RefPtr<InbandMetadataTextTrackPrivateGStreamer> m_chaptersTrack;
#endif
#if ENABLE(VIDEO_TRACK) && USE(GSTREAMER_MPEGTS)
    HashMap<AtomicString, RefPtr<InbandMetadataTextTrackPrivateGStreamer> > m_metadataTracks;
#endif
#if ENABLE(ENCRYPTED_MEDIA) || ENABLE(ENCRYPTED_MEDIA_V2)
    BinarySemaphore m_drmKeySemaphore;
#endif
#if ENABLE(MEDIA_SOURCE)
    RefPtr<MediaSourcePrivateClient> m_mediaSource;
    bool isMediaSource() const { return m_mediaSource; }
#else
    bool isMediaSource() const { return false; }
#endif

    Mutex m_pendingAsyncOperationsLock;
    GList* m_pendingAsyncOperations;
};
}

#endif // USE(GSTREAMER)
#endif
