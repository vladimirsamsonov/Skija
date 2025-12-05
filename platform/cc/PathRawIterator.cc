#include <jni.h>
#include "SkPath.h"
#include "interop.hh"

extern "C" JNIEXPORT jlong JNICALL Java_io_github_humbleui_skija_PathRawIterator__1nMake
  (JNIEnv* env, jclass jclass, jlong pathPtr) {
    SkPath* path = reinterpret_cast<SkPath*>(static_cast<uintptr_t>(pathPtr));
    SkPath::RawIter* iter = new SkPath::RawIter(*path);
    return reinterpret_cast<jlong>(iter);
}

static void deletePathRawIterator(SkPath::RawIter* iter) {
    // std::cout << "Deleting [SkPathSegmentIterator " << path << "]" << std::endl;
    delete iter;
}

extern "C" JNIEXPORT jlong JNICALL Java_io_github_humbleui_skija_PathRawIterator__1nGetFinalizer
  (JNIEnv* env, jclass jclass) {
    return static_cast<jlong>(reinterpret_cast<uintptr_t>(&deletePathRawIterator));
}

extern "C" JNIEXPORT jobject JNICALL Java_io_github_humbleui_skija_PathRawIterator__1nNext
  (JNIEnv* env, jclass jclass, jlong ptr) {
    SkPath::RawIter* instance = reinterpret_cast<SkPath::RawIter*>(static_cast<uintptr_t>(ptr));
    SkPoint pts[4];
    SkPath::Verb verb = instance->next(pts);
    jobject segment;
    switch (verb) {
        case SkPath::Verb::kDone_Verb:
            segment = env->NewObject(skija::PathSegment::cls, skija::PathSegment::ctorDone);
            break;
        case SkPath::Verb::kMove_Verb:
        case SkPath::Verb::kClose_Verb:
            segment = env->NewObject(skija::PathSegment::cls, skija::PathSegment::ctorMoveClose, static_cast<jint>(verb), pts[0].fX, pts[0].fY, false);
            break;
        case SkPath::Verb::kLine_Verb:
            segment = env->NewObject(skija::PathSegment::cls, skija::PathSegment::ctorLine, pts[0].fX, pts[0].fY, pts[1].fX, pts[1].fY, false, false);
            break;
        case SkPath::Verb::kQuad_Verb:
            segment = env->NewObject(skija::PathSegment::cls, skija::PathSegment::ctorQuad, pts[0].fX, pts[0].fY, pts[1].fX, pts[1].fY, pts[2].fX, pts[2].fY, false);
            break;
        case SkPath::Verb::kConic_Verb:
            segment = env->NewObject(skija::PathSegment::cls, skija::PathSegment::ctorConic, pts[0].fX, pts[0].fY, pts[1].fX, pts[1].fY, pts[2].fX, pts[2].fY, instance->conicWeight(), false);
            break;
        case SkPath::Verb::kCubic_Verb:
            segment = env->NewObject(skija::PathSegment::cls, skija::PathSegment::ctorCubic, pts[0].fX, pts[0].fY, pts[1].fX, pts[1].fY, pts[2].fX, pts[2].fY, pts[3].fX, pts[3].fY, false);
            break;
    }
    return segment;
}
