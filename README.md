遗留的问题：
所有WebGL的返回，均以number类型表示，在duktape中不是object，因此没有自己的属性字段，才导致了以下bug的出现：
1.0 three.js中的fetchUniformLocations/function initMaterial( material, fog, object ), 
if ( location ) { 函数，使用gl.getUniformLocation 返回的类型是number，标准返回的类型是WebGLUniformLocation
    当location != 0时候，二者在three.js的效果一样。
    当location == 0时候，而知在three.js的效果不同：表现在initMaterial函数的if(location)语句，number==0,表示条件不成立，WebGLUniformLocation的内容为0，但是对象存在，因此条件成立。
    当location == 0 || location == null || location == undeined的时候，则添加判断 if ( location || typeof location == 'number')[three.js中就是这么改的]
1.1 three.js中THREE.ImageLoader.load函数中:【已经修复了】
    image.addEventListener( 'load', callback);
    在callback中使用了this指针，在回调中this不是image，而是callback，导致image.width/height无法正常读取，因此改成this -> image
1.2 vShadowCoord[ i ] = shadowMatrix[ i ] * worldPosition;[GL编译shader的时候]
    Dynamic Indexing
    Dynamic indexing of arrays, vectors and matrices is not directly supported by some implementations. Program transforms exist but the performance is reduced.
    Should dynamic indexing of arrays be removed from the specification?
    RESOLUTION: Keep dynamic indexing of uniforms (for skinning). Remove for temps (in the limitations section).
1.3 texture的更新
    texture的needsUpdate属性对应的是version字段，是个默认为0的整数，当设置needsUpdate为ture时，会自动对version++，
    这样在setTexture函数中，发现version>0 && cache的version != version，就会触发uploadTexture函数进行纹理上传，显示图片
1.4 cubeTexture的更新
    renderer.autoClear = false;这种情况下会有问题，不能设置为false;
1.5 three.js中的正则表达式无法运行在duktape中【\_】改成【_】
    var re = /^(([\w]+\/)*)([\w-\d]+)?(\.([\w]+)(\[([\w\d\[\]\_. ]+)\])?)?(\.([\w.]+)(\[([\w\d\[\]\_. ]+)\])?)$/;
    var re = /^(([\w]+\/)*)([\w-\d]+)?(\.([\w]+)(\[([\w\d\[\]_. ]+)\])?)?(\.([\w.]+)(\[([\w\d\[\]_. ]+)\])?)$/;
1.6 H5终端document.createElement('canvas')的默认宽高是[300, 150]，在pixi中最好遵守默认，否则会出问题。
    因为isPowerOfTwo函数，在初始化状态下状态可能不一致，导致无法显示tex的bug，因为不同情况设置的参数不同：
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
2.0 配置pod的文档
    http://gitlab.alibaba-inc.com/alipods/specs/wikis/home
    http://tbdocs.alibaba-inc.com/pages/viewpage.action?pageId=218729086
    gem sources -a http://rubygems-china.oss.aliyuncs.com
    sudo gem uninstall cocoapods -v 0.35.0
3.0 char* path = getcwd(NULL, 0); free(path); 用来获得工作路径，需要手动free;
4.0 H5播放语音:
    var utterance = new SpeechSynthesisUtterance('Hello Treehouse');
    window.speechSynthesis.speak(utterance);


WebGL Demos:
https://developer.mozilla.org/zh-CN/demos/    

九大行星纹理图: http://www.celestiamotherlode.net/catalog/uranus.php
16K地球高清图: http://www.shadedrelief.com/natural3/pages/textures.html


ps与top在RSS上是一样的，如果只看RSS，那么直接使用top命令好了: top | grep chrome
dumpsys获取的是PSS，在魔盒上获取的PSS反而比RSS大很多，感觉数据不准【VSS>=RSS>=PSS>=USS】: dumpsys meminfo package/pid | grep xxx

std::string operator []操作第一步先看当前对象是否为共享的(即引用计数>0)，如果为共享对象，则拷贝一份新的数据作为本地变量，同时将老数据对象的引用计数减1，并最终返回新拷贝的数据，因此用户的操作都将在新的拷贝上生效，不会对老的对象产生影响。这个过程也就是常说的Copy-On-Write，会涉及到内存的相关操作。
SP服务刚启动时，成员变量m_strKeyForGetEtcValue引用自另外的变量（初始化时用的Config对象的成员变量），当多个线程同时执行if (strKey[0] == '_')时，operator []会执行多次copy-on-write操作，也就是说会对老数据进行多次引用计数减1；假设有A、B两个线程同时进行，其中A线程稍早完成m_strKeyForGetEtcValue本地化，但B线程并不知情也对引用计数减1，不幸的是这个操作落到了已经本地化的对象上，最终的影响可想而知。将比较行为改为使用string.compare()操作，反复压测再未出现问题。
回头总结这个问题，往往是因不了解std::string operator[]内部实现机制或未意识到其重要性而误用引起的。要想以后不再出现类似问题，一方面得加深自身的编程功底；另外加强编码严谨性，类似的调用均加上const修饰即可。


小黄鸭 交通灯 轮胎 : http://finalmesh.com/webglexamples.htm
鲜花 蝴蝶 金鱼 http://www.ibiblio.org/e-notes/webgl/models.htm
小汽车:http://blog.romanliutikov.com/post/58690562825/external-models-in-threejs

免费的3D模型网：https://clara.io/library


WebVR的参数：
1. 老版本在CardboardDeviceParams.java
2. 新版本在CardboardVRDevice.java（chrome自己写了一个适配类）
3. chrome通过JNI从Android中获取EyeParamters，代码在cardboard_vr_devcice.cc中

强大的C++开源库：机器学习、人工智能、OpenCL、图像识别等等
https://github.com/jobbole/awesome-cpp-cn

http://www.cnblogs.com/haithink/archive/2013/03/12/2956487.html

关于Java与C++的绑定
1. 以C++为主
    C++维护Java的object全局引用，Java保存C++对象的this，方便回调【C++管理Java内存】
    C++对象释放时，调用Java的onDelete函数释放java对this的保存，防止java回调时C++对象已经不存在【C++设置Java的保存的this为nullptr】
2. 以Java为主
    Java维护C++的object的指针，java对象释放时，直接delete即可

VRTookKitSDK必须要求使用gnu++，否则无法编译通过。。老的AppEngine.podspec的配置为，删除即可。
  s.pod_target_xcconfig = {
      'CLANG_CXX_LANGUAGE_STANDARD' => 'c++11',
      'CLANG_CXX_LIBRARY' => 'libc++'
  }

http://dl.google.com/android/ADT-24.0.2.zip

iOS平台EAGL的使用：http://blog.csdn.net/yzfuture2010/article/details/7799800

Canvas小游戏：https://github.com/cn-qiuhong【拼图、100层、识字等等】
Canvas小游戏：https://github.com/guqianfeng【速算】
https://github.com/MrZhw
儿童故事：https://github.com/Swer316828/ChildStory



