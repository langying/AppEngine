package app.engine;

import java.net.URI;
import java.util.HashMap;
import java.util.Map;

import org.java_websocket.client.WebSocketClient;
import org.java_websocket.drafts.Draft;
import org.java_websocket.drafts.Draft_10;
import org.java_websocket.drafts.Draft_17;
import org.java_websocket.drafts.Draft_75;
import org.java_websocket.drafts.Draft_76;
import org.java_websocket.handshake.ServerHandshake;

import android.os.Handler;
import android.os.HandlerThread;
import android.os.Process;
import android.util.Log;

public class AEWebSocketImpl {

	private static final String TAG = AEWebSocketImpl.class.getSimpleName();

	private static final Map<String, Draft> PROTOCOLS = new HashMap<String, Draft>();

	static {
		PROTOCOLS.put("10", new Draft_10());
		PROTOCOLS.put("17", new Draft_17());
		PROTOCOLS.put("75", new Draft_75());
		PROTOCOLS.put("76", new Draft_76());
	}

	private long			mThiz;
	private AEView			mView;
	private Handler			mHandler;
	private HandlerThread	mHThread;
	private WebSocketClient	mWebSocket;

	private native void onopen(long thiz, int status, String message);

	private native void onclose(long thiz, int code, String reason, boolean remote);

	private native void onerror(long thiz, String message);

	private native void onmessage(long thiz, String message);

	public AEWebSocketImpl(long thiz, AEView view, String url, String protocol) {
		mThiz = thiz;
		mView = view;

		mHThread = new HandlerThread(getClass().getSimpleName(), Process.THREAD_PRIORITY_FOREGROUND);
		mHThread.start();
		mHandler = new Handler(mHThread.getLooper());
	}

	public void init(String url, String protocol) {
		try {
			Draft draft = PROTOCOLS.get(protocol);
			if (draft == null) {
				draft = PROTOCOLS.get("10");
			}
			mWebSocket = new WebSocketClient(new URI(url), draft) {
				@Override
				public void onOpen(ServerHandshake data) {
					final int status = data.getHttpStatus();
					final String msg = data.getHttpStatusMessage();
					mView.queueEvent(new Runnable() {
						@Override
						public void run() {
							onopen(mThiz, status, msg);
						}
					});
				}

				@Override
				public void onClose(final int code, final String reason, final boolean remote) {
					mView.queueEvent(new Runnable() {
						@Override
						public void run() {
							onclose(mThiz, code, reason, remote);
						}
					});
				}

				@Override
				public void onError(final Exception e) {
					mView.queueEvent(new Runnable() {
						@Override
						public void run() {
							onerror(mThiz, e.getMessage());
						}
					});
				}

				@Override
				public void onMessage(final String message) {
					mView.queueEvent(new Runnable() {
						@Override
						public void run() {
							onmessage(mThiz, message);
						}
					});
				}
			};
		}
		catch (Exception e) {
			Log.e(TAG, "init with uri:" + url, e);
		}
	}

	public void send(byte[] data) {
		mWebSocket.send(data);
	}

	public void send(String text) {
		mWebSocket.send(text);
	}

	public void close(int code, String reason) {
		mWebSocket.close();
	}
}
