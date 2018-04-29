module ESP32
  class NVS
    # @example Write values
    #   ESP32::NVS.open("namespace", ESP32::NVS::READWRITE) do |nvs|
    #     nvs.set_i8("8", 8)
    #     nvs.set_i16("16", 16)
    #     nvs.set_i32("32", 32)
    #     nvs.set_i64("64", 64) # depends on MRB_INT_BIT
    #     nvs.set_str("str", "STR")
    #     nvs.set_blob("blob", "BLOB")
    #   end
    #
    # @example Read values
    #   ESP32::NVS.open("namespace", ESP32::NVS::READONLY) do |nvs|
    #     nvs.get_i8("8")
    #     nvs.get_i16("16")
    #     nvs.get_i32("32")
    #     nvs.get_i64("64") # depends on MRB_INT_BIT
    #     nvs.get_str("str")
    #     nvs.get_blob("blob")
    #   end
    #
    # @example Read value from different partition
    #   ESP32::NVS.open("music", "bgm", ESP32::NVS::READONLY) do |nvs|
    #     nvs.get_blob("1.mp3")
    #   end
    def self.open(part_name = ESP32::NVS::DEFAULT_PART_NAME, name, open_mode)
      nvs = new(part_name, name, open_mode)
      begin
        yield nvs
        nvs.commit
      ensure
        nvs.close
      end
    end
  end
end
