mruby-esp32-nvs
===============

Usage
-----

```rb
# build_config.rb / esp32_build_config.rb
conf.gem :github => "mruby-esp32/mruby-esp32-nvs"
```

Examples
--------

### Write values

```rb
ESP32::NVS.open("namespace", ESP32::NVS::READWRITE) do |nvs|
  nvs.set_i8("8", 8)
  nvs.set_i16("16", 16)
  nvs.set_i32("32", 32)
  nvs.set_i64("64", 64) # depends on MRB_INT_BIT
  nvs.set_str("str", "STR")
  nvs.set_blob("blob", "BLOB")
end
```

### Read values

```rb
ESP32::NVS.open("namespace", ESP32::NVS::READONLY) do |nvs|
  nvs.get_i8("8")
  nvs.get_i16("16")
  nvs.get_i32("32")
  nvs.get_i64("64") # depends on MRB_INT_BIT
  nvs.get_str("str")
  nvs.get_blob("blob")
end
```
