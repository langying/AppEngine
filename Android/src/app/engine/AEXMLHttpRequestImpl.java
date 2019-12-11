package app.engine;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.net.URLConnection;
import java.util.HashMap;
import java.util.Map;

import android.annotation.SuppressLint;
import android.os.AsyncTask;
import android.text.TextUtils;
import android.util.Log;

public class AEXMLHttpRequestImpl {

    private static final String TAG = AEXMLHttpRequestImpl.class.getSimpleName();
    private static final String MIME = "text/plain";

    private boolean mAsync;
    private String mURL;
    private String mMethod;
    private String mMimeType;
    private String mOverrideMimeType;
    private String mOverrideResponseType;

    private final long mThiz;
    private final AEView mView;
    // private final HttpClient mClient;
    private final Map<String, String> mReqHeaders = new HashMap<String, String>();
    private final Map<String, String> mRetHeaders = new HashMap<String, String>();

    private native void onFinish(long thiz, String mime, String type, byte[] data);

    public AEXMLHttpRequestImpl(long thiz, AEView view) {
        mThiz = thiz;
        mView = view;

//        HttpParams params = new BasicHttpParams();
//        params.setIntParameter(CoreConnectionPNames.SO_TIMEOUT, 3000);
//        params.setIntParameter(CoreConnectionPNames.CONNECTION_TIMEOUT, 3000);
//        mClient = new DefaultHttpClient(params);
    }

    public void open(String method, String url, boolean async, String username, String password) {
        mURL = url;
        mAsync = async;
        mMethod = method;
        mMimeType = URLConnection.getFileNameMap().getContentTypeFor(url);
        if (TextUtils.isEmpty(mMimeType)) {
            mMimeType = MIME;
        }
    }

    public void send() {
        if (!mAsync) {
            if (mURL.startsWith("file:") || mURL.startsWith("data:")) {
                onSyncSuccess(bytesWithAsset(mURL));
            } else {
                onSyncSuccess(bytesWithURL(mURL));
            }
            return;
        }
        new AsyncTask<String, Integer, byte[]>() {
            @Override
            protected byte[] doInBackground(String... params) {
                if (mURL.startsWith("file:") || mURL.startsWith("data:")) {
                    return bytesWithAsset(mURL);
                } else {
                    return bytesWithURL(mURL);
                }
            }

            @Override
            protected void onPostExecute(byte[] data) {
                onSuccess(data);
            }
        }.execute();
    }

    public void abort() {
    }

    public void reset() {
        mURL = "";
        mMethod = "GET";
        mMimeType = MIME;
        mOverrideMimeType = "";
        mOverrideResponseType = "";
        mAsync = true;
    }

    public void overrideMimeType(String mime) {
        mOverrideMimeType = mime;
    }

    public void overrideResponseType(String type) {
        mOverrideResponseType = type;
    }

    public void setRequestHeader(String key, String val) {
        mReqHeaders.put(key, val);
    }

    public String getAllResponseHeaders() {
        return "";
    }

    public String getResponseHeader(String key) {
        return mRetHeaders.get(key);
    }

    @SuppressLint("DefaultLocale")
    private String mimetype() {
        String mime = mOverrideMimeType;
        if (TextUtils.isEmpty(mime)) {
            mime = mMimeType;
        }
        if (TextUtils.isEmpty(mime)) {
            mime = "text/plain";
        }
        return mime.toLowerCase();
    }

    private String responseType() {
        if (!TextUtils.isEmpty(mOverrideResponseType)) {
            return mOverrideResponseType;
        }
        String mime = mimetype();
        if (mime.contains("text") || mime.contains("json") || mime.contains("javascript")) {
            return "text";
        } else {
            return "blob";
        }
    }

    private byte[] bytesWithURL(String url) {
        byte[] buff = { 0 };
//        InputStream inStream = null;
//        ByteArrayOutputStream otStream = null;
//        try {
//            HttpUriRequest http;
//            if ("GET".equalsIgnoreCase(mMethod)) {
//                http = new HttpGet(url);
//            } else {
//                http = new HttpPost(url);
//            }
//            for (Map.Entry<String, String> entry : mReqHeaders.entrySet()) {
//                http.setHeader(entry.getKey(), entry.getValue());
//            }
//
//            HttpResponse response = mClient.execute(http);
//            inStream = response.getEntity().getContent();
//            otStream = new ByteArrayOutputStream();
//            byte[] data = new byte[1024];
//            for (int size = -1; (size = inStream.read(data)) != -1;) {
//                otStream.write(data, 0, size);
//            }
//            buff = otStream.toByteArray();
//        } catch (Exception e) {
//            Log.e(TAG, "response " + mMethod + ":" + url, e);
//        } finally {
//            AEApp.close(inStream, otStream);
//        }
        return buff;
    }

    private byte[] bytesWithAsset(String url) {
        byte[] buff = { 0 };
        InputStream inStream = null;
        ByteArrayOutputStream otStream = null;
        try {
            inStream = AEApp.inStreamWithURL(mView.getContext(), url);
            otStream = new ByteArrayOutputStream();
            byte[] buffer = new byte[1024];
            for (int len = -1; (len = inStream.read(buffer)) != -1;) {
                otStream.write(buffer, 0, len);
            }
            buff = otStream.toByteArray();
        } catch (Exception e) {
            Log.e(TAG, "bytesWithAsset:" + url, e);
        } finally {
            AEApp.close(inStream, otStream);
        }
        return buff;
    }

    private void onSuccess(final byte[] data) {
        mView.queueEvent(new Runnable() {
            @Override
            public void run() {
                onSyncSuccess(data);
            }
        });
    }

    private void onSyncSuccess(byte[] data) {
        String mime = mimetype();
        String type = responseType();
        onFinish(mThiz, mime, type, data);
    }
}
