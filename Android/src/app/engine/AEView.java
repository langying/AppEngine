package app.engine;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.R.color;
import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.AsyncTask;
import android.os.Build;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.MotionEvent;

@SuppressLint("ClickableViewAccessibility")
public class AEView extends GLSurfaceView implements Renderer {

	static {
		System.loadLibrary("AppEngine");
	}

	private native void jsgc(long mgr);

	private native void delete(long mgr);

	private native long create(int width, int height, float scale, String agent);

	private native void loadURL(long mgr, String url);

	private native void applyTimer(long mgr);

	private native void applyResize(long mgr, int width, int height);

	private native void applyKeyboard(long mgr, int event, int keyCode);

	private native void touch(long mgr, int event, long[] ids, float[] pxs, float[] pys);

	private native void finishImage(long mgr, long img, String url, int width, int height);

	private native String evalute(long mgr, String script);

	private static final String TAG = AEView.class.getSimpleName();

	private static final int	KeyUp		= 1;
	private static final int	KeyDown		= 2;
	private static final int	MouseUp		= 3;
	private static final int	MouseOut	= 4;
	private static final int	MouseDown	= 5;
	private static final int	MouseMove	= 6;
	private static final int	TouchEnd	= 7;
	private static final int	TouchMove	= 8;
	private static final int	TouchStart	= 9;
	private static final int	TouchCancel	= 10;

	private int			mFPS		= 0;
	private long		mMgr		= 0;
	private long		mFPSTime	= 0;
	private boolean		mNeedInit	= true;
	private String		mURL;
	private Callback	mCallback;

	public AEView(Context context) {
		super(context);
		initEGLContext();
	}

	public AEView(Context context, AttributeSet attrs) {
		super(context, attrs);
		initEGLContext();
	}

	protected void initEGLContext() {
		keepScreenOn(true);
		setEGLContextClientVersion(2);
		setFocusableInTouchMode(true);
		setBackgroundColor(color.white);
		setPreserveEGLContextOnPause(true);
		setEGLConfigChooser(8, 8, 8, 8, 24, 8);
		setRenderer(this);
	}

	@Override
	public void onResume() {
		super.onResume();
		setRenderMode(RENDERMODE_CONTINUOUSLY);
	}

	@Override
	public void onPause() {
		super.onPause();
		setRenderMode(RENDERMODE_WHEN_DIRTY);
	}

	public void onDestroy() {
		delete(mMgr);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		String agent = String.format("%s/1.0 (%s; %s %s)", "AppEngine", "Android", Build.MODEL, Build.VERSION.RELEASE);
		mMgr = create(width, height, 1, agent);
	}

	@Override
	public void onDrawFrame(GL10 gl) {
		if (mNeedInit) {
			mNeedInit = false;
			loadURL(mMgr, mURL);
		}
		else {
			long current = System.currentTimeMillis();
			if (current - mFPSTime >= 1000) {
				if (mCallback != null) {
					mCallback.onFPS(mFPS);
				}
				mFPS = 0;
				mFPSTime = current;
			}
			mFPS++;
			applyTimer(mMgr);
		}
	}

	@Override
	public boolean onTouchEvent(final MotionEvent event) {
		int action = event.getAction() & MotionEvent.ACTION_MASK;
		switch (action) {
			case MotionEvent.ACTION_DOWN:
			case MotionEvent.ACTION_POINTER_DOWN: {
				int idx = 0;
				if (action == MotionEvent.ACTION_POINTER_DOWN) {
					idx = event.getAction() >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
				}
				final long[] _ids = { event.getPointerId(idx) };
				final float[] _pxs = { event.getX(idx) };
				final float[] _pys = { event.getY(idx) };
				queueEvent(new Runnable() {
					@Override
					public void run() {
						touch(mMgr, TouchStart, _ids, _pxs, _pys);
					}
				});
				break;
			}
			case MotionEvent.ACTION_MOVE: {
				final int num = event.getPointerCount();
				final long[] ids = new long[num];
				final float[] pxs = new float[num];
				final float[] pys = new float[num];
				for (int idx = 0; idx < num; idx++) {
					ids[idx] = event.getPointerId(idx);
					pxs[idx] = event.getX(idx);
					pys[idx] = event.getY(idx);
				}
				queueEvent(new Runnable() {
					@Override
					public void run() {
						touch(mMgr, TouchMove, ids, pxs, pys);
					}
				});
				break;
			}
			case MotionEvent.ACTION_UP:
			case MotionEvent.ACTION_POINTER_UP: {
				int idx = 0;
				if (action == MotionEvent.ACTION_POINTER_UP) {
					idx = event.getAction() >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;
				}
				final long[] _ids = { event.getPointerId(idx) };
				final float[] _pxs = { event.getX(idx) };
				final float[] _pys = { event.getY(idx) };
				queueEvent(new Runnable() {
					@Override
					public void run() {
						touch(mMgr, TouchEnd, _ids, _pxs, _pys);
					}
				});
				break;
			}
			case MotionEvent.ACTION_CANCEL: {
				final int num = event.getPointerCount();
				final long[] ids = new long[num];
				final float[] pxs = new float[num];
				final float[] pys = new float[num];
				for (int idx = 0; idx < num; idx++) {
					ids[idx] = event.getPointerId(idx);
					pxs[idx] = event.getX(idx);
					pys[idx] = event.getY(idx);
				}
				queueEvent(new Runnable() {
					@Override
					public void run() {
						touch(mMgr, TouchCancel, ids, pxs, pys);
					}
				});
				break;
			}
		}
		return true;
	}

	@Override
	public boolean onKeyUp(final int keyCode, KeyEvent event) {
		switch (keyCode) {
			// case KeyEvent.KEYCODE_BACK:
			case KeyEvent.KEYCODE_MENU:
			case KeyEvent.KEYCODE_DPAD_LEFT:
			case KeyEvent.KEYCODE_DPAD_RIGHT:
			case KeyEvent.KEYCODE_DPAD_UP:
			case KeyEvent.KEYCODE_DPAD_DOWN:
			case KeyEvent.KEYCODE_ENTER:
			case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
			case KeyEvent.KEYCODE_DPAD_CENTER: {
				queueEvent(new Runnable() {
					@Override
					public void run() {
						applyKeyboard(mMgr, KeyUp, keyCode);
					}
				});
				return true;
			}
			default: {
				return super.onKeyUp(keyCode, event);
			}
		}
	}

	@Override
	public boolean onKeyDown(final int keyCode, KeyEvent event) {
		switch (keyCode) {
			// case KeyEvent.KEYCODE_BACK:
			case KeyEvent.KEYCODE_MENU:
			case KeyEvent.KEYCODE_DPAD_LEFT:
			case KeyEvent.KEYCODE_DPAD_RIGHT:
			case KeyEvent.KEYCODE_DPAD_UP:
			case KeyEvent.KEYCODE_DPAD_DOWN:
			case KeyEvent.KEYCODE_ENTER:
			case KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE:
			case KeyEvent.KEYCODE_DPAD_CENTER: {
				queueEvent(new Runnable() {
					@Override
					public void run() {
						applyKeyboard(mMgr, KeyDown, keyCode);
					}
				});
				return true;
			}
			default: {
				return super.onKeyDown(keyCode, event);
			}
		}
	}

	public void evalute(final String text) {
		queueEvent(new Runnable() {
			@Override
			public void run() {
				evalute(mMgr, text);
			}
		});
	}

	public void onLowMemory() {
		queueEvent(new Runnable() {
			@Override
			public void run() {
				jsgc(mMgr);
			}
		});
	}

	public void loadURL(String url) {
		mURL = url;
	}

	/** fps callback */
	public void setCallback(Callback callback) {
		mCallback = callback;
	}

	/** fps callback */
	public static interface Callback {
		public void onFPS(int fps);
	}

	public void alert(final String text) {
		post(new Runnable() {
			@Override
			public void run() {
				AlertDialog.Builder builder = new AlertDialog.Builder(getContext());
				builder.setMessage(text);
				builder.setNegativeButton("好", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						dialog.dismiss();
					}
				});
				builder.create().show();
			}
		});
	}

	public void keepScreenOn(final boolean on) {
		post(new Runnable() {
			@Override
			public void run() {
				setKeepScreenOn(on);
			}
		});
	}

	public void delItem(String key) {
		SharedPreferences preferences = getContext().getSharedPreferences(TAG, Context.MODE_PRIVATE);
		Editor editor = preferences.edit();
		editor.remove(key);
		editor.commit();
	}

	public String getItem(String key) {
		SharedPreferences preferences = getContext().getSharedPreferences(TAG, Context.MODE_PRIVATE);
		return preferences.getString(key, "");
	}

	public void setItem(String key, String value) {
		SharedPreferences preferences = getContext().getSharedPreferences(TAG, Context.MODE_PRIVATE);
		Editor editor = preferences.edit();
		editor.putString(key, value);
		editor.commit();
	}

	public String textWithURL(String url) {
		byte[] data = dataWithURL(url);
		String text = new String(data);
		return text;
	}

	public void onloadImage(final long mgr, final long img, final String src) {
		if (src != null && src.startsWith("data:")) {
			try {
				InputStream inStream = AEApp.inStreamWithURL(getContext(), src);
				BitmapFactory.Options options = new BitmapFactory.Options();
				options.inJustDecodeBounds = true;
				BitmapFactory.decodeStream(inStream, null, options);
				inStream.close();

				finishImage(mgr, img, src, options.outWidth, options.outHeight);
			}
			catch (Exception e) {
				Log.e(TAG, "decodeFile eror:" + src, e);
			}
			return;
		}
		new AsyncTask<String, Integer, String>() {
			@Override
			protected String doInBackground(String... params) {
				return cachedFileWithURL(src);
			}

			@Override
			protected void onPostExecute(final String file) {
				queueEvent(new Runnable() {
					@Override
					public void run() {
						try {
							InputStream inStream = AEApp.inStreamWithURL(getContext(), file);
							BitmapFactory.Options options = new BitmapFactory.Options();
							options.inJustDecodeBounds = true;
							BitmapFactory.decodeStream(inStream, null, options);
							inStream.close();

							finishImage(mgr, img, src, options.outWidth, options.outHeight);
						}
						catch (Exception e) {
							Log.e(TAG, "decodeFile eror:" + file, e);
						}
					}
				});
			}
		}.execute();
	}

	public Bitmap bitmapWithURL(String url) {
		try {
			BitmapFactory.Options options = new BitmapFactory.Options();
			options.inPurgeable = true;
			options.inInputShareable = true;
			options.inPreferredConfig = Bitmap.Config.ARGB_8888;

			url = cachedFileWithURL(url);
			InputStream inStream = AEApp.inStreamWithURL(getContext(), url);
			Bitmap bitmap = BitmapFactory.decodeStream(inStream, null, options);
			inStream.close();
			return bitmap;
		}
		catch (Exception e) {
			Log.e(TAG, "AEView.asyncBitmap:" + url, e);
		}
		return null;
	}

	private byte[] dataWithURL(String name) {
		byte[] buff = { 0 };
		InputStream inStream = null;
		ByteArrayOutputStream otStream = null;
		try {
			inStream = AEApp.inStreamWithURL(getContext(), name);
			otStream = new ByteArrayOutputStream();
			byte[] buffer = new byte[1024];
			for (int len = -1; (len = inStream.read(buffer)) != -1;) {
				otStream.write(buffer, 0, len);
			}
			buff = otStream.toByteArray();
		}
		catch (Exception e) {
			Log.e(TAG, "AEView.dataWithURL:" + name, e);
		}
		finally {
			AEApp.close(inStream, otStream);
		}
		return buff;
	}

	private String cachedFileWithURL(String src) {
		String url = AEApp.absoluteURL(getContext(), src);
		if (url.startsWith("file:") || url.startsWith("data:")) {
			return url;
		}
		String md5 = AEApp.MD5(url);
		File file = new File(getContext().getCacheDir(), md5);
		if (!file.exists()) {
			saveURL2File(url, file);
		}
		return file.getAbsolutePath();
	}

	private void saveURL2File(String url, File file) {
		InputStream inStream = null;
		OutputStream otStream = null;
		try {
			byte[] buffer = new byte[1024];
			inStream = AEApp.inStreamWithURL(getContext(), url);
			otStream = new FileOutputStream(file);
			for (int len = -1; (len = inStream.read(buffer)) != -1;) {
				otStream.write(buffer, 0, len);
			}
		}
		catch (Exception e) {
			Log.e(TAG, "AEView.saveURL2File:" + url, e);
		}
		finally {
			AEApp.close(inStream, otStream);
		}
	}
}
