package io.github.humbleui.skija;

import java.util.*;
import org.jetbrains.annotations.*;
import io.github.humbleui.skija.impl.*;

@ApiStatus.Internal
public class PathRawIterator extends Managed implements Iterator<PathSegment> {
    static { Library.staticLoad(); }
    
    public final Path _path;
    public PathSegment _nextSegment;

    @Override
    public PathSegment next() {
        try {
            if (_nextSegment._verb == PathVerb.DONE)
                throw new NoSuchElementException();
            PathSegment res = _nextSegment;
            _nextSegment = _nNext(_ptr);
            return res;
        } finally {
            ReferenceUtil.reachabilityFence(this);
        }
    }

    @Override
    public boolean hasNext() {
        return _nextSegment._verb != PathVerb.DONE;
    }

    public PathVerb peek(){
 	    return _nextSegment._verb;
    }
    public float getConicWeight(){
        return _nextSegment._conicWeight;
    }

    @ApiStatus.Internal
    public PathRawIterator(Path path, long ptr) {
        super(ptr, _nGetFinalizer());
        this._path = path;
        Stats.onNativeCall();
    }

    public static PathRawIterator make(Path path) {
        try {
            long ptr = _nMake(Native.getPtr(path));
            PathRawIterator i = new PathRawIterator(path, ptr);
            i._nextSegment = _nNext(ptr);
            return i;
        } finally {
            ReferenceUtil.reachabilityFence(path);
        }
    }


    @ApiStatus.Internal
    public static class _FinalizerHolder {
        public static final long PTR = _nGetFinalizer();
    }

    public static native long _nMake(long pathPtr);
    public static native long _nGetFinalizer();
    public static native PathSegment _nNext(long ptr);
}
