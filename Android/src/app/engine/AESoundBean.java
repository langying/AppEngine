package app.engine;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import android.content.Context;
import android.media.AudioManager;
import android.media.SoundPool;
import android.media.SoundPool.OnLoadCompleteListener;
import android.os.Build;
import android.os.Vibrator;
import android.util.Log;

public class AESoundBean {

	private static final String		TAG		= AESoundBean.class.getSimpleName();
	private static final Integer	INVALID	= -1;

	private final ConcurrentHashMap<String, Sound> mSounds = new ConcurrentHashMap<String, Sound>();

	private float		mVolume	= 1f;
	private Context		mContext;
	private Vibrator	mVibrator;
	private SoundPool	mSoundPool;

	public AESoundBean(long thiz, AEView view) {
		mContext = view.getContext().getApplicationContext();
		mSoundPool = new SoundPool(Build.MODEL.contains("GT-I9100") ? 3 : 5, AudioManager.STREAM_MUSIC, 5);
		mSoundPool.setOnLoadCompleteListener(new OnLoadCompleteListener() {
			@Override
			public void onLoadComplete(SoundPool pool, int soundId, int status) {
				if (status != 0) {
					return;
				}
				Sound sound = soundForId(soundId);
				if (sound == null) {
					return;
				}
				if (sound.autoPlay) {
					doPlaySound(sound);
				}
				synchronized (sound) {
					sound.notifyAll();
				}
			}
		});
	}

	public void release() {
		mSoundPool.release();
		mContext = null;
		mVibrator = null;
		mSoundPool = null;
	}

	public void onBackground() {
		mSoundPool.autoPause();
	}

	public void onForeground() {
		mSoundPool.autoResume();
	}

	public void vibrate(float duration) {
		mVibrator.vibrate((long) (duration * 1000));
	}

	public void stopALL() {
		for (Map.Entry<String, Sound> entry : mSounds.entrySet()) {
			List<Integer> streamIds = entry.getValue().streamIds;
			for (Integer streamId : streamIds) {
				mSoundPool.stop(streamId);
			}
			streamIds.clear();
		}
	}

	public void pauseALL() {
		for (Map.Entry<String, Sound> entry : mSounds.entrySet()) {
			List<Integer> streamIds = entry.getValue().streamIds;
			for (Integer streamId : streamIds) {
				mSoundPool.pause(streamId);
			}
		}
	}

	public void resumeALL() {
		for (Map.Entry<String, Sound> entry : mSounds.entrySet()) {
			List<Integer> streamIds = entry.getValue().streamIds;
			for (Integer streamId : streamIds) {
				mSoundPool.resume(streamId);
			}
		}
	}

	public void setMuted(boolean muted) {
		float volume = muted ? 0 : mVolume;
		for (Map.Entry<String, Sound> entry : mSounds.entrySet()) {
			List<Integer> streamIds = entry.getValue().streamIds;
			for (Integer streamId : streamIds) {
				mSoundPool.setVolume(streamId, volume, volume);
			}
		}
	}

	public void setVolume(float volume) {
		mVolume = AEApp.clamp(volume, 0, 1);
		for (Map.Entry<String, Sound> entry : mSounds.entrySet()) {
			List<Integer> streamIds = entry.getValue().streamIds;
			for (Integer streamId : streamIds) {
				mSoundPool.setVolume(streamId, mVolume, mVolume);
			}
		}
	}

	public int play(String url) {
		Sound sound = mSounds.get(url);
		if (sound != null) {
			return doPlaySound(sound);
		}

		Integer soundId = preload(url);
		if (soundId == INVALID) {
			return INVALID;
		}

		sound = mSounds.get(url);
		mSounds.putIfAbsent(url, sound);
		synchronized (sound) {
			try {
				sound.wait(500);
			}
			catch (Exception e) {
				Log.i(TAG, "Audio.playSound[wait timeout]", e);
			}
		}
		return sound.id;

	}

	public void stop(int streamId) {
		mSoundPool.stop(streamId);
		for (Map.Entry<String, Sound> entry : mSounds.entrySet()) {
			List<Integer> streamIds = entry.getValue().streamIds;
			int index = streamIds.indexOf(streamId);
			if (index >= 0) {
				streamIds.remove(index);
			}
		}
	}

	public void pause(int streamId) {
		mSoundPool.pause(streamId);
	}

	public void resume(int streamId) {
		mSoundPool.resume(streamId);
	}

	public void unload(String url) {
		if (!mSounds.containsKey(url)) {
			return;
		}
		Sound sound = mSounds.get(url);
		for (Integer streamId : sound.streamIds) {
			mSoundPool.stop(streamId);
		}
		mSoundPool.unload(sound.id);
		mSounds.remove(url);
	}

	public int preload(String url) {
		try {
			Sound sound = mSounds.get(url);
			if (sound != null) {
				return sound.id;
			}
			sound = new Sound();
			if (url.startsWith("file:///")) {
				url = url.replace("file:///asset/", "");
				url = url.replace("file://", "");
			}
			if (url.startsWith("/")) {
				sound.id = mSoundPool.load(url, 0);
			}
			else {
				sound.id = mSoundPool.load(mContext.getAssets().openFd(url), 0);
			}
			mSounds.put(url, sound);
			return sound.id;
		}
		catch (Exception e) {
			Log.e(TAG, "preload url:" + url, e);
		}
		return INVALID;
	}

	private Sound soundForId(int soundId) {
		for (Map.Entry<String, Sound> entry : mSounds.entrySet()) {
			Sound sound = entry.getValue();
			if (sound.id == soundId) {
				return sound;
			}
		}
		return null;
	}

	private int doPlaySound(Sound sound) {
		int streamId = mSoundPool.play(sound.id, mVolume, mVolume, 1, (sound.loop ? -1 : 0), 1f);
		sound.streamIds.add(streamId);
		return streamId;
	}

	private static class Sound {
		public int				id			= INVALID;
		public boolean			loop		= false;
		public boolean			autoPlay	= false;
		public List<Integer>	streamIds	= new ArrayList<Integer>();
	}
}
