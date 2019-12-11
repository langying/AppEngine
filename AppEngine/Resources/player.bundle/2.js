window.loadFile = window.__execute || function() {
};
window.loadFile('file:///asset/AppEngine.bundle/three.min.js');
window.loadFile('file:///asset/AppEngine.bundle/promise.js');
window.loadFile('file:///asset/AppEngine.bundle/WebVR.js');
window.loadFile('file:///asset/AppEngine.bundle/VREffect.js');
window.loadFile('file:///asset/AppEngine.bundle/VRControls.js');

var video, scene, effect, camera, texture, controls, renderer;

scene = new THREE.Scene();
camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 1, 2000);
camera.layers.enable(1); // render left view when no stereo available
controls = new THREE.VRControls(camera);

renderer = new THREE.WebGLRenderer();
renderer.setClearColor(0x101010);
renderer.setPixelRatio(window.devicePixelRatio);
renderer.setSize(window.innerWidth, window.innerHeight);
document.body.appendChild(renderer.domElement);
document.addEventListener('resize', onWindowResize, false);

effect = new THREE.VREffect(renderer);
effect.scale = 0; // video doesn't need eye separation
effect.setSize(window.innerWidth, window.innerHeight);

// if (WEBVR.isAvailable() === true) {
// document.body.appendChild(WEBVR.getButton(effect));
// effect.requestPresent();
// }
if (WEBVR.isLatestAvailable() === false) {
	document.body.appendChild(WEBVR.getMessage());
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

video = document.createElement('video');
video.src = getQueryString("src");
;
video.loop = true;
video.setAttribute('webkit-playsinline', 'webkit-playsinline');
video.play();

texture = new THREE.VideoTexture(video);
texture.format = THREE.RGBFormat;
texture.minFilter = THREE.NearestFilter;
texture.maxFilter = THREE.NearestFilter;
texture.generateMipmaps = false;

initLeft();
initRight();

animate();
function animate() {
	effect.requestAnimationFrame(animate);
	controls.update();
	effect.render(scene, camera);
}

function initLeft() {
	var material = new THREE.MeshBasicMaterial({
		map : texture
	});
	var geometry = new THREE.SphereGeometry(500, 60, 40);
	geometry.scale(-1, 1, 1);

	var uvs = geometry.faceVertexUvs[0];
	for (var i = 0, l = uvs.length; i < l; i++) {
		for (var j = 0; j < 3; j++) {
			uvs[i][j].x *= 0.5;
		}
	}

	var mesh = new THREE.Mesh(geometry, material);
	mesh.rotation.y = -Math.PI / 2;
	mesh.layers.set(1);
	scene.add(mesh);

}
function initRight() {
	var material = new THREE.MeshBasicMaterial({
		map : texture
	});
	var geometry = new THREE.SphereGeometry(500, 60, 40);
	geometry.scale(-1, 1, 1);

	var uvs = geometry.faceVertexUvs[0];
	for (var i = 0, l = uvs.length; i < l; i++) {
		for (var j = 0; j < 3; j++) {
			uvs[i][j].x *= 0.5;
			uvs[i][j].x += 0.5;
		}
	}

	var mesh = new THREE.Mesh(geometry, material);
	mesh.rotation.y = -Math.PI / 2;
	mesh.layers.set(2);
	scene.add(mesh);
}

function onWindowResize() {
	camera.aspect = window.innerWidth / window.innerHeight;
	camera.updateProjectionMatrix();
	effect.setSize(window.innerWidth, window.innerHeight);
}

function getQueryString(name) {
	var reg = new RegExp('(^|&)' + name + '=([^&]*)(&|$)', 'i');
	var r = window.location.search.substr(1).match(reg);
	if (r) {
		return unescape(r[2]);
	}
	return null;
}
