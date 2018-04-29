MRuby::Gem::Specification.new('esp32-nvs') do |spec|
  spec.license = 'MIT'
  spec.author  = 'Seiei Miyagi'
  spec.summary = 'ESP-IDF Non-volatile storage library wrapper'
  spec.add_dependency("esp32-ext_esp_err")
end
