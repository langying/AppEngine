package app.engine.texture;

import android.graphics.SurfaceTexture;
import android.graphics.SurfaceTexture.OnFrameAvailableListener;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.opengl.EGLContext;
import android.opengl.GLES20;
import android.util.Log;
import android.view.Surface;
import app.engine.AEHTMLVideoElementImpl;
import app.engine.AEHTMLVideoElementImpl.Stream;
import app.engine.egl.AEGLContext;
import app.engine.egl.AEGLProgram;

public class AEVideoTexture implements Stream, OnErrorListener, OnPreparedListener, OnFrameAvailableListener {

	private static final String TAG = AEVideoTexture.class.getSimpleName();

	private final float[] mMatrix = new float[16];

	private int		mFBO		= 0;
	private int		mTEX		= 0;
	private int		mWidth		= 0;
	private int		mHeight		= 0;
	private float	mVolume		= 1;
	private boolean	mMuted		= false;
	private boolean	mIsDirty	= false;

	private String					mURL;
	private AEHTMLVideoElementImpl	mImpl;
	private EGLContext				mShared;
	private AEGLContext				mContext;
	private AEGLProgram				mProgram;
	private MediaPlayer				mMPlayer;
	private SurfaceTexture			mTexture;

	public AEVideoTexture(AEHTMLVideoElementImpl impl, EGLContext shared, String url) {
		mURL = url;
		mImpl = impl;
		mShared = shared;
	}

	@Override
	public boolean open() {
		mContext = new AEGLContext(mShared);
		mContext.initWithOffScreen(1, 1);
		mContext.makeCurrent();

		mProgram = new AEGLProgram();
		mTexture = new SurfaceTexture(mProgram.texture());
		mTexture.setOnFrameAvailableListener(this);

		Surface surface = new Surface(mTexture);
		mMPlayer = new MediaPlayer();
		mMPlayer.setSurface(surface);
		mMPlayer.setOnErrorListener(this);
		mMPlayer.setOnPreparedListener(this);
		mMPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
		surface.release();

		try {
			mMPlayer.setDataSource(mURL);
			mMPlayer.prepareAsync();
			return true;
		}
		catch (Exception e) {
			Log.e(TAG, "can't play video:" + mURL, e);
			return false;
		}
	}

	@Override
	public boolean isDirty() {
		return mIsDirty;
	}

	@Override
	public String currentURL() {
		return mURL;
	}

	@Override
	public void play() {
	}

	@Override
	public void stop() {
	}

	@Override
	public void close() {
		mMPlayer.stop();
		mMPlayer.release();
		mMPlayer = null;

		mContext.makeCurrent();
		if (mFBO != 0) {
			GLES20.glDeleteFramebuffers(1, new int[] { mFBO }, 0);
		}
		mContext.release();
		mContext = null;
		mImpl = null;
	}

	@Override
	public void texImage2D(int texId, int target, int level, int infmt, int format, int type) {
		mIsDirty = false;
		mContext.makeCurrent();
		mTexture.updateTexImage();
		mTexture.getTransformMatrix(mMatrix);

		if (mTEX != texId) {
			if (mFBO != 0) {
				GLES20.glDeleteFramebuffers(1, new int[] { mFBO }, 0);
				mFBO = 0;
			}
			mTEX = texId;
			GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, texId);
			GLES20.glTexImage2D(GLES20.GL_TEXTURE_2D, 0, GLES20.GL_RGB, mWidth, mHeight, 0, GLES20.GL_RGB, GLES20.GL_UNSIGNED_BYTE, null);

			int[] values = new int[1];
			GLES20.glGenFramebuffers(1, values, 0);
			GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, values[0]);
			GLES20.glFramebufferTexture2D(GLES20.GL_FRAMEBUFFER, GLES20.GL_COLOR_ATTACHMENT0, GLES20.GL_TEXTURE_2D, texId, 0);
			if (GLES20.glCheckFramebufferStatus(GLES20.GL_FRAMEBUFFER) != GLES20.GL_FRAMEBUFFER_COMPLETE) {
				Log.e(TAG, "texImage2D::glCheckFramebufferStatus failure.");
			}
			mFBO = values[0];
		}

		GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, mFBO);
		GLES20.glViewport(0, 0, mWidth, mHeight);
		mProgram.draw(mMatrix);
		GLES20.glFinish();
	}

	@Override
	public void texSubImage2D(int target, int level, int xoffset, int yoffset, int format, int type) {
	}

	@Override
	public void onFrameAvailable(SurfaceTexture surfaceTexture) {
		mIsDirty = true;
	}

	@Override
	public void onPrepared(MediaPlayer mp) {
		mp.setLooping(true);
		mp.start();

		mWidth = mp.getVideoWidth();
		mHeight = mp.getVideoHeight();
		mImpl.onPrepared(this, mWidth, mHeight, mp.getDuration());
	}

	@Override
	public boolean onError(MediaPlayer mp, int what, int extra) {
		Log.e(TAG, "onError: cant play video, code: " + what);
		return false;
	}

	@Override
	public boolean getLoop() {
		return mMPlayer.isLooping();
	}

	@Override
	public void setLoop(boolean val) {
		mMPlayer.setLooping(val);
	}

	@Override
	public boolean getMuted() {
		return mMuted;
	}

	@Override
	public void setMuted(boolean val) {
		mMuted = val;
		if (mMuted) {
			mMPlayer.setVolume(0, 0);
		}
		else {
			mMPlayer.setVolume(mVolume, mVolume);
		}
	}

	@Override
	public boolean getPaused() {
		return !mMPlayer.isPlaying();
	}

	@Override
	public void setPaused(boolean val) {
		if (val) {
			mMPlayer.pause();
		}
		else {
			mMPlayer.start();
		}
	}

	@Override
	public boolean getAutoplay() {
		return true;
	}

	@Override
	public void setAutoplay(boolean val) {
	}

	@Override
	public int getCurrentTime() {
		return mMPlayer.getCurrentPosition() / 1000;
	}

	@Override
	public void setCurrentTime(int val) {
		mMPlayer.seekTo(val * 1000);
	}

	@Override
	public float getVolume() {
		return mVolume;
	}

	@Override
	public void setVolume(float val) {
		mVolume = val;
		mMPlayer.setVolume(mVolume, mVolume);
	}

	@Override
	public float getPlaybackRate() {
		return 1;
	}

	@Override
	public void setPlaybackRate(float val) {
	}
}
