#ifndef SMACTIONVIDEO_H
#define SMACTIONVIDEO_H

//#define VIDEO_QTAV

#ifdef VIDEO_QTAV
    #include "smactionvideoqtav.h"
    #define SMActionVideo SMActionVideoQtAV

#else
    #include "smactionvideonative.h"
    #define SMActionVideo SMActionVideoNative

#endif

#endif // SMACTIONVIDEO_H

