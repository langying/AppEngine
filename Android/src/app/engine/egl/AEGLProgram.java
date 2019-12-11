package app.engine.egl;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;

import android.opengl.GLES11Ext;
import android.opengl.GLES20;
import android.opengl.Matrix;
import android.util.Log;

public class AEGLProgram {

	private static final int	FLOAT	= 4;
	private static final String	TAG		= AEGLProgram.class.getSimpleName();

	private static final String VERTEX_SHADER = "" +				//
			"attribute vec4 aPosition;\n" +							//
			"attribute vec4 aTexCoord;\n" +							//
			"varying   vec2 vTexCoord;\n" +							//
			"uniform   mat4 uMVPMatrix;\n" +						//
			"uniform   mat4 uTexMatrix;\n" +						//
			"void main() {\n" +										//
			"	gl_Position = uMVPMatrix * aPosition;\n" +			//
			"	vTexCoord = (uTexMatrix * aTexCoord).xy;\n" +		//
			"}\n";

	private static final String FRAGMENT_SHADER_EXT = "" +			//
			"#extension GL_OES_EGL_image_external : require\n" +	//
			"precision mediump float;\n" +							//
			"varying vec2 vTexCoord;\n" +							//
			"uniform samplerExternalOES sTexture;\n" +				//
			"void main() {\n" +										//
			"	gl_FragColor = texture2D(sTexture, vTexCoord);\n" + //
			"}\n";

	private static final float[] mPosIndexs = {
			// X, Y
			-1.0f, -1.0f,	// LB
			+1.0f, -1.0f,	// RB
			-1.0f, +1.0f,	// LT
			+1.0f, +1.0f	// RT
	};

	private static final float[] mTexIndexs = {
			// X, Y
			0.0f, 0.0f,	// LB
			1.0f, 0.0f,	// RB
			0.0f, 1.0f,	// LT
			1.0f, 1.0f	// RT
	};

	private int	mTexture;
	private int	mProgram;
	private int	aPosition;
	private int	aTexCoord;
	private int	uMVPMatrix;
	private int	uTexMatrix;

	private final float[]		IDENTITY	= new float[16];
	private final FloatBuffer	mPosCorrd	= ByteBuffer.allocateDirect(mPosIndexs.length * FLOAT).order(ByteOrder.nativeOrder()).asFloatBuffer();
	private final FloatBuffer	mTexCorrd	= ByteBuffer.allocateDirect(mTexIndexs.length * FLOAT).order(ByteOrder.nativeOrder()).asFloatBuffer();

	public AEGLProgram() {
		Matrix.setIdentityM(IDENTITY, 0);
		mPosCorrd.put(mPosIndexs).position(0);
		mTexCorrd.put(mTexIndexs).position(0);

		mProgram = createProgram(VERTEX_SHADER, FRAGMENT_SHADER_EXT);
		aPosition = GLES20.glGetAttribLocation(mProgram, "aPosition");
		aTexCoord = GLES20.glGetAttribLocation(mProgram, "aTexCoord");
		uMVPMatrix = GLES20.glGetUniformLocation(mProgram, "uMVPMatrix");
		uTexMatrix = GLES20.glGetUniformLocation(mProgram, "uTexMatrix");
		checkGlError("createProgram");

		int[] textures = new int[1];
		GLES20.glGenTextures(1, textures, 0);
		checkGlError("glGenTextures");

		mTexture = textures[0];
		GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, mTexture);
		GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameterf(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
		GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
		GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
		checkGlError("glTexParameter");
	}

	public void release() {
		Log.d(TAG, "deleting program " + mProgram);
		GLES20.glDeleteProgram(mProgram);
		mProgram = -1;
	}

	public void draw(float[] texMatrix) {
		GLES20.glUseProgram(mProgram);
		checkGlError("glUseProgram");

		GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
		GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, mTexture);

		GLES20.glUniformMatrix4fv(uMVPMatrix, 1, false, IDENTITY, 0);
		checkGlError("glUniformMatrix4fv: uMVPMatrix");

		GLES20.glUniformMatrix4fv(uTexMatrix, 1, false, texMatrix, 0);
		checkGlError("glUniformMatrix4fv: uTexMatrix");

		GLES20.glEnableVertexAttribArray(aPosition);
		GLES20.glVertexAttribPointer(aPosition, 2, GLES20.GL_FLOAT, false, 2 * FLOAT, mPosCorrd);
		checkGlError("glVertexAttribPointer aPosition");

		GLES20.glEnableVertexAttribArray(aTexCoord);
		GLES20.glVertexAttribPointer(aTexCoord, 2, GLES20.GL_FLOAT, false, 2 * FLOAT, mTexCorrd);
		checkGlError("glVertexAttribPointer aTexCoord");

		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_STRIP, 0, 4); // 102314
		checkGlError("glDrawArrays");

		GLES20.glDisableVertexAttribArray(aPosition);
		GLES20.glDisableVertexAttribArray(aTexCoord);
		GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, 0);
		GLES20.glUseProgram(0);
	}

	public int texture() {
		return mTexture;
	}

	private int createProgram(String vertexSource, String fragmentSource) {
		int vertexShader = loadShader(GLES20.GL_VERTEX_SHADER, vertexSource);
		if (vertexShader == 0) {
			return 0;
		}
		int fragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fragmentSource);
		if (fragmentShader == 0) {
			return 0;
		}

		int program = GLES20.glCreateProgram();
		if (program != 0) {
			GLES20.glAttachShader(program, vertexShader);
			checkGlError("glAttachShader: vertexShader");
			GLES20.glAttachShader(program, fragmentShader);
			checkGlError("glAttachShader: fragmentShader");
			GLES20.glLinkProgram(program);
			int[] linkStatus = new int[1];
			GLES20.glGetProgramiv(program, GLES20.GL_LINK_STATUS, linkStatus, 0);
			if (linkStatus[0] != GLES20.GL_TRUE) {
				Log.e(TAG, "Could not link program: ");
				Log.e(TAG, GLES20.glGetProgramInfoLog(program));
				GLES20.glDeleteProgram(program);
				program = 0;
			}
		}
		return program;
	}

	private int loadShader(int shaderType, String source) {
		int shader = GLES20.glCreateShader(shaderType);
		if (shader != 0) {
			GLES20.glShaderSource(shader, source);
			GLES20.glCompileShader(shader);
			int[] compiled = new int[1];
			GLES20.glGetShaderiv(shader, GLES20.GL_COMPILE_STATUS, compiled, 0);
			if (compiled[0] == GLES20.GL_FALSE) {
				Log.e(TAG, "Could not compile shader " + shaderType + ":");
				Log.e(TAG, GLES20.glGetShaderInfoLog(shader));
				GLES20.glDeleteShader(shader);
				shader = 0;
			}
		}
		return shader;
	}

	private void checkGlError(String op) {
		for (int error = 0; (error = GLES20.glGetError()) != GLES20.GL_NO_ERROR;) {
			Log.e(TAG, op + ": glError " + error);
		}
	}
}
