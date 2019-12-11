package app.engine;

import java.io.IOException;
import java.io.InputStream;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Align;
import android.graphics.Paint.Style;
import android.graphics.PorterDuff.Mode;
import android.graphics.Rect;
import android.graphics.Typeface;
import android.text.TextPaint;
import android.util.Log;

public class AECanvasRenderingContext2DImpl {
	private static final String TAG = AECanvasRenderingContext2DImpl.class.getSimpleName();

	private AEView		_view;
	private TextPaint	_paint;
	private Bitmap		_bitmap;
	private Canvas		_canvas;

	@Override
	protected void finalize() throws Throwable {
		Log.e(TAG, "AEBitmap::finalize");
		_bitmap.recycle();
		_view = null;
		_paint = null;
		_bitmap = null;
		_canvas = null;
		super.finalize();
	}

	public AECanvasRenderingContext2DImpl(AEView view, int width, int height) {
		_view = view;
		_paint = new TextPaint(Paint.ANTI_ALIAS_FLAG);
		_paint.setTextSize(16);
		_paint.setColor(Color.TRANSPARENT);
		resize(width, height);
	}

	public Bitmap bitmap() {
		return _bitmap;
	}

	public void resize(int width, int height) {
		if (_bitmap != null) {
			_bitmap.recycle();
		}
		_bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
		_canvas = new Canvas(_bitmap);
		_canvas.drawColor(Color.TRANSPARENT, Mode.CLEAR);
	}

	public void clearRect(int x, int y, int w, int h) {
		int color = _paint.getColor();
		_paint.setColor(Color.TRANSPARENT);
		_canvas.drawRect(x, y, x + w, y + h, _paint);
		_paint.setColor(color);
	}

	public void drawImage(String url, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh) {
		try {
			InputStream inStream = AEApp.inStreamWithURL(_view.getContext(), url);
			Bitmap bitmap = BitmapFactory.decodeStream(inStream);
			inStream.close();
			Rect src = new Rect(sx, sy, sx + sw, sy + sh);
			Rect dst = new Rect(dx, dy, dx + dw, dy + dh);
			_canvas.drawBitmap(bitmap, src, dst, _paint);
		}
		catch (IOException e) {
			Log.e(TAG, "drawImage:" + url, e);
		}
	}

	public void drawRect(int x, int y, int w, int h) {
		_paint.setStyle(Style.STROKE);
		_canvas.drawRect(x, y, x + w, y + h, _paint);
	}

	public void fillRect(int x, int y, int w, int h) {
		_paint.setStyle(Style.FILL_AND_STROKE);
		_canvas.drawRect(x, y, x + w, y + h, _paint);
	}

	public void fillText(String text, int x, int y) {
		_canvas.drawText(text, x, y, _paint);
	}

	public float measureText(String text) {
		return _paint.measureText(text);
	}

	public void scale(float x, float y) {
		_paint.setTextScaleX(x);
	}

	public void strokeText(String text, int x, int y) {
		_canvas.drawText(text, x, y, _paint);
	}

	public void setAlignment(int align) {
		switch (align) {
			case 0: {
				_paint.setTextAlign(Align.LEFT);
				break;
			}
			case 1: {
				_paint.setTextAlign(Align.CENTER);
				break;
			}
			case 2: {
				_paint.setTextAlign(Align.RIGHT);
				break;
			}
		}
	}

	public void setFillColor(int val) {
		_paint.setColor(val | 0xFF000000);
	}

	public void setFontColor(int val) {
		_paint.setColor(val | 0xFF000000);
	}

	public void setFontSize(float val) {
		_paint.setTextSize(val);
	}

	public void setLineWidth(float val) {
		_paint.setStrokeWidth(val);
	}

	public void setFontName(String val) {
		Typeface font = Typeface.create(val, Typeface.NORMAL);
		if (font != null) {
			_paint.setTypeface(font);
		}
		else {
			Log.w(TAG, String.format("setFontName(%s) not found.", val));
		}
	}

	public void setFillStyle(String val) {
		try {
			int color = Color.parseColor(val);
			_paint.setColor(color);
		}
		catch (Exception e) {
			Log.w(TAG, "setFillStyle:" + val, e);
		}
	}
}
