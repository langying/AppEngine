function HTMLElement(tagName) {
	this._src = '';
	this.style = {};
	this.children = [];
	this.tagName = (tagName && tagName.toLowerCase()) || '';
	this.onload = function() {
	};
}
HTMLElement.prototype = HTMLElement;
HTMLElement.prototype.constructor = HTMLElement;
Object.defineProperties(HTMLElement.prototype, {
	src : {
		get : function() {
			return this._src;
		},
		set : function(value) {
			this._src = value;
			if (this.tagName == 'script') {
				__execute(value);
			}
		}
	},
	appendChild : {
		value : function(element) {
			element.ownerDocument = this;
			this.children.push(element);
		}
	},
	removeChild : {
		value : function(element) {
			for (var i = this.children.length; i--;) {
				if (this.children[i] === element) {
					this.children.splice(i, 1);
					element.ownerDocument = null;
				}
			}
		}
	},
	getAttribute : {
		value : function(attr) {
			return this[attr];
		}
	},
	setAttribute : {
		value : function(attr, value) {
			this[attr] = value;
		}
	},
	insertBefore : {
		value : function(newElement, existingElement) {
			this.children.push(newElement);
		}
	},
	addEventListener : {
		value : function(event, method) {
			if (event == 'load') {
				this.onload = method;
			}
			else {
				document.addEventListener(event, method);
			}
		}
	},
	removeEventListener : {
		value : function(event, method) {
			document.removeEventListener(event, method);
		}
	},
	getBoundingClientRect : {
		value : function() {
			return {
				top : 0,
				left : 0,
				width : window.innerWidth,
				height : window.innerHeight
			};
		}
	}
});

Object.defineProperties(HTMLAudioElement.prototype, {
	setAttribute : {
		value : function(key, val) {
		}
	},
	onFinish : {
		value : function() {
			this.onload && this.onload();
		}
	},
	addEventListener : {
		value : function(event, method) {
			if (event == 'load') {
				this.onload = method;
			}
		}
	}
});

Object.defineProperties(HTMLVideoElement.prototype, {
	setAttribute : {
		value : function(key, val) {
		}
	},
	onFinish : {
		value : function() {
			this.onload && this.onload();
		}
	},
	addEventListener : {
		value : function(event, method) {
			if (event == 'load') {
				this.onload = method;
			}
		}
	}
});

function HTMLCanvasElement() {
	HTMLElement.call(this);
	this._width = 300;
	this._height = 150;
	this._context = null;
}
HTMLCanvasElement.prototype = Object.create(HTMLElement.prototype);
HTMLCanvasElement.prototype.constructor = HTMLCanvasElement;
Object.defineProperties(HTMLCanvasElement.prototype, {
	width : {
		get : function() {
			return this._width;
		},
		set : function(value) {
			if (this._width !== value) {
				this._width = value;
				this._context && this._context.size(this._width, this._height);
			}
		}
	},
	height : {
		get : function() {
			return this._height;
		},
		set : function(value) {
			if (this._height !== value) {
				this._height = value;
				this._context && this._context.size(this._width, this._height);
			}
		}
	},
	getContext : {
		value : function(type, attrs) {
			if (this._context) {
				return this._context;
			}
			if (type === 'webgl') {
				this._context = new WebGLRenderingContext(attrs);
			}
			else {
				this._context = new CanvasRenderingContext2D();
				this._context.size(this.width, this.height);
			}
			return this._context;
		}
	},
	exitFullscreen : {
		value : function(args) {
			if (args && args.vrDisplay) {
				args.vrDisplay.isPresenting = false;
			}
			setTimeout(function() {
				document.dispatchEvent(new Event('fullscreenchange'));
			}, 1);
		}
	},
	requestFullscreen : {
		value : function(args) {
			if (args && args.vrDisplay) {
				args.vrDisplay.isPresenting = true;
			}
			setTimeout(function() {
				document.dispatchEvent(new Event('fullscreenchange'));
			}, 1);
		}
	}
});

function Audio() {
}
Audio.prototype = Audio;
Audio.prototype.constructor = Audio;
Object.defineProperties(Audio.prototype, {
	canPlayType : {
		value : function(ext) {
			if (ext == 'audio/mpeg') {
				return 'probably';
			}
			return '';
		}
	}
});
function AudioContext() {
	this.destination = new AESound();
}
AudioContext.prototype = AudioContext;
AudioContext.prototype.constructor = AudioContext;
Object.defineProperties(AudioContext.prototype, {
	decodeAudioData : {
		value : function(response, success, failure) {
			var sound = this.destination.load(response);
			var buffer = AudioBuffer();
			buffer.soundId = sound.id;
			buffer.duration = sound.duration;
			success(buffer);
		}
	},
	createBufferSource : {
		value : function() {
			return new AudioBufferSourceNode(this.destination);
		}
	},
});
function AudioBuffer() {
	this.length = 0;
	this.soundId = 0;
	this.duration = 0;
	this.numberOfChannels = 1;
}
function AudioBufferSourceNode() {
	this.loop = false;
	this.buffer = null;
	this.streamId = 0;
	this.destination = null;
}
AudioBufferSourceNode.prototype = AudioBufferSourceNode;
AudioBufferSourceNode.prototype.constructor = AudioBufferSourceNode;
Object.defineProperties(AudioBufferSourceNode.prototype, {
	stop : {
		value : function(time) {
			this.destination.stop(this.streamId);
		}
	},
	start : {
		value : function(time) {
			this.streamId = this.destination.play(this.buffer);
		}
	},
	connect : {
		value : function(destination) {
			this.destination = destination;
		}
	}
});

function SpeechSynthesisUtterance(text) {
	this.text = text || "";
	this.lang = "en-US";
	this.rate = 1.0;
	this.pitch = null;
	this.voice = null;
	this.volume = null;
}
SpeechSynthesisUtterance.prototype = SpeechSynthesisUtterance;
SpeechSynthesisUtterance.prototype.constructor = SpeechSynthesisUtterance;

function Document(win) {
	this.head = new HTMLElement('head');
	this.body = new HTMLElement('body');
	this.style = {};
	this.events = {};
	this.hidden = false;
	this.location = win.location;
	this.readystate = 'complete';
	this.visibilityState = 'visible';
	this.documentElement = win;

	this.body.clientWidth = win.innerWidth;
	this.body.clientHeight = win.innerHeight;
}
Document.prototype = Document;
Document.prototype.constructor = Document;
Object.defineProperties(Document.prototype, {
	createEvent : {
		value : function(type) {
			return new Event(type);
		}
	},
	createElement : {
		value : function(name) {
			var ele, tag = name.toLowerCase();
			if (tag === 'img') {
				ele = new HTMLImageElement();
			}
			else if (tag == 'video') {
				ele = new HTMLVideoElement();
			}
			else if (tag === 'canvas') {
				ele = new HTMLCanvasElement();
			}
			else {
				ele = new HTMLElement(tag);
			}
			ele.parentElement = this.body;
			return ele;
		}
	},
	createElementNS : {
		value : function(uri, name) {
			return this.createElement(name);
		}
	},
	getElementById : {
		value : function(elementId) {
			var children = this.body.children;
			for (var i = 0, length = children.length; i < length; i++) {
				if (children[i].id === elementId) {
					return children[i];
				}
			}
			return null;
		}
	},
	getElementsByTagName : {
		value : function(tagName) {
			tagName = tagName.toLowerCase();
			var elements = [];
			if (tagName === 'head') {
				elements.push(this.head);
			}
			else if (tagName === 'body') {
				elements.push(this.body);
			}
			else {
				var children = document.body.children;
				for (var i = 0, length = children.length; i < length; i++) {
					if (children[i].tagName === tagName) {
						elements.push(children[i]);
					}
				}
			}
			return elements;
		}
	},
	dispatchEvent : {
		value : function(event) {
			var listeners = this.events[event.type];
			if (!listeners) {
				return;
			}
			for (var i = 0; i < listeners.length; i++) {
				listeners[i](event);
			}
		}
	},
	addEventListener : {
		value : function(type, callback, useCapture) {
			if (type == 'DOMContentLoaded') {
				setTimeout(callback, 1);
				return;
			}
			if (!this.events[type]) {
				this.events[type] = [];
			}
			this.events[type].push(callback);
		}
	},
	removeEventListener : {
		value : function(type, callback) {
			var listeners = this.events[type];
			if (!listeners) {
				return;
			}

			for (var i = listeners.length; i--;) {
				if (listeners[i] === callback) {
					listeners.splice(i, 1);
				}
			}
		}
	}
});

// VRDisplay 对象属于Native对象，已经预置到引擎中
Object.defineProperties(VRDisplay.prototype, {
	capabilities : {
		value : {
			canPresent : false,
			hasPosition : false,
			hasOrientation : true,
			hasExternalDisplay : false
		}
	},
	getPose : {
		value : function() {
			if (!this.pose) {
				this.pose = {
					position : null,
					timeStamp : 0,
					orientation : [ 0, 0, 0, 1 ],
					linearVelocity : 0,
					angularVelocity : 0,
					linearAcceleration : 0,
					angularAcceleration : 0
				};
			}
			this.update();
			var o = this.pose.orientation;
			o[0] = this.x;
			o[1] = this.y;
			o[2] = this.z;
			o[3] = this.w;
			return this.pose;
		}
	},
	getEyeParameters : {
		value : function(which) {
			if (which === 'left') {
				if (!this.left) {
					this.left = {
						offset : [ -0.032, 0, 0 ],
						fieldOfView : {},
						renderWidth : __width / 2,
						renderHeight : __height
					};
				}
				var fov = this.left.fieldOfView;
				fov.upDegrees = this.lu;
				fov.downDegrees = this.ld;
				fov.leftDegrees = this.ll;
				fov.rightDegrees = this.lr;
				return this.left;
			}
			else {
				if (!this.right) {
					this.right = {
						offset : [ 0.032, 0, 0 ],
						fieldOfView : {},
						renderWidth : __width / 2,
						renderHeight : __height
					};
				}
				var fov = this.right.fieldOfView;
				fov.upDegrees = this.ru;
				fov.downDegrees = this.rd;
				fov.leftDegrees = this.rl;
				fov.rightDegrees = this.rr;
				return this.right;
			}
		}
	},
	requestAnimationFrame : {
		value : requestAnimationFrame
	},
	exitPresent : {
		value : function() {
			this.isPresenting = false;
		}
	},
	requestPresent : {
		value : function(args) {
			this.isPresenting = true;
			var canvas = args[0].source;
			if (canvas) {
				canvas.requestFullscreen();
			}
		}
	}
});

Object.defineProperties(XMLHttpRequest.prototype, {
	onFinish : {
		value : function() {
			var url = this.responseURL;
			var event = {
				target : {
					response : url
				}
			};
			if (this.responseType.indexOf('text') >= 0) {
				event.target.response = this.responseText;
			}
			this.onload && this.onload(event);
		}
	},
	addEventListener : {
		value : function(event, method) {
			if (event == 'load') {
				this.onload = method;
			}
		}
	}
});

// HTMLImageElement 对象属于Native对象，已经预置到引擎中
Object.defineProperties(HTMLImageElement.prototype, {
	onFinish : {
		value : function() {
			this.onload && this.onload();
		}
	},
	addEventListener : {
		value : function(event, method) {
			if (event == 'load') {
				this.onload = method;
			}
		}
	}
});

Object.defineProperties(WebSocket.prototype, {
	onFinish : {
		value : function(type, message, code, remote) {
			var evt = new Event(type);
			evt.code = code;
			evt.data = message;
			evt.reason = message;
			evt.message = message;
			if (type == 'onopen') {
				this.onopen && this.onopen(evt);
			}
			else if (type == 'onclose') {
				this.onclose && this.onclose(evt);
			}
			else if (type == 'onerror') {
				this.onerror && this.onerror(evt);
			}
			else if (type == 'onmessage') {
				this.onmessage && this.onmessage(evt);
			}
			else {
				console.error('WebSocket.onFinish type is invaild:' + type);
			}
		}
	}
});

function Touch(x, y) {
	this._x = x;
	this._y = y;
}
Touch.prototype = Touch;
Touch.prototype.constructor = Touch;
Object.defineProperties(Touch.prototype, {
	x : {
		get : function() {
			return this._x;
		},
		set : function(value) {
			this._x = value;
		}
	},
	y : {
		get : function() {
			return this._y;
		},
		set : function(value) {
			this._y = value;
		}
	},
	pageX : {
		get : function() {
			return this._x;
		},
		set : function(value) {
			this._x = value;
		}
	},
	pageY : {
		get : function() {
			return this._y;
		},
		set : function(value) {
			this._y = value;
		}
	},
	clientX : {
		get : function() {
			return this._x;
		},
		set : function(value) {
			this._x = value;
		}
	},
	clientY : {
		get : function() {
			return this._y;
		},
		set : function(value) {
			this._y = value;
		}
	},
	screenX : {
		get : function() {
			return this._x;
		},
		set : function(value) {
			this._x = value;
		}
	},
	screenY : {
		get : function() {
			return this._y;
		},
		set : function(value) {
			this._y = value;
		}
	}
});

function Event(type) {
	this.type = type;
	this.repeat = false;
	this.which = 0;
	this.keyCode = 0;
	this.touches = [];
}
Event.prototype = Event;
Event.prototype.constructor = Event;
Object.defineProperties(Event.prototype, {
	x : {
		get : function() {
			var touch = this.touches[0];
			return touch ? touch.x : 0;
		},
		set : function(value) {
			var touch = this.touches[0];
			if (touch) {
				touch.x = value;
			}
		}
	},
	y : {
		get : function() {
			var touch = this.touches[0];
			return touch ? touch.y : 0;
		},
		set : function(value) {
			var touch = this.touches[0];
			if (touch) {
				touch.y = value;
			}
		}
	},
	pageX : {
		get : function() {
			var touch = this.touches[0];
			return touch ? touch.x : 0;
		},
		set : function(value) {
			var touch = this.touches[0];
			if (touch) {
				touch.x = value;
			}
		}
	},
	pageY : {
		get : function() {
			var touch = this.touches[0];
			return touch ? touch.y : 0;
		},
		set : function(value) {
			var touch = this.touches[0];
			if (touch) {
				touch.y = value;
			}
		}
	},
	clientX : {
		get : function() {
			var touch = this.touches[0];
			return touch ? touch.x : 0;
		},
		set : function(value) {
			var touch = this.touches[0];
			if (touch) {
				touch.x = value;
			}
		},
	},
	clientY : {
		get : function() {
			var touch = this.touches[0];
			return touch ? touch.y : 0;
		},
		set : function(value) {
			var touch = this.touches[0];
			if (touch) {
				touch.y = value;
			}
		},
	},
	screenX : {
		get : function() {
			var touch = this.touches[0];
			return touch ? touch.x : 0;
		},
		set : function(value) {
			var touch = this.touches[0];
			if (touch) {
				touch.x = value;
			}
		},
	},
	screenY : {
		get : function() {
			var touch = this.touches[0];
			return touch ? touch.y : 0;
		},
		set : function(value) {
			var touch = this.touches[0];
			if (touch) {
				touch.y = value;
			}
		}
	},
	changedTouches : {
		get : function() {
			return this.touches;
		},
		set : function(value) {
			this.touches = value;
		}
	},
	preventDefault : {
		value : function() {
		}
	},
	stopPropagation : {
		value : function() {
		}
	},
	autoPreventDefault : {
		value : function() {
			return true;
		}
	}
});

Event.createTouchEvent = function(type, pxs, pys) {
	var event = new Event(type);
	for (var idx = 0, len = pxs.length; idx < len; idx++) {
		event.touches.push(new Touch(pxs[idx], pys[idx]));
	}
	return event;
};
Event.createKeyboardEvent = function(type, keyCode) {
	var event = new Event(type);
	event.repeat = false;
	event.which = keyCode;
	event.keyCode = keyCode;
	return event;
};

EventLoop = {
	timers : [],
	expiring : null,
	nextTimerId : 1,
	minimumDelay : 1,
	minimumWait : 1,
	maximumWait : 60000,
	maxExpirys : 10
};
EventLoop.dumpState = function() {
	console.log('TIMER STATE:');
	this.timers.forEach(function(timer) {
		console.log(timer);
	});
	if (this.expiring) {
		console.log('EXPIRING: ' + this.expiring);
	}
};
EventLoop.getEarliestTimer = function() {
	var n = this.timers.length;
	return (n > 0 ? this.timers[n - 1] : null);
};
EventLoop.getEarliestWait = function() {
	var t = this.getEarliestTimer();
	return (t ? t.target - Date.now() : null);
};
EventLoop.insertTimer = function(timer) {
	var timers = this.timers;
	var i, n, t;
	n = timers.length;
	for (i = n - 1; i >= 0; i--) {
		t = timers[i];
		if (timer.target <= t.target) {
			break;
		}
	}
	timers.splice(i + 1 /* start */, 0 /* deleteCount */, timer);
};
EventLoop.removeTimerById = function(timer_id) {
	var timers = this.timers;
	var i, n, t;

	t = this.expiring;
	if (t) {
		if (t.id === timer_id) {
			t.removed = true;
			return;
		}
	}

	n = timers.length;
	for (i = 0; i < n; i++) {
		t = timers[i];
		if (t.id === timer_id) {
			t.removed = true;
			this.timers.splice(i /* start */, 1 /* deleteCount */);
			return;
		}
	}
};
EventLoop.processTimers = function() {
	var now = Date.now();
	var timers = this.timers;
	var sanity = this.maxExpirys;
	var n, t;

	while (sanity-- > 0) {
		n = timers.length;
		if (n <= 0) {
			break;
		}
		t = timers[n - 1];
		if (now <= t.target) {
			break;
		}
		timers.pop();
		if (t.oneshot) {
			t.removed = true;
		}
		else {
			t.target = now + t.delay;
		}
		this.expiring = t;
		try {
			t.cb();
		}
		catch (e) {
			console.log('timer callback failed, ignored: ' + e);
		}
		this.expiring = null;

		if (!t.removed) {
			this.insertTimer(t);
		}
	}
};
function setTimeout(func, delay) {
	var cb_func;
	var bind_args;
	var timer_id;
	var evloop = EventLoop;

	if (typeof delay !== 'number') {
		throw new TypeError('delay is not a number');
	}
	delay = Math.max(evloop.minimumDelay, delay);

	if (typeof func === 'string') {
		cb_func = eval.bind(this, func);
	}
	else if (typeof func !== 'function') {
		throw new TypeError('callback is not a function/string');
	}
	else if (arguments.length > 2) {
		bind_args = Array.prototype.slice.call(arguments, 2);
		bind_args.unshift(this);
		cb_func = func.bind.apply(func, bind_args);
	}
	else {
		cb_func = func;
	}

	timer_id = evloop.nextTimerId++;
	evloop.insertTimer({
		id : timer_id,
		oneshot : true,
		cb : cb_func,
		delay : delay,
		target : Date.now() + delay
	});
	return timer_id;
}
function clearTimeout(timer_id) {
	var evloop = EventLoop;
	if (typeof timer_id !== 'number') {
		throw new TypeError('timer ID is not a number');
	}
	evloop.removeTimerById(timer_id);
}
function setInterval(func, delay) {
	var cb_func;
	var bind_args;
	var timer_id;
	var evloop = EventLoop;

	if (typeof delay !== 'number') {
		throw new TypeError('delay is not a number');
	}
	delay = Math.max(evloop.minimumDelay, delay);

	if (typeof func === 'string') {
		cb_func = eval.bind(this, func);
	}
	else if (typeof func !== 'function') {
		throw new TypeError('callback is not a function/string');
	}
	else if (arguments.length > 2) {
		bind_args = Array.prototype.slice.call(arguments, 2);
		bind_args.unshift(this); // [ global(this), arg1, arg2, ... ]
		cb_func = func.bind.apply(func, bind_args);
	}
	else {
		cb_func = func;
	}

	timer_id = evloop.nextTimerId++;
	evloop.insertTimer({
		id : timer_id,
		oneshot : false,
		cb : cb_func,
		delay : delay,
		target : Date.now() + delay
	});
	return timer_id;
}
function clearInterval(timer_id) {
	if (typeof timer_id !== 'number') {
		throw new TypeError('timer ID is not a number');
	}
	EventLoop.removeTimerById(timer_id);
}

this.self = this;
this.window = this;

window.top = window;
window.Audio = HTMLAudioElement;
window.Image = HTMLImageElement;
window.TouchEvent = Event;
window.parent = window;
window.innerWidth = __width;
window.innerHeight = __height;
window.devicePixelRatio = __scale;
window.document = new Document(window);
window.ontouchend = null;
window.ontouchmove = null;
window.ontouchstart = null;
window.ontouchcancel = null;
window.getSpeechSynthesis = function() {
	if (!window.speechSynthesis) {
		window.speechSynthesis = new SpeechSynthesis();
	}
	return window.speechSynthesis;
};
window.URL = {
	createObjectURL : function(src) {
		return src || 'camera://video';
	},
	revokeObjectURL : function(src) {
	}
};
window.screen = {
	width : __width,
	height : __height,
	orientation : {
		angle : 90,
	}
};
window.console = {
	debug : function() {
		__println(0, Array.prototype.slice.call(arguments).join(' '));
	},
	log : function() {
		__println(1, Array.prototype.slice.call(arguments).join(' '));
	},
	warn : function() {
		__println(2, Array.prototype.slice.call(arguments).join(' '));
	},
	error : function() {
		__println(3, Array.prototype.slice.call(arguments).join(' '));
	},
	assert : function() {
		__println(4, Array.prototype.slice.call(arguments).join(' '));
	}
};
window.location = {};
window.navigator = {
	language : 'zh-CN',
	userAgent : __agent,
	appVersion : __agent,
	cookieEnabled : true,
	mVRDisplay : null,
	getGamepads : function() {
		return [ null, null, null, null ];
	},
	getUserMedia : function(paramter, success, failure) {
		setTimeout(function() {
			success('camera://video');
		}, 1);
	},
	getVRDisplays : function() {
		if (this.mVRDisplay == null) {
			this.mVRDisplay = new VRDisplay();
		}
		var mList = [ this.mVRDisplay ];
		return {
			then : function(handle) {
				handle(mList);
			}
		};
	}
};
window.performance = {
	now : function() {
		return __now();
	}
};
window.localStorage = {
	getItem : function(name) {
		return __getItem(name);
	},
	setItem : function(name, text) {
		__setItem(name, text + '');
	},
	removeItem : function(name) {
		__delItem(name);
	}
};
window.addEventListener = function(event, method) {
	document.addEventListener(event, method);
};
window.removeEventListener = function(event, method) {
	document.removeEventListener(event, method);
};

function alert(title) {
	__alert(title);
}
function garbageCollect() {
	__garbage();
}
RAFList = []; // RequestAnimationFrameLoop
function RAFExec() {
	var time = Date.now();
	var list = RAFList;
	RAFList = [];
	for (var i = 0, l = list.length; i < l; i++) {
		list[i](time);
	}
}
function cancelAnimationFrame(id) {
}
function requestAnimationFrame(cb) {
	RAFList.push(cb);
}
function __loadURL(href, protocol, host, port, pathname, search, hash) {
	location.href = href;
	location.protocol = protocol;
	location.host = host;
	location.port = port;
	location.pathname = pathname;
	location.search = search;
	location.hash = hash;
	__execute(href);
}
function __applyTimer() {
	EventLoop.processTimers();
}
function __touch(type, pxs, pys) {
	document.dispatchEvent(Event.createTouchEvent(type, pxs, pys));
}
function __orientation(alpha, beta, gamma) {
	var event = new Event('deviceorientation');
	event.absolute = false;
	event.alpha = alpha;
	event.beta = beta;
	event.gamma = gamma;
	document.dispatchEvent(event);
}
function __applyResize(w, h) {
	var event = new Event('resize');
	event.touches.push(new Touch(w, h));
	document.dispatchEvent(event);
}
function __applyKeyboard(type, keyCode) {
	document.dispatchEvent(Event.createKeyboardEvent(type, keyCode));
}
