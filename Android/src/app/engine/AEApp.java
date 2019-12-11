package app.engine;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.Closeable;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.security.MessageDigest;

import android.content.Context;
import android.util.Base64;
import android.util.Log;

public class AEApp {

	private static final String	TAG		= AEApp.class.getSimpleName();
	private static final char	CODE[]	= { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	public static String MD5(String text) {
		if (text == null) {
			text = "";
		}
		String code = "" + text.hashCode();
		try {
			MessageDigest MD5 = MessageDigest.getInstance("MD5");
			MD5.update(text.getBytes());
			byte[] md = MD5.digest();
			char[] rt = new char[md.length * 2];
			for (int i = 0, k = 0, length = md.length; i < length; i++) {
				byte bt = md[i];
				rt[k++] = CODE[bt >>> 4 & 0xF];
				rt[k++] = CODE[bt & 0xF];
			}
			code = new String(rt);
		}
		catch (Exception e) {
			Log.e(TAG, "MD5 Not Found:" + text, e);
		}
		return code;
	}

	public static void close(Object... handles) {
		for (Object handle : handles) {
			try {
				if (handle == null) {
				}
				else if (handle instanceof Closeable) {
					((Closeable) handle).close();
				}
				else {
					Log.i(TAG, "close unknown type:" + handle);
				}
			}
			catch (Exception e) {
			}
		}
	}

	public static String absoluteURL(Context ctx, String url) {
		if (url.startsWith("/")) {
			return "file://" + url;
		}
		else if (url.startsWith("file:")) {
			if (url.startsWith("file:///asset/")) {
				url = url.replace("file:///asset/", "file:///android_asset/");
			}
			else if (url.startsWith("file:///cache/")) {
				url = url.replace("file:///cache/", "/");
				url = "file://" + ctx.getCacheDir().getAbsolutePath() + url;
			}
			return url;
		}
		else {
			return url;
		}
	}

	public static InputStream inStreamWithURL(Context ctx, String url) throws IOException {
		url = absoluteURL(ctx, url);
		if (url.startsWith("file:")) {
			int idx = url.indexOf('?');
			if (idx > 0) {
				// 本地URL要去除?以及后面的参数，保证PathFile的正确读取
				url = url.substring(0, idx);
			}
			if (url.startsWith("file:///android_asset/")) {
				url = url.replace("file:///android_asset/", "");
				return ctx.getAssets().open(url);
			}
			else {
				url = url.replace("file://", "");
				return new FileInputStream(url);
			}
		}
		else if (url.startsWith("data:")) {
			String[] comps = url.split(":");
			String[] datas = comps[comps.length - 1].split(";");
			String[] codes = datas[datas.length - 1].split(",");
			byte[] bytes = Base64.decode(codes[codes.length - 1], Base64.DEFAULT);
			return new ByteArrayInputStream(bytes);
		}
		else if (url.startsWith("http:") || url.startsWith("https:")) {
			return new URL(url).openStream();
		}
		else {
			return new URL(url).openStream();
		}
	}

	public static float clamp(float value, float min, float max) {
		return Math.max(min, (Math.min(value, max)));
	}

	public static long totalMemory() {
		long size = -1;
		FileReader reader = null;
		BufferedReader buffer = null;
		try {
			reader = new FileReader("/proc/meminfo");	// 读取meminfo第一行，系统总内存大小
			buffer = new BufferedReader(reader);
			String line = buffer.readLine();
			String[] data = line.split("\\s+");
			size = Long.parseLong(data[1]) * 1024;		// 获得系统总内存，单位是KB，乘以1024转换为Byte
		}
		catch (IOException e) {
			Log.w(TAG, "totalMemory: cant read pathfile /proc/meminfo", e);
		}
		finally {
			close(buffer, reader);
		}
		return size;
	}
}
