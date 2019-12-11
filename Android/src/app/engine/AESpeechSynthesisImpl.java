package app.engine;

import java.util.Locale;

import android.content.Context;
import android.speech.tts.TextToSpeech;
import android.speech.tts.TextToSpeech.OnInitListener;
import android.speech.tts.UtteranceProgressListener;
import android.util.Log;

public class AESpeechSynthesisImpl extends UtteranceProgressListener implements OnInitListener {

	private static final String TAG = AESpeechSynthesisImpl.class.getSimpleName();

	private Status			mStatus;
	private Context			mContext;
	private TextToSpeech	mSpeaker;

	public AESpeechSynthesisImpl(long thiz, AEView view) {
		mStatus = Status.NONE;
		mContext = view.getContext().getApplicationContext();
		mSpeaker = new TextToSpeech(mContext, this);
		mSpeaker.setOnUtteranceProgressListener(this);
	}

	@Override
	public void onInit(int status) {
		boolean success = false;
		if (status == TextToSpeech.SUCCESS) {
			int ret = mSpeaker.setLanguage(Locale.UK);
			success = (ret != TextToSpeech.LANG_MISSING_DATA && ret != TextToSpeech.LANG_NOT_SUPPORTED);
		}
		if (!success) {
			Log.e(TAG, "onFinish:Language is not available");
		}
		mStatus = Status.INITED;
	}

	@Override
	public void onStart(String utteranceId) {
		mStatus = Status.STARTED;
	}

	@Override
	public void onDone(String utteranceId) {
		mStatus = Status.COMPLETE;
	}

	@Override
	public void onError(String utteranceId) {
		mStatus = Status.FAILURE;
	}

	public void pause() {
		Log.e(TAG, "pause: not support.");
	}

	public void speak(float rate, String lang, String text) {
		mSpeaker.setSpeechRate(rate);
		mSpeaker.speak(text, TextToSpeech.QUEUE_ADD, null);
	}

	public void cancel() {
		mSpeaker.stop();
	}

	public void resume() {
		Log.e(TAG, "resume: not support.");
	}

	public void release() {
		mSpeaker.stop();
		mSpeaker.shutdown();
		mSpeaker.setOnUtteranceProgressListener(null);
	}

	public void getVoices() {
		Log.e(TAG, "getVoices: not support.");
	}

	public boolean paused() {
		return false;
	}

	public boolean pending() {
		return mStatus == Status.NONE;
	}

	public boolean speaking() {
		return mSpeaker.isSpeaking();
	}

	public static enum Status {
		NONE, INITED, PENDING, STARTED, COMPLETE, FAILURE;
	}
}
