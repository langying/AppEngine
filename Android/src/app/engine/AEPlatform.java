package app.engine;

import java.net.URLDecoder;
import java.net.URLEncoder;

import android.util.Log;

public class AEPlatform {

	private static final String	kTAG	= AEPlatform.class.getSimpleName();
	private static final String	kUTF	= "UTF-8";

	public static String encodeURL(String url) {
		try {
			return URLEncoder.encode(url, kUTF);
		}
		catch (Exception e) {
			Log.i(kTAG, String.format(":encodeURL(%s) exception:%s", url, e.getMessage()));
			return url;
		}
	}

	public static String decodeURL(String url) {
		try {
			return URLDecoder.decode(url, kUTF);
		}
		catch (Exception e) {
			Log.i(kTAG, String.format(":decodeURL(%s) exception:%s", url, e.getMessage()));
			return url;
		}
	}
}
