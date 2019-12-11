package app.engine.texture;

import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.Size;
import android.opengl.EGLContext;
import android.opengl.GLES20;
import android.util.Log;
import app.engine.AEHTMLVideoElementImpl;
import app.engine.AEHTMLVideoElementImpl.Stream;
import app.engine.egl.AEGLContext;
import app.engine.egl.AEGLProgram;

public class AECameraTexture implements Stream, SurfaceTexture.OnFrameAvailableListener {

	private static final String TAG = AECameraTexture.class.getSimpleName();

	private final float[] mMatrix = new float[16];

	private int		mFBO		= 0;
	private int		mTEX		= 0;
	private int		mWidth		= 0;
	private int		mHeight		= 0;
	private boolean	mIsDirty	= false;

	private String					mURL;
	private AEHTMLVideoElementImpl	mImpl;
	private Camera					mCamera;
	private EGLContext				mShared;
	private AEGLContext				mContext;
	private AEGLProgram				mProgram;
	private SurfaceTexture			mTexture;

	public AECameraTexture(AEHTMLVideoElementImpl impl, EGLContext shared, String url) {
		mURL = url;
		mImpl = impl;
		mShared = shared;
	}

	@Override
	public boolean open() {
		try {
			mContext = new AEGLContext(mShared);
			mContext.initWithOffScreen(1, 1);
			mContext.makeCurrent();

			mProgram = new AEGLProgram();
			mTexture = new SurfaceTexture(mProgram.texture());
			mTexture.setOnFrameAvailableListener(this);

			mCamera = Camera.open();
			Parameters params = mCamera.getParameters();
			params.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
			mCamera.setParameters(params);
			mCamera.setPreviewTexture(mTexture);
			mCamera.startPreview();

			Size size = mCamera.getParameters().getPreviewSize();
			mWidth = size.width;
			mHeight = size.height;
			mImpl.onPrepared(this, mWidth, mHeight, Integer.MAX_VALUE);
			return true;
		}
		catch (Exception e) {
			Log.e(TAG, "open camera exception.", e);
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
		if (mCamera != null) {
			mCamera.setPreviewCallback(null);
			mCamera.stopPreview();
			mCamera.release();
			mCamera = null;
		}

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
	public boolean getLoop() {
		return false;
	}

	@Override
	public void setLoop(boolean val) {
	}

	@Override
	public boolean getMuted() {
		return false;
	}

	@Override
	public void setMuted(boolean val) {
	}

	@Override
	public boolean getPaused() {
		return false;
	}

	@Override
	public void setPaused(boolean val) {
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
		return 0;
	}

	@Override
	public void setCurrentTime(int val) {
	}

	@Override
	public float getVolume() {
		return 1;
	}

	@Override
	public void setVolume(float val) {
	}

	@Override
	public float getPlaybackRate() {
		return 1;
	}

	@Override
	public void setPlaybackRate(float val) {
	}
}
