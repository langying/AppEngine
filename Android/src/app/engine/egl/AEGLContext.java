package app.engine.egl;

import static android.opengl.EGL14.EGL_CONTEXT_CLIENT_VERSION;
import static android.opengl.EGL14.EGL_NONE;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

import android.graphics.Bitmap;
import android.opengl.EGL14;
import android.opengl.EGLConfig;
import android.opengl.EGLContext;
import android.opengl.EGLDisplay;
import android.opengl.EGLSurface;
import android.opengl.GLES20;
import android.util.Log;

public final class AEGLContext {

	private static final String TAG = AEGLContext.class.getSimpleName();

	private static final int EGL_OPENGL_ES3_BIT_KHR = 0x0040;

	private EGLConfig	mEGLConfig	= null;
	private EGLDisplay	mEGLDisplay	= EGL14.EGL_NO_DISPLAY;
	private EGLContext	mEGLContext	= EGL14.EGL_NO_CONTEXT;
	private EGLSurface	mEGLSurface	= EGL14.EGL_NO_SURFACE;

	public AEGLContext(EGLContext sharedContext) {
		do {
			if (sharedContext == null) {
				sharedContext = EGL14.EGL_NO_CONTEXT;
			}

			mEGLDisplay = EGL14.eglGetDisplay(EGL14.EGL_DEFAULT_DISPLAY);
			if (mEGLDisplay == EGL14.EGL_NO_DISPLAY) {
				Log.e(TAG, "[eglGetDisplay failure.]");
				break;
			}
			int[] values = new int[2];
			if (!EGL14.eglInitialize(mEGLDisplay, values, 0, values, 1)) {
				mEGLDisplay = null;
				Log.e(TAG, "[eglInitialize failure.]");
				break;
			}

			mEGLConfig = getConfig(2);
			if (mEGLConfig != null) {
				mEGLContext = EGL14.eglCreateContext(mEGLDisplay, mEGLConfig, sharedContext, new int[] { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE }, 0);
				checkEglError("eglCreateContext 2.0");
			}
			else {
				mEGLConfig = getConfig(3);
				mEGLContext = EGL14.eglCreateContext(mEGLDisplay, mEGLConfig, sharedContext, new int[] { EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE }, 0);
				checkEglError("eglCreateContext 3.0");
			}
		}
		while (false);
	}

	public void initWithSurface(Object surface) {
		int[] attribs = {					//
				EGL14.EGL_NONE				//
		};
		mEGLSurface = EGL14.eglCreateWindowSurface(mEGLDisplay, mEGLConfig, surface, attribs, 0);
		checkEglError("initWithSurface");
		if (mEGLSurface == null) {
			Log.e(TAG, "eglCreateWindowSurface return null.");
		}
	}

	public void initWithOffScreen(int width, int height) {
		int[] attribs = {					//
				EGL14.EGL_WIDTH, width,		//
				EGL14.EGL_HEIGHT, height,	//
				EGL14.EGL_NONE				//
		};
		mEGLSurface = EGL14.eglCreatePbufferSurface(mEGLDisplay, mEGLConfig, attribs, 0);
		checkEglError("initWithOffScreen");
		if (mEGLSurface == null) {
			Log.e(TAG, "eglCreatePbufferSurface return null.");
		}
	}

	public void release() {
		if (mEGLDisplay != EGL14.EGL_NO_DISPLAY) {
			EGL14.eglMakeCurrent(mEGLDisplay, EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_CONTEXT);
			EGL14.eglDestroySurface(mEGLDisplay, mEGLSurface);
			EGL14.eglDestroyContext(mEGLDisplay, mEGLContext);
			EGL14.eglTerminate(mEGLDisplay);
			EGL14.eglReleaseThread();
		}

		mEGLConfig = null;
		mEGLContext = EGL14.EGL_NO_CONTEXT;
		mEGLDisplay = EGL14.EGL_NO_DISPLAY;
		mEGLSurface = EGL14.EGL_NO_SURFACE;
	}

	public void swapBuffers() {
		EGL14.eglSwapBuffers(mEGLDisplay, mEGLSurface);
	}

	public void makeCurrent() {
		EGL14.eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext);
	}

	public void makeNothingCurrent() {
		EGL14.eglMakeCurrent(mEGLDisplay, EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_SURFACE, EGL14.EGL_NO_CONTEXT);
	}

	public boolean isCurrent() {
		EGLContext context = EGL14.eglGetCurrentContext();
		EGLSurface surface = EGL14.eglGetCurrentSurface(EGL14.EGL_DRAW);
		return mEGLContext.equals(context) && mEGLSurface.equals(surface);
	}

	public String queryString(int what) {
		return EGL14.eglQueryString(mEGLDisplay, what);
	}

	public int querySurface(EGLSurface eglSurface, int what) {
		int[] value = new int[1];
		EGL14.eglQuerySurface(mEGLDisplay, eglSurface, what, value, 0);
		return value[0];
	}

	private void checkEglError(String msg) {
		int error;
		if ((error = EGL14.eglGetError()) != EGL14.EGL_SUCCESS) {
			throw new RuntimeException(msg + ": EGL error: 0x" + Integer.toHexString(error));
		}
	}

	private EGLConfig getConfig(int version) {
		int type = EGL14.EGL_OPENGL_ES2_BIT;
		if (version >= 3) {
			type |= EGL_OPENGL_ES3_BIT_KHR;
		}
		int[] AttrsList = {							//
				EGL14.EGL_RED_SIZE, 8,				//
				EGL14.EGL_GREEN_SIZE, 8,			//
				EGL14.EGL_BLUE_SIZE, 8,				//
				EGL14.EGL_ALPHA_SIZE, 8,			//
				EGL14.EGL_RENDERABLE_TYPE, type,	//
				EGL14.EGL_NONE						//
		};
		int[] numbs = new int[1];
		EGLConfig[] configs = new EGLConfig[1];
		if (EGL14.eglChooseConfig(mEGLDisplay, AttrsList, 0, configs, 0, configs.length, numbs, 0)) {
			return configs[0];
		}
		Log.w(TAG, String.format("getConfig(%d) failure.", version));
		return null;
	}

	public void saveFrame(File file, int width, int height) throws IOException {
		if (!isCurrent()) {
			Log.e(TAG, "Expected EGL context/surface is not current");
		}

		String filename = file.toString();
		ByteBuffer buff = ByteBuffer.allocateDirect(width * height * 4);
		buff.order(ByteOrder.LITTLE_ENDIAN);
		GLES20.glReadPixels(0, 0, width, height, GLES20.GL_RGBA, GLES20.GL_UNSIGNED_BYTE, buff);
		checkEglError("glReadPixels");
		buff.rewind();

		BufferedOutputStream bos = null;
		try {
			bos = new BufferedOutputStream(new FileOutputStream(filename));
			Bitmap bmp = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
			bmp.copyPixelsFromBuffer(buff);
			bmp.compress(Bitmap.CompressFormat.PNG, 90, bos);
			bmp.recycle();
		}
		finally {
			if (bos != null) {
				bos.close();
			}
		}
		Log.i(TAG, "Saved " + width + "x" + height + " frame as '" + filename + "'");
	}
}
