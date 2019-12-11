package app.engine;

import com.google.vrtoolkit.cardboard.CardboardDeviceParams;
import com.google.vrtoolkit.cardboard.Distortion;
import com.google.vrtoolkit.cardboard.Eye;
import com.google.vrtoolkit.cardboard.FieldOfView;
import com.google.vrtoolkit.cardboard.HeadMountedDisplayManager;
import com.google.vrtoolkit.cardboard.HeadTransform;
import com.google.vrtoolkit.cardboard.ScreenParams;
import com.google.vrtoolkit.cardboard.sensors.HeadTracker;

import android.opengl.Matrix;

public class AEVRDisplayImpl {

	private final float[]	mWebVRParams	= new float[12];
	private final float[]	mLEyeTranslate	= new float[16];
	private final float[]	mREyeTranslate	= new float[16];

	private final Eye			mLEye;
	private final Eye			mREye;
	private final HeadTransform	mHead;

	private final HeadTracker				mTracker;
	private final HeadMountedDisplayManager	mManager;

	public AEVRDisplayImpl(long thiz, AEView view) {
		mHead = new HeadTransform();
		mLEye = new Eye(Eye.Type.LEFT);
		mREye = new Eye(Eye.Type.RIGHT);
		mManager = new HeadMountedDisplayManager(view.getContext());
		mTracker = HeadTracker.createFromContext(view.getContext());
	}

	public void stopTracking() {
		mTracker.stopTracking();
	}

	public void startTracking() {
		mTracker.startTracking();
	}

	public float[] toWebVRArray() {
		mTracker.getLastHeadView(mHead.getHeadView(), 0);
		updateFieldOfView(mLEye.getFov(), mREye.getFov());

		float halfInterpupillaryDistance = mManager.getHeadMountedDisplay().getCardboardDeviceParams().getInterLensDistance() * 0.5f;
		Matrix.setIdentityM(this.mLEyeTranslate, 0);
		Matrix.setIdentityM(this.mREyeTranslate, 0);
		Matrix.translateM(this.mLEyeTranslate, 0, halfInterpupillaryDistance, 0.0f, 0.0f);
		Matrix.translateM(this.mREyeTranslate, 0, -halfInterpupillaryDistance, 0.0f, 0.0f);
		Matrix.multiplyMM(mLEye.getEyeView(), 0, this.mLEyeTranslate, 0, mHead.getHeadView(), 0);
		Matrix.multiplyMM(mREye.getEyeView(), 0, this.mREyeTranslate, 0, mHead.getHeadView(), 0);

		mHead.getQuaternion(mWebVRParams, 0);
		mWebVRParams[4] = mLEye.getFov().getTop();
		mWebVRParams[5] = mLEye.getFov().getBottom();
		mWebVRParams[6] = mLEye.getFov().getLeft();
		mWebVRParams[7] = mLEye.getFov().getRight();
		mWebVRParams[8] = mREye.getFov().getTop();
		mWebVRParams[9] = mREye.getFov().getBottom();
		mWebVRParams[10] = mREye.getFov().getLeft();
		mWebVRParams[11] = mREye.getFov().getRight();

		return mWebVRParams;
	}

	private float updateFieldOfView(FieldOfView leftEyeFov, FieldOfView rightEyeFov) {
		ScreenParams screen = mManager.getHeadMountedDisplay().getScreenParams();
		CardboardDeviceParams cdp = mManager.getHeadMountedDisplay().getCardboardDeviceParams();
		Distortion distortion = cdp.getDistortion();

		float eyeToScreenDist = 0.011F + cdp.getScreenToLensDistance();
		float outerDist = (screen.getWidthMeters() - cdp.getInterLensDistance()) / 2.0F;
		float innerDist = cdp.getInterLensDistance() / 2.0F;
		float bottomDist = cdp.getVerticalDistanceToLensCenter() - screen.getBorderSizeMeters();
		float topDist = screen.getHeightMeters() + screen.getBorderSizeMeters() - cdp.getVerticalDistanceToLensCenter();
		float outerAngle = (float) Math.toDegrees(Math.atan2(distortion.distort(outerDist), eyeToScreenDist));
		float innerAngle = (float) Math.toDegrees(Math.atan2(distortion.distort(innerDist), eyeToScreenDist));
		float bottomAngle = (float) Math.toDegrees(Math.atan2(distortion.distort(bottomDist), eyeToScreenDist));
		float topAngle = (float) Math.toDegrees(Math.atan2(distortion.distort(topDist), eyeToScreenDist));

		leftEyeFov.setLeft(Math.min(outerAngle, leftEyeFov.getLeft()));
		leftEyeFov.setRight(Math.min(innerAngle, leftEyeFov.getRight()));
		leftEyeFov.setBottom(Math.min(bottomAngle, leftEyeFov.getBottom()));
		leftEyeFov.setTop(Math.min(topAngle, leftEyeFov.getTop()));

		rightEyeFov.setLeft(Math.min(innerAngle, rightEyeFov.getLeft()));
		rightEyeFov.setRight(Math.min(outerAngle, rightEyeFov.getRight()));
		rightEyeFov.setBottom(Math.min(bottomAngle, rightEyeFov.getBottom()));
		rightEyeFov.setTop(Math.min(topAngle, rightEyeFov.getTop()));

		return eyeToScreenDist;
	}
}
