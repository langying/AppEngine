Pod::Spec.new do |s|
  s.name        = 'AppEngine'
  s.version     = '1.0.0'
  s.author      = {'hanqiong' => 'langying.hq@taobao.com'}
  s.license     = {:type => 'BSD'}
  s.source      = {:git  => 'http://gitlab.alibaba-inc.com/langying.hq/AppEngine.git', :tag => s.version.to_s}
  s.summary     = 'basic ios developer tools.'
  s.homepage    = 'http://gitlab.alibaba-inc.com/langying.hq/AppEngine'
  s.description = 'basic ios developer tools with lots of categories.'

  s.platform      = :ios
  s.requires_arc  = false
  s.resources     = 'AppEngine/**/*.bundle'
  s.framework     = 'GLKit', 'UIKit', 'OpenAL', 'OpenGLES', 'AVFoundation', 'AudioToolbox', 'CoreGraphics', 'CoreMedia', 'CoreMotion', 'CoreVideo'
  s.ios.deployment_target = '8.0'

  s.public_header_files = [
    'AppEngine/AppEngine.h',
    'AppEngine/Common/Utils/AEBase.h',
    'AppEngine/Platform/AEView.h',
  ]

  s.source_files = [
    'AppEngine/**/*.{h,hpp,c,m,mm,cpp}',
  ]

  s.library = 'c++'
end
