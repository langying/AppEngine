function Euler( x, y, z, order ) {
	x = x || 0;
	y = y || 0;
	z = z || 0;
	this._order = order || Euler.DefaultOrder;
	this.elements = new Float32Array( [ x, y, z] );
}

Euler.RotationOrders = [ 'XYZ', 'YZX', 'ZXY', 'XZY', 'YXZ', 'ZYX' ];

Euler.DefaultOrder = 'XYZ';

Euler.prototype = {

	constructor: Euler,

	isEuler: true,

	get x () {
		return this.elements[0];
	},

	set x ( value ) {
		this.elements[0] = value;
		this.onChangeCallback();
	},

	get y () {
		return this.elements[1];
	},

	set y ( value ) {
		this.elements[1] = value;
		this.onChangeCallback();
	},

	get z () {
		return this.this.elements[2];
	},

	set z ( value ) {
		this.elements[2] = value;
		this.onChangeCallback();
	},

	get order () {
		return this._order;
	},

	set order ( value ) {
		this._order = value;
		this.onChangeCallback();
	},

	set: function ( x, y, z, order ) {
		var e = this.elements;
		e[0] = x;
		e[1] = y;
		e[2] = z;
		this._order = order || this._order;
		this.onChangeCallback();
		return this;
	},

	clone: function () {
		var e = this.elements;
		return new this.constructor( e[0], e[1], e[2], this._order );
	},

	copy: function ( euler ) {
		var te = this.elements;
		var me = euler.elements;
		te[0] = me[0];
		te[1] = me[1];
		te[2] = me[2];
		this._order = euler._order;
		this.onChangeCallback();
		return this;
	},

	setFromRotationMatrix: function ( m, order, update ) {
		eule_setFromRotationMatrix(this.elements, m.elements, order);
		this._order = order;
		if ( update !== false ) this.onChangeCallback();
		return this;
	},

	setFromQuaternion: function (q, order, update) {
		order = order || this._order;
		eule_setFromQuaternion(this.elements, q.elements, order);
		this._order = order;
		if ( update !== false ) this.onChangeCallback();
		return this;
	},

	setFromVector3: function ( v, order ) {
		return this.set( v.x, v.y, v.z, order || this._order );
	},

	reorder: function (newer) {
		eule_reorder(this.elements, this._order, newer);
		this._order = newer;
		return this;
	},

	equals: function ( e ) {
		var me = e.elements;
		var te = this.elements;
		return ( me[0] === te[0] ) && ( me[1] == te[1] ) && ( me[2] == te[2] ) && ( e._order === this._order );
	},

	fromArray: function ( array ) {
		var e = this.elements;
		e[ 0 ] = array[ 0 ];
		e[ 1 ] = array[ 1 ];
		e[ 2 ] = array[ 2 ];
		if ( array[ 3 ] !== undefined ) this._order = array[ 3 ];
		this.onChangeCallback();
		return this;
	},

	toArray: function ( array, offset ) {
		if ( array === undefined ) array = [];
		if ( offset === undefined ) offset = 0;

		var e = this.elements;
		array[ offset ] = e[0];
		array[ offset + 1 ] = e[1];
		array[ offset + 2 ] = e[2];
		array[ offset + 3 ] = this._order;
		return array;
	},

	toVector3: function ( optionalResult ) {
		var e = this.elements;
		if ( optionalResult ) {
			return optionalResult.set( e[0], e[1], e[2] );
		} else {
			return new Vector3( e[0], e[1], e[2] );
		}
	},

	onChange: function ( callback ) {
		this.onChangeCallback = callback;
		return this;
	},

	onChangeCallback: function () {}

};

function Matrix3() {
	this.elements = new Float32Array( [
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	] );

	if ( arguments.length > 0 ) {
		console.error( 'THREE.Matrix3: the constructor no longer reads arguments. use .set() instead.' );
	}
}

Matrix3.prototype = {

	constructor: Matrix3,

	isMatrix3: true,

	set: function ( n11, n12, n13, n21, n22, n23, n31, n32, n33 ) {
		var te = this.elements;
		te[ 0 ] = n11; te[ 1 ] = n21; te[ 2 ] = n31;
		te[ 3 ] = n12; te[ 4 ] = n22; te[ 5 ] = n32;
		te[ 6 ] = n13; te[ 7 ] = n23; te[ 8 ] = n33;
		return this;
	},

	identity: function () {
		this.set(
			1, 0, 0,
			0, 1, 0,
			0, 0, 1
		);
		return this;
	},

	clone: function () {
		return new this.constructor().fromArray( this.elements );
	},

	copy: function ( m ) {
		var me = m.elements;
		this.set(
			me[ 0 ], me[ 3 ], me[ 6 ],
			me[ 1 ], me[ 4 ], me[ 7 ],
			me[ 2 ], me[ 5 ], me[ 8 ]
		);
		return this;
	},

	setFromMatrix4: function( m ) {
		mat3_setFromMatrix4(this.elements, m.elements);
		return this;
	},

	applyToVector3Array: function (array, offset, length) {
		if ( offset === undefined ) offset = 0;
		if ( length === undefined ) length = array.length;

		mat3_applyToVector3Array(this.elements, array, offset, length);
		return array;
	},

	applyToBuffer: function () {

		var v1;

		return function applyToBuffer( buffer, offset, length ) {

			if ( v1 === undefined ) v1 = new Vector3();
			if ( offset === undefined ) offset = 0;
			if ( length === undefined ) length = buffer.length / buffer.itemSize;

			for ( var i = 0, j = offset; i < length; i ++, j ++ ) {

				v1.x = buffer.getX( j );
				v1.y = buffer.getY( j );
				v1.z = buffer.getZ( j );

				v1.applyMatrix3( this );

				buffer.setXYZ( j, v1.x, v1.y, v1.z );

			}

			return buffer;

		};

	}(),

	multiplyScalar: function ( s ) {
		mat3_multiplyScalar(this.elements, s);
		return this;
	},

	determinant: function () {
		return mat3_determinant(this.elements);
	},

	getInverse: function ( matrix, throwOnDegenerate ) {
		if ( (matrix && matrix.isMatrix4) ) {
			console.error( "THREE.Matrix3.getInverse no longer takes a Matrix4 argument." );
		}
		mat3_getInverse(this.elements, matrix.elements);
		return this;
	},

	transpose: function () {
		mat3_transpose(this.elements);
		return this;
	},

	flattenToArrayOffset: function ( array, offset ) {
		console.warn( "THREE.Matrix3: .flattenToArrayOffset is deprecated - just use .toArray instead." );
		return this.toArray( array, offset );
	},

	getNormalMatrix: function ( matrix4 ) {
		mat3_getNormalMatrix(this.elements, matrix4.elements);
		return this;
	},

	transposeIntoArray: function ( r ) {
		mat3_transposeIntoArray(this.elements, r);
		return this;
	},

	fromArray: function ( array, offset ) {
		if ( offset === undefined ) offset = 0;
		var e = this.elements;
		for (var i = 0; i < 9; i ++) {
			e[i] = array[i + offset];
		}
		return this;
	},

	toArray: function ( array, offset ) {
		if ( array === undefined ) array = [];
		if ( offset === undefined ) offset = 0;
		
		var te = this.elements;
		array[ offset ] = te[ 0 ];
		array[ offset + 1 ] = te[ 1 ];
		array[ offset + 2 ] = te[ 2 ];
		array[ offset + 3 ] = te[ 3 ];
		array[ offset + 4 ] = te[ 4 ];
		array[ offset + 5 ] = te[ 5 ];
		array[ offset + 6 ] = te[ 6 ];
		array[ offset + 7 ] = te[ 7 ];
		array[ offset + 8 ] = te[ 8 ];
		return array;
	}

};

function Matrix4() {
	this.elements = new Float32Array( [
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	] );
	if ( arguments.length > 0 ) {
		console.error( 'THREE.Matrix4: the constructor no longer reads arguments. use .set() instead.' );
	}
}

Matrix4.prototype = {

	constructor: Matrix4,

	isMatrix4: true,

	set: function ( n11, n12, n13, n14, n21, n22, n23, n24, n31, n32, n33, n34, n41, n42, n43, n44 ) {
		var te = this.elements;
		te[ 0 ] = n11; te[ 4 ] = n12; te[ 8 ] = n13; te[ 12 ] = n14;
		te[ 1 ] = n21; te[ 5 ] = n22; te[ 9 ] = n23; te[ 13 ] = n24;
		te[ 2 ] = n31; te[ 6 ] = n32; te[ 10 ] = n33; te[ 14 ] = n34;
		te[ 3 ] = n41; te[ 7 ] = n42; te[ 11 ] = n43; te[ 15 ] = n44;
		return this;
	},

	identity: function () {
		this.set(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
		return this;
	},

	clone: function () {
		return new Matrix4().fromArray( this.elements );
	},

	copy: function ( m ) {
		this.elements.set( m.elements );
		return this;
	},

	copyPosition: function ( m ) {
		var te = this.elements;
		var me = m.elements;

		te[ 12 ] = me[ 12 ];
		te[ 13 ] = me[ 13 ];
		te[ 14 ] = me[ 14 ];
		return this;
	},

	extractBasis: function ( xAxis, yAxis, zAxis ) {
		xAxis.setFromMatrixColumn( this, 0 );
		yAxis.setFromMatrixColumn( this, 1 );
		zAxis.setFromMatrixColumn( this, 2 );
		return this;
	},

	makeBasis: function ( xAxis, yAxis, zAxis ) {
		this.set(
			xAxis.x, yAxis.x, zAxis.x, 0,
			xAxis.y, yAxis.y, zAxis.y, 0,
			xAxis.z, yAxis.z, zAxis.z, 0,
			0,       0,       0,       1
		);
		return this;
	},

	extractRotation: function (m) {
		mat4_extractRotation(this.elements, m.elements);
		return this;
	},

	makeRotationFromEuler: function ( euler ) {
		if ( (euler && euler.isEuler) === false ) {
			console.error( 'THREE.Matrix: .makeRotationFromEuler() now expects a Euler rotation rather than a Vector3 and order.' );
		}
		mat4_makeRotationFromEuler(this.elements, euler.elements, euler.order);
		return this;
	},

	makeRotationFromQuaternion: function ( q ) {
		mat4_makeRotationFromQuaternion(this.elements, q.elements);
		return this;
	},

	lookAt: function (eye, target, up) {
		mat4_lookAt(this.elements, eye.elements, target.elements, up.elements);
		return this;
	},

	multiply: function ( m, n ) {
		if ( n !== undefined ) {
			console.warn( 'THREE.Matrix4: .multiply() now only accepts one argument. Use .multiplyMatrices( a, b ) instead.' );
			return this.multiplyMatrices( m, n );
		}
		return this.multiplyMatrices( this, m );
	},

	premultiply: function ( m ) {
		return this.multiplyMatrices( m, this );
	},

	multiplyMatrices: function ( a, b ) {
		mat4_multiplyMatrices(this.elements, a.elements, b.elements);
		return this;
	},

	multiplyToArray: function ( a, b, r ) {
		var te = this.elements;
		this.multiplyMatrices( a, b );
		r[ 0 ] = te[ 0 ]; r[ 1 ] = te[ 1 ]; r[ 2 ] = te[ 2 ]; r[ 3 ] = te[ 3 ];
		r[ 4 ] = te[ 4 ]; r[ 5 ] = te[ 5 ]; r[ 6 ] = te[ 6 ]; r[ 7 ] = te[ 7 ];
		r[ 8 ] = te[ 8 ]; r[ 9 ] = te[ 9 ]; r[10 ] = te[10 ]; r[11 ] = te[11 ];
		r[12 ] = te[12 ]; r[13 ] = te[13 ]; r[14 ] = te[14 ]; r[15 ] = te[15 ];
		return this;
	},

	multiplyScalar: function ( s ) {
		mat4_multiplyScalar(this.elements, s);
		return this;
	},

	applyToVector3Array: function (array, offset, length) {
		if ( offset === undefined ) offset = 0;
		if ( length === undefined ) length = array.length;
		mat4_applyToVector3Array(this.elements, array, offset, length);
		return array;
	},

	applyToBuffer: function () {
		var v1;
		return function applyToBuffer( buffer, offset, length ) {
			if ( v1 === undefined ) v1 = new Vector3();
			if ( offset === undefined ) offset = 0;
			if ( length === undefined ) length = buffer.length / buffer.itemSize;
			
			for ( var i = 0, j = offset; i < length; i ++, j ++ ) {
				v1.x = buffer.getX( j );
				v1.y = buffer.getY( j );
				v1.z = buffer.getZ( j );
				v1.applyMatrix4( this );
				buffer.setXYZ( j, v1.x, v1.y, v1.z );
			}
			return buffer;
		};
	}(),

	determinant: function () {
		return mat4_determinant(this.elements);
	},

	transpose: function () {
		mat4_transpose(this.elements);
		return this;
	},

	flattenToArrayOffset: function ( array, offset ) {
		console.warn( "THREE.Matrix3: .flattenToArrayOffset is deprecated - just use .toArray instead." );
		return this.toArray( array, offset );
	},

	getPosition: function () {
		var v1;
		return function getPosition() {
			if ( v1 === undefined ) v1 = new Vector3();
			console.warn( 'THREE.Matrix4: .getPosition() has been removed. Use Vector3.setFromMatrixPosition( matrix ) instead.' );
			return v1.setFromMatrixColumn( this, 3 );
		};
	}(),

	setPosition: function ( v ) {
		mat4_setPosition(this.elements, v.elements);
		return this;
	},

	getInverse: function ( m, throwOnDegenerate ) {
		mat4_getInverse(this.elements, m.elements);
		return this;
	},

	scale: function ( v ) {
		mat4_scale(this.elements, v.elements);
		return this;
	},

	getMaxScaleOnAxis: function () {
		return mat4_getMaxScaleOnAxis(this.elements);
	},

	makeTranslation: function ( x, y, z ) {
		mat4_makeTranslation(this.elements, x, y, z);
		return this;
	},

	makeRotationX: function ( theta ) {
		mat4_makeRotationX(this.elements, theta);
		return this;
	},

	makeRotationY: function ( theta ) {
		mat4_makeRotationY(this.elements, theta);
		return this;
	},

	makeRotationZ: function ( theta ) {
		mat4_makeRotationZ(this.elements, theta);
		return this;
	},

	makeRotationAxis: function ( axis, angle ) {
		mat4_makeRotationAxis(this.elements, axis.elements, angle);
		return this;
	},

	makeScale: function ( x, y, z ) {
		mat4_makeScale(this.elements, x, y, z);
		return this;
	},

	compose: function ( position, quaternion, scale ) {
		mat4_compose(this.elements, position.elements, quaternion.elements, scale.elements);
		return this;
	},

	decompose: function () {

		var vector, matrix;

		return function decompose( position, quaternion, scale ) {

			if ( vector === undefined ) {

				vector = new Vector3();
				matrix = new Matrix4();

			}

			var te = this.elements;

			var sx = vector.set( te[ 0 ], te[ 1 ], te[ 2 ] ).length();
			var sy = vector.set( te[ 4 ], te[ 5 ], te[ 6 ] ).length();
			var sz = vector.set( te[ 8 ], te[ 9 ], te[ 10 ] ).length();

			// if determine is negative, we need to invert one scale
			var det = this.determinant();
			if ( det < 0 ) {

				sx = - sx;

			}

			position.x = te[ 12 ];
			position.y = te[ 13 ];
			position.z = te[ 14 ];

			// scale the rotation part

			matrix.elements.set( this.elements ); // at this point matrix is incomplete so we can't use .copy()

			var invSX = 1 / sx;
			var invSY = 1 / sy;
			var invSZ = 1 / sz;

			matrix.elements[ 0 ] *= invSX;
			matrix.elements[ 1 ] *= invSX;
			matrix.elements[ 2 ] *= invSX;

			matrix.elements[ 4 ] *= invSY;
			matrix.elements[ 5 ] *= invSY;
			matrix.elements[ 6 ] *= invSY;

			matrix.elements[ 8 ] *= invSZ;
			matrix.elements[ 9 ] *= invSZ;
			matrix.elements[ 10 ] *= invSZ;

			quaternion.setFromRotationMatrix( matrix );

			scale.x = sx;
			scale.y = sy;
			scale.z = sz;

			return this;

		};

	}(),

	makeFrustum: function ( left, right, bottom, top, near, far ) {

		var te = this.elements;
		var x = 2 * near / ( right - left );
		var y = 2 * near / ( top - bottom );

		var a = ( right + left ) / ( right - left );
		var b = ( top + bottom ) / ( top - bottom );
		var c = - ( far + near ) / ( far - near );
		var d = - 2 * far * near / ( far - near );

		te[ 0 ] = x;	te[ 4 ] = 0;	te[ 8 ] = a;	te[ 12 ] = 0;
		te[ 1 ] = 0;	te[ 5 ] = y;	te[ 9 ] = b;	te[ 13 ] = 0;
		te[ 2 ] = 0;	te[ 6 ] = 0;	te[ 10 ] = c;	te[ 14 ] = d;
		te[ 3 ] = 0;	te[ 7 ] = 0;	te[ 11 ] = - 1;	te[ 15 ] = 0;

		return this;

	},

	makePerspective: function ( fov, aspect, near, far ) {

		var ymax = near * Math.tan( _Math.DEG2RAD * fov * 0.5 );
		var ymin = - ymax;
		var xmin = ymin * aspect;
		var xmax = ymax * aspect;

		return this.makeFrustum( xmin, xmax, ymin, ymax, near, far );

	},

	makeOrthographic: function ( left, right, top, bottom, near, far ) {

		var te = this.elements;
		var w = 1.0 / ( right - left );
		var h = 1.0 / ( top - bottom );
		var p = 1.0 / ( far - near );

		var x = ( right + left ) * w;
		var y = ( top + bottom ) * h;
		var z = ( far + near ) * p;

		te[ 0 ] = 2 * w;	te[ 4 ] = 0;	te[ 8 ] = 0;	te[ 12 ] = - x;
		te[ 1 ] = 0;	te[ 5 ] = 2 * h;	te[ 9 ] = 0;	te[ 13 ] = - y;
		te[ 2 ] = 0;	te[ 6 ] = 0;	te[ 10 ] = - 2 * p;	te[ 14 ] = - z;
		te[ 3 ] = 0;	te[ 7 ] = 0;	te[ 11 ] = 0;	te[ 15 ] = 1;

		return this;

	},

	equals: function ( matrix ) {
		return mat4_equals(this.elements, matrix.elements);
	},

	fromArray: function ( array, offset ) {
		if ( offset === undefined ) offset = 0;
		var e = this.elements;
		for (var i = 0; i < 16; i++) {
			e[i] = array[i + offset];
		}
		return this;
	},

	toArray: function ( array, offset ) {
		if ( array === undefined ) array = [];
		if ( offset === undefined ) offset = 0;
		
		var te = this.elements;
		array[ offset ] = te[ 0 ];
		array[ offset + 1 ] = te[ 1 ];
		array[ offset + 2 ] = te[ 2 ];
		array[ offset + 3 ] = te[ 3 ];
		array[ offset + 4 ] = te[ 4 ];
		array[ offset + 5 ] = te[ 5 ];
		array[ offset + 6 ] = te[ 6 ];
		array[ offset + 7 ] = te[ 7 ];
		array[ offset + 8 ] = te[ 8 ];
		array[ offset + 9 ] = te[ 9 ];
		array[ offset + 10] = te[ 10];
		array[ offset + 11] = te[ 11];
		array[ offset + 12] = te[ 12];
		array[ offset + 13] = te[ 13];
		array[ offset + 14] = te[ 14];
		array[ offset + 15] = te[ 15];
		return array;
	}
};

function Quaternion( x, y, z, w ) {
	x = x || 0;
	y = y || 0;
	z = z || 0;
	w = ( w !== undefined ) ? w : 1;
	this.elements = new Float32Array([x, y, z, w]);
}

Quaternion.prototype = {

	constructor: Quaternion,

	get x () {
		return this.elements[0];
	},

	set x ( value ) {
		this.elements[0] = value;
		this.onChangeCallback();
	},

	get y () {
		return this.elements[1];
	},

	set y ( value ) {
		this.elements[1] = value;
		this.onChangeCallback();
	},

	get z () {
		return this.elements[2];
	},

	set z ( value ) {
		this.elements[2] = value;
		this.onChangeCallback();
	},

	get w () {
		return this.elements[3];
	},

	set w ( value ) {
		this.elements[3] = value;
		this.onChangeCallback();
	},

	set: function ( x, y, z, w ) {
		var e = this.elements;
		e[0] = x;
		e[1] = y;
		e[2] = z;
		e[3] = w;
		this.onChangeCallback();
		return this;
	},

	clone: function () {
		var e = this.elements;
		return new this.constructor(e[0], e[1], e[2], e[3]);
	},

	copy: function ( q ) {
		var e = this.elements;
		e[0] = q.x;
		e[1] = q.y;
		e[2] = q.z;
		e[3] = q.w;
		this.onChangeCallback();
		return this;
	},

	setFromEuler: function ( euler, update ) {
		if ( (euler && euler.isEuler) === false ) {
			throw new Error( 'THREE.Quaternion: .setFromEuler() now expects an Euler rotation rather than a Vector3 and order.' );
		}
		quat_setFromEuler(this.elements, euler.elements, euler.order);
		if ( update !== false ) this.onChangeCallback();
		return this;
	},

	setFromAxisAngle: function ( axis, angle ) {
		quat_setFromAxisAngle(this.elements, axis.elements, angle);
		this.onChangeCallback();
		return this;
	},

	setFromRotationMatrix: function ( m ) {
		quat_setFromRotationMatrix(this.elements, m.elements);
		this.onChangeCallback();
		return this;
	},

	setFromUnitVectors: function (vFrom, vTo) {
		quat_setFromUnitVectors(this.elements, vFrom.elements, vTo.elements);
		this.onChangeCallback();
		return this;
	},

	inverse: function () {
		quat_inverse(this.elements);
		this.onChangeCallback();
		return this;
	},

	conjugate: function () {
		quat_conjugate(this.elements);
		this.onChangeCallback();
		return this;
	},

	dot: function ( v ) {
		return quat_dot(this.elements, v.elements);
	},

	lengthSq: function () {
		return quat_lengthSq(this.elements);
	},

	length: function () {
		return quat_length(this.elements);
	},

	normalize: function () {
		quat_normalize(this.elements);
		this.onChangeCallback();
		return this;
	},

	multiply: function ( q, p ) {
		if ( p !== undefined ) {
			console.warn( 'THREE.Quaternion: .multiply() now only accepts one argument. Use .multiplyQuaternions( a, b ) instead.' );
			return this.multiplyQuaternions( q, p );
		}
		return this.multiplyQuaternions( this, q );
	},

	premultiply: function ( q ) {
		return this.multiplyQuaternions( q, this );
	},

	multiplyQuaternions: function ( a, b ) {
		multiplyQuaternions(this.elements, a.elements, b.elements);
		this.onChangeCallback();
		return this;
	},

	slerp: function ( qb, t ) {
		quat_slerp(this.elements, qb.elements, t);
		this.onChangeCallback();
		return this;
	},

	equals: function ( q ) {
		return quat_equals(this.elements, q.elements);
	},

	fromArray: function ( array, offset ) {
		if ( offset === undefined ) offset = 0;
		var e = this.elements;
		e[0] = array[ offset ];
		e[1] = array[ offset + 1 ];
		e[2] = array[ offset + 2 ];
		e[3] = array[ offset + 3 ];
		this.onChangeCallback();
		return this;
	},

	toArray: function ( array, offset ) {
		if ( array === undefined ) array = [];
		if ( offset === undefined ) offset = 0;
		var e = this.elements;
		array[ offset ] = e[0];
		array[ offset + 1 ] = e[1];
		array[ offset + 2 ] = e[2];
		array[ offset + 3 ] = e[3];
		return array;
	},

	onChange: function ( callback ) {
		this.onChangeCallback = callback;
		return this;
	},

	onChangeCallback: function () {}

};

Object.assign( Quaternion, {

	slerp: function( qa, qb, qm, t ) {

		return qm.copy( qa ).slerp( qb, t );

	},

	slerpFlat: function(
			dst, dstOffset, src0, srcOffset0, src1, srcOffset1, t ) {

		// fuzz-free, array-based Quaternion SLERP operation

		var x0 = src0[ srcOffset0 + 0 ],
			y0 = src0[ srcOffset0 + 1 ],
			z0 = src0[ srcOffset0 + 2 ],
			w0 = src0[ srcOffset0 + 3 ],

			x1 = src1[ srcOffset1 + 0 ],
			y1 = src1[ srcOffset1 + 1 ],
			z1 = src1[ srcOffset1 + 2 ],
			w1 = src1[ srcOffset1 + 3 ];

		if ( w0 !== w1 || x0 !== x1 || y0 !== y1 || z0 !== z1 ) {

			var s = 1 - t,

				cos = x0 * x1 + y0 * y1 + z0 * z1 + w0 * w1,

				dir = ( cos >= 0 ? 1 : - 1 ),
				sqrSin = 1 - cos * cos;

			// Skip the Slerp for tiny steps to avoid numeric problems:
			if ( sqrSin > Number.EPSILON ) {

				var sin = Math.sqrt( sqrSin ),
					len = Math.atan2( sin, cos * dir );

				s = Math.sin( s * len ) / sin;
				t = Math.sin( t * len ) / sin;

			}

			var tDir = t * dir;

			x0 = x0 * s + x1 * tDir;
			y0 = y0 * s + y1 * tDir;
			z0 = z0 * s + z1 * tDir;
			w0 = w0 * s + w1 * tDir;

			// Normalize in case we just did a lerp:
			if ( s === 1 - t ) {

				var f = 1 / Math.sqrt( x0 * x0 + y0 * y0 + z0 * z0 + w0 * w0 );

				x0 *= f;
				y0 *= f;
				z0 *= f;
				w0 *= f;

			}

		}

		dst[ dstOffset + 0 ] = x0;
		dst[ dstOffset + 1 ] = y0;
		dst[ dstOffset + 2 ] = z0;
		dst[ dstOffset + 3 ] = w0;

	}

} );

function Vector3( x, y, z ) {
	x = x || 0;
	y = y || 0;
	z = z || 0;
	this.elements = new Float32Array([ x, y, z ]);
}

Vector3.prototype = {

	constructor: Vector3,

	isVector3: true,
	
	get x () {
		return this.elements[0];
	},

	set x ( value ) {
		this.elements[0] = value;
	},

	get y () {
		return this.elements[1];
	},

	set y ( value ) {
		this.elements[1] = value;
	},

	get z () {
		return this.elements[2];
	},

	set z ( value ) {
		this.elements[2] = value;
	},

	set: function ( x, y, z ) {
		var e = this.elements;
		e[0] = x;
		e[1] = y;
		e[2] = z;
		return this;
	},

	setScalar: function ( scalar ) {
		var e = this.elements;
		e[0] = scalar;
		e[1] = scalar;
		e[2] = scalar;
		return this;
	},

	setX: function ( x ) {
		this.elements[0] = x;
		return this;
	},

	setY: function ( y ) {
		this.elements[1] = y;
		return this;
	},

	setZ: function ( z ) {
		this.elements[2] = z;
		return this;
	},

	setComponent: function ( index, value ) {
		switch ( index ) {
			case 0: this.x = value; break;
			case 1: this.y = value; break;
			case 2: this.z = value; break;
			default: throw new Error( 'index is out of range: ' + index );
		}
		return this;
	},

	getComponent: function ( index ) {
		switch ( index ) {
			case 0: return this.x;
			case 1: return this.y;
			case 2: return this.z;
			default: throw new Error( 'index is out of range: ' + index );
		}
	},

	clone: function () {
		return new this.constructor( this.x, this.y, this.z );
	},

	copy: function ( v ) {
		return this.set(v.x, v.y, v.z);
	},

	add: function ( v, w ) {
		if ( w !== undefined ) {
			console.warn( 'THREE.Vector3: .add() now only accepts one argument. Use .addVectors( a, b ) instead.' );
			return this.addVectors( v, w );
		}
		vec3_add(this.elements, v.elements);
		return this;
	},

	addScalar: function ( s ) {
		vec3_addScalar(this.elements, s);
		return this;
	},

	addVectors: function ( a, b ) {
		vec3_addVectors(this.elements, a.elements, b.elements);
		return this;
	},

	addScaledVector: function ( v, s ) {
		vec3_addScaledVector(this.elements, v.elements, s);
		return this;
	},

	sub: function ( v, w ) {
		if ( w !== undefined ) {
			console.warn( 'THREE.Vector3: .sub() now only accepts one argument. Use .subVectors( a, b ) instead.' );
			return this.subVectors( v, w );
		}
		vec3_sub(this.elements, v.elements);
		return this;
	},

	subScalar: function ( s ) {
		vec3_subScalar(this.elements, s);
		return this;
	},

	subVectors: function ( a, b ) {
		vec3_subVectors(this.elements, a.elements, b.elements);
		return this;
	},

	multiply: function ( v, w ) {
		if ( w !== undefined ) {
			console.warn( 'THREE.Vector3: .multiply() now only accepts one argument. Use .multiplyVectors( a, b ) instead.' );
			return this.multiplyVectors( v, w );
		}
		vec3_multiply(this.elements, v.elements);
		return this;
	},

	multiplyScalar: function ( s ) {
		vec3_multiplyScalar(this.elements, s);
		return this;
	},

	multiplyVectors: function ( a, b ) {
		vec3_multiplyVectors(this.elements, a.elements, b.elements);
		return this;
	},
	applyEuler: function (e) {
		vec3_applyEuler(this.elements, e.elements, e.order);
		return this;
	},

	applyAxisAngle: function (axis, angle) {
		vec3_applyAxisAngle(this.elements, axis.elements, angle);
		return this;
	},

	applyMatrix3: function ( m ) {
		vec3_applyMatrix3(this.elements, m.elements);
		return this;
	},

	applyMatrix4: function ( m ) {
		vec3_applyMatrix4(this.elements, m.elements);
		return this;
	},

	applyProjection: function ( m ) {
		vec3_applyProjection(this.elements, m.elements);
		return this;
	},

	applyQuaternion: function ( q ) {
		vec3_applyQuaternion(this.elements, q.elements);
		return this;
	},

	project: function (camera) {
		vec3_project(this.elements, camera.projectionMatrix.elements, camera.matrixWorld.elements);
		return this;
	},

	unproject: function () {
		vec3_unproject(this.elements, camera.matrixWorld.elements, camera.projectionMatrix.elements);
		return this;
	},

	transformDirection: function ( m ) {
		vec3_transformDirection(this.elements, m.elements);
		return this;
	},

	divide: function ( v ) {
		vec3_divide(this.elements, v.elements);
		return this;
	},

	divideScalar: function ( s ) {
		vec3_divideScalar(this.elements, s);
		return this;
	},

	min: function ( v ) {
		vec3_min(this.elements, v.elements);
		return this;
	},

	max: function ( v ) {
		vec3_max(this.elements, v.elements);
		return this;
	},

	clamp: function ( min, max ) {
		vec3_clamp(this.elements, min.elements, max.elements);
		return this;
	},

	clampScalar: function (minVal, maxVal) {
		vec3_clampScalar(this.elements, minVal, maxVal);
		return this;
	},

	clampLength: function ( min, max ) {
		vec3_clampLength(this.elements, min, max);
		return this;
	},

	floor: function () {
		vec3_floor(this.elements);
		return this;
	},

	ceil: function () {
		vec3_ceil(this.elements);
		return this;
	},

	round: function () {
		vec3_round(this.elements);
		return this;
	},

	roundToZero: function () {
		vec3_roundToZero(this.elements);
		return this;
	},

	negate: function () {
		vec3_negate(this.elements);
		return this;
	},

	dot: function ( v ) {
		return vec3_dot(this.elements, v.elements);
	},

	lengthSq: function () {
		return vec3_lengthSq(this.elements);
	},

	length: function () {
		return vec3_length(this.elements);
	},

	lengthManhattan: function () {
		return vec3_lengthManhattan(this.elements);
	},

	normalize: function () {
		vec3_normalize(this.elements);
		return this;
	},

	setLength: function ( length ) {
		vec3_setLength(this.elements, length);
		return this;
	},

	lerp: function ( v, alpha ) {
		vec3_lerp(this.elements, v.elements, alpha);
		return this;
	},

	lerpVectors: function ( v1, v2, alpha ) {
		vec3_lerpVectors(this.elements, v1.elements, v2.elements, alpha);
		return this;
	},

	cross: function ( v, w ) {
		if ( w !== undefined ) {
			console.warn( 'THREE.Vector3: .cross() now only accepts one argument. Use .crossVectors( a, b ) instead.' );
			return this.crossVectors( v, w );
		}
		vec3_cross(this.elements, v.elements);
		return this;
	},

	crossVectors: function ( a, b ) {
		vec3_crossVectors(this.elements, a.elements, b.elements);
		return this;
	},

	projectOnVector: function ( v ) {
		vec3_projectOnVector(this.elements, v.elements);
		return this;
	},

	projectOnPlane: function (normal) {
		vec3_projectOnPlane(this.elements, normal.elements);
	},

	reflect: function (normal) {
		vec3_reflect(this.elements, normal.elements);
		return this;
	},

	angleTo: function ( v ) {
		return vec3_angleTo(this.elements, v.elements);
	},

	distanceTo: function ( v ) {
		return vec3_distanceTo(this.elements, v.elements);
	},

	distanceToSquared: function ( v ) {
		return vec3_distanceToSquared(this.elements, v.elements);
	},

	distanceToManhattan: function ( v ) {
		return vec3_distanceToManhattan(this.elements, v.elements);
	},

	setFromSpherical: function( s ) {
		var sinPhiRadius = Math.sin( s.phi ) * s.radius;

		this.x = sinPhiRadius * Math.sin( s.theta );
		this.y = Math.cos( s.phi ) * s.radius;
		this.z = sinPhiRadius * Math.cos( s.theta );

		return this;
	},

	setFromMatrixPosition: function ( m ) {
		vec3_setFromMatrixPosition(this.elements, m.elements);
		return this;
	},

	setFromMatrixScale: function ( m ) {
		vec3_setFromMatrixScale(this.elements, m.elements);
		return this;
	},

	setFromMatrixColumn: function ( m, index ) {
		if ( typeof m === 'number' ) {
			console.warn( 'THREE.Vector3: setFromMatrixColumn now expects ( matrix, index ).' );
			var temp = m
			m = index;
			index = temp;
		}

		vec3_setFromMatrixColumn(this.elements, m.elements, index);
		return this;
	},

	equals: function ( v ) {
		return vec3_equals(this.elements, v.elements);
	},

	fromArray: function ( array, offset ) {
		if ( offset === undefined ) offset = 0;
		var e = this.elements;
		e[0] = array[ offset ];
		e[1] = array[ offset + 1 ];
		e[2] = array[ offset + 2 ];
		return this;
	},

	toArray: function ( array, offset ) {
		if ( array === undefined ) array = new Float32Array(3);
		if ( offset === undefined ) offset = 0;
		var e = this.elements;
		array[ offset ] = e[0];
		array[ offset + 1 ] = e[1];
		array[ offset + 2 ] = e[2];
		return this;
	},

	fromAttribute: function ( attribute, index, offset ) {
		if ( offset === undefined ) offset = 0;
		
		index = index * attribute.itemSize + offset;
		this.x = attribute.array[ index ];
		this.y = attribute.array[ index + 1 ];
		this.z = attribute.array[ index + 2 ];
		return this;
	}
};

function Vector4( x, y, z, w ) {
	x = x || 0;
	y = y || 0;
	z = z || 0;
	w = ( w !== undefined ) ? w : 1;
	this.elements = new Float32Array([ x, y, z, w ]);
}

Vector4.prototype = {

	constructor: Vector4,

	isVector4: true,

	get x () {
		return this.elements[0];
	},

	set x ( value ) {
		this.elements[0] = value;
	},

	get y () {
		return this.elements[1];
	},

	set y ( value ) {
		this.elements[1] = value;
	},

	get z () {
		return this.elements[2];
	},

	set z ( value ) {
		this.elements[2] = value;
	},
	
	get w () {
		return this.elements[3];
	},

	set w ( value ) {
		this.elements[3] = value;
	},
	
	set: function ( x, y, z, w ) {
		var e = this.elements;
		e[0] = x;
		e[1] = y;
		e[2] = z;
		e[3] = w;
		return this;
	},

	setScalar: function ( s ) {
		var e = this.elements;
		e[0] = s;
		e[1] = s;
		e[2] = s;
		e[3] = s;
		return this;
	},

	setX: function ( x ) {
		this.elements[0] = x;
		return this;
	},

	setY: function ( y ) {
		this.elements[1] = y;
		return this;
	},

	setZ: function ( z ) {
		this.elements[2] = z;
		return this;
	},

	setW: function ( w ) {
		this.elements[3] = w;
		return this;
	},

	setComponent: function ( index, value ) {
		switch ( index ) {
			case 0: this.x = value; break;
			case 1: this.y = value; break;
			case 2: this.z = value; break;
			case 3: this.w = value; break;
			default: throw new Error( 'index is out of range: ' + index );
		}
		return this;
	},

	getComponent: function ( index ) {
		switch ( index ) {
			case 0: return this.x;
			case 1: return this.y;
			case 2: return this.z;
			case 3: return this.w;
			default: throw new Error( 'index is out of range: ' + index );
		}
	},

	clone: function () {
		return new this.constructor( this.x, this.y, this.z, this.w );
	},

	copy: function ( v ) {
		this.x = v.x;
		this.y = v.y;
		this.z = v.z;
		this.w = ( v.w !== undefined ) ? v.w : 1;
		return this;
	},

	add: function ( v, w ) {
		if ( w !== undefined ) {
			console.warn( 'THREE.Vector4: .add() now only accepts one argument. Use .addVectors( a, b ) instead.' );
			return this.addVectors( v, w );
		}
		vec4_add(this.elements, v.elements);
		return this;
	},

	addScalar: function ( s ) {
		addScalar(this.elements, s);
		return this;
	},

	addVectors: function ( a, b ) {
		vec4_addVectors(this.elements, a.elements, b.elements);
		return this;
	},

	addScaledVector: function ( v, s ) {
		vec4_addScaledVector(this.elements, v.elements, s);
		return this;
	},

	sub: function ( v, w ) {
		if ( w !== undefined ) {
			console.warn( 'THREE.Vector4: .sub() now only accepts one argument. Use .subVectors( a, b ) instead.' );
			return this.subVectors( v, w );
		}
		vec4_sub(this.elements, v.elements);
		return this;
	},

	subScalar: function ( s ) {
		vec4_subScalar(this.elements, s);
		return this;
	},

	subVectors: function ( a, b ) {
		vec4_subVectors(this.elements, a.elements, b.elements);
		return this;
	},

	multiplyScalar: function ( s ) {
		vec4_multiplyScalar(this.elements, s);
		return this;
	},

	applyMatrix4: function ( m ) {
		vec4_applyMatrix4(this.elements, m.elements);
		return this;
	},

	divideScalar: function ( s ) {
		vec4_divideScalar(this.elements, s);
		return this;
	},

	setAxisAngleFromQuaternion: function ( q ) {
		vec4_setAxisAngleFromQuaternion(this.elements, q.elements);
		return this;
	},

	setAxisAngleFromRotationMatrix: function ( m ) {
		vec4_setAxisAngleFromRotationMatrix(this.elements, m.elements);
		return this;
	},

	min: function ( v ) {
		vec4_min(this.elements, v.elements);
		return this;
	},

	max: function ( v ) {
		vec4_max(this.elements, v.elements);
		return this;
	},

	clamp: function ( min, max ) {
		vec4_clamp(this.elements, min.elements, max.elements);
		return this;
	},

	clampScalar: function (minVal, maxVal) {
		vec4_clampScalar(this.elements, minVal, maxVal);
		return this;
	},

	floor: function () {
		vec4_floor(this.elements);
		return this;
	},

	ceil: function () {
		vec4_ceil(this.elements);
		return this;
	},

	round: function () {
		vec4_round(this.elements);
		return this;
	},

	roundToZero: function () {
		vec4_roundToZero(this.elements);
		return this;
	},

	negate: function () {
		vec4_negate(this.elements);
		return this;
	},

	dot: function ( v ) {
		return vec4_dot(this.elements, v.elements);
	},

	lengthSq: function () {
		return vec4_lengthSq(this.elements);
	},

	length: function () {
		return vec4_length(this.elements);
	},

	lengthManhattan: function () {
		return vec4_lengthManhattan(this.elements);
	},

	normalize: function () {
		vec4_normalize(this.elements);
		return this;
	},

	setLength: function ( length ) {
		vec4_setLength(this.elements, length);
		return this;
	},

	lerp: function ( v, alpha ) {
		vec4_lerp(this.elements, v.elements, alpha);
		return this;
	},

	lerpVectors: function ( v1, v2, alpha ) {
		vec4_lerpVectors(this.elements, v1.elements, v2.elements, alpha);
		return this;
	},

	equals: function ( v ) {
		return vec4_equals(this.elements, v.elements);
	},

	fromArray: function ( array, offset ) {
		if ( offset === undefined ) offset = 0;
		var e = this.elements;
		e[0] = array[ offset ];
		e[1] = array[ offset + 1 ];
		e[2] = array[ offset + 2 ];
		e[3] = array[ offset + 3 ];
		return this;
	},

	toArray: function ( array, offset ) {
		if ( array === undefined ) array = [];
		if ( offset === undefined ) offset = 0;
		var e = this.elements;
		array[ offset ] = e[0];
		array[ offset + 1 ] = e[1];
		array[ offset + 2 ] = e[2];
		array[ offset + 3 ] = e[3];
		return array;
	},

	fromAttribute: function ( attribute, index, offset ) {
		if ( offset === undefined ) offset = 0;
		index = index * attribute.itemSize + offset;

		this.x = attribute.array[ index ];
		this.y = attribute.array[ index + 1 ];
		this.z = attribute.array[ index + 2 ];
		this.w = attribute.array[ index + 3 ];
		return this;
	}
};
