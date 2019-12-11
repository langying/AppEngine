package app.engine;

import java.util.concurrent.CountDownLatch;

import android.opengl.EGL14;
import android.opengl.EGLContext;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Process;
import android.util.Log;
import app.engine.texture.AECameraTexture;
import app.engine.texture.AEVideoTexture;

public class AEHTMLVideoElementImpl {

	private static final String TAG = AEHTMLVideoElementImpl.class.getSimpleName();

	private final Object mLocker = new Object();

	private String			url;
	private long			mThiz;
	private AEView			mView;
	private Stream			mStream;
	private EGLContext		mShared;
	private HandlerThread	mThread;
	private Handler			mHandler;

	private native void onFinish(long thiz, int width, int height, int duration);

	public AEHTMLVideoElementImpl(long thiz, AEView view) {
		mThiz = thiz;
		mView = view;
		mShared = EGL14.eglGetCurrentContext();
		mThread = new HandlerThread(getClass().getSimpleName(), Process.THREAD_PRIORITY_FOREGROUND);
		mThread.start();
		mHandler = new Handler(mThread.getLooper());
	}

	public void release() {
		final CountDownLatch finished = new CountDownLatch(1);
		mHandler.post(new Runnable() {
			@Override
			public void run() {
				mStream.close();
				finished.countDown();
			}
		});
		try {
			finished.await();
			mThread.quit();
			mHandler = null;
		}
		catch (Exception e) {
			Log.e(TAG, "release: mLocker.wait() exception.", e);
		}
	}

	public void load() {
	}

	public void play() {
	}

	public void stop() {
	}

	public boolean getLoop() {
		return mStream.getLoop();
	}

	public void setLoop(final boolean loop) {
		mHandler.post(new Runnable() {
			@Override
			public void run() {
				mStream.setLoop(loop);
			}
		});
	}

	public boolean getMuted() {
		if (mStream == null) {
			return false;
		}
		else {
			return mStream.getMuted();
		}
	}

	public void setMuted(boolean muted) {
		if (mStream != null) {
			mStream.setMuted(muted);
		}
	}

	public boolean getAutoplay() {
		if (mStream == null) {
			return false;
		}
		else {
			return mStream.getAutoplay();
		}
	}

	public void setAutoplay(boolean autoplay) {
		if (mStream != null) {
			mStream.setAutoplay(autoplay);
		}
	}

	public int getCurrentTime() {
		if (mStream == null) {
			return 0;
		}
		else {
			return mStream.getCurrentTime();
		}
	}

	public void setCurrentTime(int currentTime) {
		if (mStream != null) {
			mStream.setCurrentTime(currentTime);
		}
	}

	public float getVolume() {
		if (mStream == null) {
			return mStream.getVolume();
		}
		else {
			return 1;
		}
	}

	public void setVolume(float volume) {
		if (mStream != null) {
			mStream.setVolume(volume);
		}
	}

	public float getPlaybackRate() {
		if (mStream == null) {
			return mStream.getPlaybackRate();
		}
		else {
			return 1;
		}
	}

	public void setPlaybackRate(float rate) {
		if (mStream != null) {
			mStream.setPlaybackRate(rate);
		}
	}

	public String getSrc() {
		return this.url;
	}

	public void setSrc(String src) {
		url = src != null ? src : "";

		final CountDownLatch finished = new CountDownLatch(1);
		mHandler.post(new Runnable() {
			@Override
			public void run() {
				if (url.startsWith("camera://")) {
					mStream = new AECameraTexture(AEHTMLVideoElementImpl.this, mShared, url);
				}
				else {
					mStream = new AEVideoTexture(AEHTMLVideoElementImpl.this, mShared, url);
				}
				mStream.open();
				finished.countDown();
			}
		});
		try {
			finished.await();
		}
		catch (Exception e) {
			Log.e(TAG, "setSrc: mLocker wait exception.", e);
		}
	}

	public void onPrepared(Stream stream, final int width, final int height, final int duration) {
		mView.queueEvent(new Runnable() {
			@Override
			public void run() {
				onFinish(mThiz, width, height, duration);
			}
		});
	}

	public void texImage2D(final int texId, final int target, final int level, final int infmt, final int format, final int type) {
		if (mStream == null || !mStream.isDirty()) {
			return;
		}

		synchronized (mLocker) {
			mHandler.post(new Runnable() {
				@Override
				public void run() {
					synchronized (mLocker) {
						mStream.texImage2D(texId, target, level, infmt, format, type);
						mLocker.notify();
					}
				}
			});
			try {
				mLocker.wait();
			}
			catch (Exception e) {
				Log.e(TAG, "texImage2D: mLocker wait exception.", e);
			}
		}
	}

	public void texSubImage2D(final int target, final int level, final int xoffset, final int yoffset, final int format, final int type) {
		if (mStream == null) {
			return;
		}
		synchronized (mLocker) {
			mHandler.post(new Runnable() {
				@Override
				public void run() {
					synchronized (mLocker) {
						mStream.texSubImage2D(target, level, xoffset, yoffset, format, type);
						mLocker.notify();
					}
				}
			});
			try {
				mLocker.wait();
			}
			catch (Exception e) {
				Log.e(TAG, "texImage2D: mLocker wait exception.", e);
			}
		}
	}

	public static interface Stream {

		public boolean open();

		public boolean isDirty();

		public String currentURL();

		public void play();

		public void stop();

		public void close();

		public void texImage2D(int texId, int target, int level, int infmt, int format, int type);

		public void texSubImage2D(int target, int level, int xoffset, int yoffset, int format, int type);

		/** --------------Getter_And_Setter_Methods--------------------- */
		public boolean getLoop();

		public void setLoop(boolean val);

		public boolean getMuted();

		public void setMuted(boolean val);

		public boolean getPaused();

		public void setPaused(boolean val);

		public boolean getAutoplay();

		public void setAutoplay(boolean val);

		public int getCurrentTime();

		public void setCurrentTime(int val);

		public float getVolume();

		public void setVolume(float val);

		public float getPlaybackRate();

		public void setPlaybackRate(float val);
	}
}
