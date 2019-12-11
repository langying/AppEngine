window.loadFile = window.__execute || function() {
};
window.loadFile('file:///asset/AppEngine.bundle/three.min.js');
window.loadFile('file:///asset/AppEngine.bundle/promise.js');
window.loadFile('file:///asset/AppEngine.bundle/WebVR.js');
window.loadFile('file:///asset/AppEngine.bundle/VREffect.js');
window.loadFile('file:///asset/AppEngine.bundle/VRControls.js');

var renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);

var path = 'http://aeapp.oss-cn-hangzhou.aliyuncs.com/skybox/room/';
var cube = [ 'px.jpg', 'nx.jpg', 'py.jpg', 'ny.jpg', 'pz.jpg', 'nz.jpg' ];

var camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.01, 1000);
var effect = new THREE.VREffect(renderer);
var loader = new THREE.TextureLoader();
var control = new THREE.VRControls(camera);

var video = document.createElement('video');
video.src = getQueryString("src");
video.loop = true;
video.setAttribute('webkit-playsinline', 'webkit-playsinline');
video.play();

var texture = new THREE.VideoTexture(video);
texture.format = THREE.RGBFormat;
texture.minFilter = THREE.NearestFilter;
texture.maxFilter = THREE.NearestFilter;
texture.generateMipmaps = false;

var screen = new THREE.Mesh(new THREE.PlaneBufferGeometry(40, 20), new THREE.MeshBasicMaterial({
	map : texture
}));
screen.position.z = -40;

var scene = new THREE.Scene();
scene.add(screen);
scene.add(new THREE.AmbientLight(0xFFFFFF));
scene.background = new THREE.CubeTextureLoader().setPath(path).load(cube);

animate();
function animate() {
	effect.requestAnimationFrame(animate);
	control.update();
	effect.render(scene, camera);
}

function getQueryString(name) {
	var reg = new RegExp('(^|&)' + name + '=([^&]*)(&|$)', 'i');
	var r = window.location.search.substr(1).match(reg);
	if (r) {
		return unescape(r[2]);
	}
	return null;
}

var is_vr_mode = false;
function __request_vr() {
	is_vr_mode = !is_vr_mode;
	if (is_vr_mode) {
		effect.requestPresent();
	}
	else {
		effect.exitPresent();
	}
}
