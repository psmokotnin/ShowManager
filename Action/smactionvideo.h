#ifndef SMACTIONVIDEO_H
#define SMACTIONVIDEO_H

#ifdef VIDEO_QTAV
    #include "smactionvideoqtav.h"
    #define SMActionVideo SMActionVideoQtAV

#else
#ifdef VIDEO_VLC
    #include "smactionvideovlc.h"
    #define SMActionVideo SMActionVideoVLC

#else
    #include "smactionvideonative.h"
    #define SMActionVideo SMActionVideoNative

#endif
#endif
#endif // SMACTIONVIDEO_H

