#include <mruby.h>
/* #include <mruby/array.h> */
/* #include <mruby/class.h> */
#include <mruby/data.h>
/* #include <mruby/hash.h> */
#include <mruby/string.h>
/* #include <mruby/value.h> */
#include <mruby/variable.h>
#include <esp_err.h>
#include <nvs_flash.h>
#include <mruby-esp32-ext_esp_err.h>

void mrb_esp32_nvs_free(mrb_state *mrb, void *out_handle) {
  if (out_handle) {
    nvs_close((nvs_handle)out_handle);
  }
}

static const struct mrb_data_type mrb_esp32_nvs_type = {
  "nvs", mrb_esp32_nvs_free
};

/*
 * @example
 *   ESP32::NVS.init
 */
static mrb_value mrb_esp32_nvs_s_init(mrb_state *mrb, mrb_value self) {
  esp_err_t err = nvs_flash_init();
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_nil_value();
}

/*
 * @example
 *   ESP32::NVS.init_partition("partition label")
 */
static mrb_value mrb_esp32_nvs_s_init_partition(mrb_state *mrb, mrb_value self) {
  char *part_name;
  mrb_get_args(mrb, "z", &part_name);

  esp_err_t err = nvs_flash_init_partition(part_name);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_nil_value();
}

/*
 * @example
 *   ESP32::NVS.deinit
 */
static mrb_value mrb_esp32_nvs_s_deinit(mrb_state *mrb, mrb_value self) {
  esp_err_t err = nvs_flash_deinit();
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_nil_value();
}

/*
 * @example
 *   ESP32::NVS.deinit_partition("partition label")
 */
static mrb_value mrb_esp32_nvs_s_deinit_partition(mrb_state *mrb, mrb_value self) {
  char *part_name;
  mrb_get_args(mrb, "z", &part_name);

  esp_err_t err = nvs_flash_deinit_partition(part_name);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_nil_value();
}

/*
 * @example
 *   ESP32::NVS.erase
 */
static mrb_value mrb_esp32_nvs_s_erase(mrb_state *mrb, mrb_value self) {
  esp_err_t err = nvs_flash_erase();
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_nil_value();
}

/*
 * @example
 *   ESP32::NVS.erase_partition("partition label")
 */
static mrb_value mrb_esp32_nvs_s_erase_partition(mrb_state *mrb, mrb_value self) {
  char *part_name;
  mrb_get_args(mrb, "z", &part_name);

  esp_err_t err = nvs_flash_erase_partition(part_name);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_nil_value();
}

/*
 * @example
 *   open_mode = ESP32::NVS::READWRITE
 *   ESP32::NVS.new("name", open_mode)
 *   ESP32::NVS.new("part name", "name", open_mode)
 */
static mrb_value mrb_esp32_nvs_init(mrb_state *mrb, mrb_value self) {
  mrb_value part_name, name, open_mode;
  if (mrb_get_args(mrb, "oo|o", &part_name, &name, &open_mode) == 2) {
    open_mode = name;
    name = part_name;
    part_name = mrb_const_get(mrb, mrb_obj_value(mrb_class_get_under(mrb, mrb_module_get(mrb, "ESP32"), "NVS")), mrb_intern_lit(mrb, "DEFAULT_PART_NAME"));
  }

  if (!mrb_string_p(part_name)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "part_name should be a string");
  }

  if (!mrb_string_p(name)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "name should be a string");
  }

  if (!mrb_fixnum_p(open_mode)) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "opt_mode should be a integer");
  }

  nvs_handle out_handle;
  esp_err_t err = nvs_open_from_partition(RSTRING_PTR(part_name), RSTRING_PTR(name), (nvs_open_mode)mrb_fixnum(open_mode), &out_handle);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  mrb_data_init(self, (void *)out_handle, &mrb_esp32_nvs_type);

  return self;
}

#if MRB_INT_BIT >= 8
/*
 * @example
 *   nvs.set_i8("key", 42)
 */
static mrb_value mrb_esp32_nvs_set_i8(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_int value;
  mrb_get_args(mrb, "zi", &key, &value);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  esp_err_t err = nvs_set_i8(handle, key, (int8_t)value);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_fixnum_value(value);
}

/*
 * @example
 *   nvs.get_i8("key")
 */
static mrb_value mrb_esp32_nvs_get_i8(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_get_args(mrb, "z", &key);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  int8_t out_value;
  esp_err_t err = nvs_get_i8(handle, key, &out_value);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_fixnum_value((mrb_int)out_value);
}
#endif

#if MRB_INT_BIT >= 16
/*
 * @example
 *   nvs.set_i16("key", 42)
 */
static mrb_value mrb_esp32_nvs_set_i16(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_int value;
  mrb_get_args(mrb, "zi", &key, &value);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  esp_err_t err = nvs_set_i16(handle, key, (int16_t)value);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_fixnum_value(value);
}

/*
 * @example
 *   nvs.get_i16("key")
 */
static mrb_value mrb_esp32_nvs_get_i16(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_get_args(mrb, "z", &key);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  int16_t out_value;
  esp_err_t err = nvs_get_i16(handle, key, &out_value);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_fixnum_value((mrb_int)out_value);
}
#endif

#if MRB_INT_BIT >= 32
/*
 * @example
 *   nvs.set_i32("key", 42)
 */
static mrb_value mrb_esp32_nvs_set_i32(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_int value;
  mrb_get_args(mrb, "zi", &key, &value);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  esp_err_t err = nvs_set_i32(handle, key, (int32_t)value);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_fixnum_value(value);
}

/*
 * @example
 *   nvs.get_i32("key")
 */
static mrb_value mrb_esp32_nvs_get_i32(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_get_args(mrb, "z", &key);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  int32_t out_value;
  esp_err_t err = nvs_get_i32(handle, key, &out_value);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_fixnum_value((mrb_int)out_value);
}
#endif

#if MRB_INT_BIT >= 64
/*
 * @example
 *   nvs.set_i64("key", 42)
 */
static mrb_value mrb_esp32_nvs_set_i64(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_int value;
  mrb_get_args(mrb, "zi", &key, &value);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  esp_err_t err = nvs_set_i64(handle, key, (int64_t)value);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_fixnum_value(value);
}

/*
 * @example
 *   nvs.get_i64("key")
 */
static mrb_value mrb_esp32_nvs_get_i64(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_get_args(mrb, "z", &key);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  int64_t out_value;
  esp_err_t err = nvs_get_i64(handle, key, &out_value);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_fixnum_value((mrb_int)out_value);
}
#endif

/*
 * @example
 *   nvs.set_str("key", "hi")
 */
static mrb_value mrb_esp32_nvs_set_str(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_value value;
  mrb_get_args(mrb, "zS", &key, &value);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  esp_err_t err = nvs_set_str(handle, key, RSTRING_PTR(value));
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return value;
}

/*
 * @example
 *   nvs.get_str("key")
 */
static mrb_value mrb_esp32_nvs_get_str(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_get_args(mrb, "z", &key);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  size_t length;
  esp_err_t err = nvs_get_str(handle, key, NULL, &length);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  mrb_value str = mrb_str_new(mrb, NULL, length);
  err = nvs_get_str(handle, key, RSTRING_PTR(str), &length);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return str;
}

/*
 * @example
 *   nvs.set_blob("key", "hi")
 */
static mrb_value mrb_esp32_nvs_set_blob(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_value value;
  mrb_get_args(mrb, "zS", &key, &value);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  esp_err_t err = nvs_set_blob(handle, key, RSTRING_PTR(value), RSTRING_LEN(value));
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return value;
}

/*
 * @example
 *   nvs.get_blob("key")
 */
static mrb_value mrb_esp32_nvs_get_blob(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_get_args(mrb, "z", &key);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  size_t length;
  esp_err_t err = nvs_get_blob(handle, key, NULL, &length);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  mrb_value str = mrb_str_new(mrb, NULL, length);
  err = nvs_get_blob(handle, key, RSTRING_PTR(str), &length);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return str;
}

/*
 * @example
 *   nvs.erase_key("key")
 */
static mrb_value mrb_esp32_nvs_erase_key(mrb_state *mrb, mrb_value self) {
  char *key;
  mrb_get_args(mrb, "z", &key);

  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  esp_err_t err = nvs_erase_key(handle, key);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_nil_value();
}

/*
 * @example
 *   nvs.erase_all
 */
static mrb_value mrb_esp32_nvs_erase_all(mrb_state *mrb, mrb_value self) {
  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  esp_err_t err = nvs_erase_all(handle);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_nil_value();
}

/*
 * @example
 *   nvs.commit
 */
static mrb_value mrb_esp32_nvs_commit(mrb_state *mrb, mrb_value self) {
  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  esp_err_t err = nvs_commit(handle);
  if (err != ESP_OK) {
    mrb_raise_esp32_err(mrb, err);
  }

  return mrb_nil_value();
}

/*
 * @example
 *   nvs.close
 */
static mrb_value mrb_esp32_nvs_close(mrb_state *mrb, mrb_value self) {
  nvs_handle handle = (nvs_handle)DATA_PTR(self);
  nvs_close(handle);
  return mrb_nil_value();
}

void mrb_esp32_nvs_gem_init(mrb_state* mrb) {
  struct RClass *mrb_esp32, *mrb_esp32_nvs;

  mrb_esp32 = mrb_define_module(mrb, "ESP32");

  /*
   * ESP-IDF Non-volatile storage library wrapper class.
   *
   * c.f. {http://esp-idf.readthedocs.io/en/v3.0/api-reference/storage/nvs_flash.html Non-volatile storage library documentation}
   */
  mrb_esp32_nvs = mrb_define_class_under(mrb, mrb_esp32, "NVS", mrb->object_class);
  mrb_define_class_method(mrb, mrb_esp32_nvs, "init", mrb_esp32_nvs_s_init, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, mrb_esp32_nvs, "init_partition", mrb_esp32_nvs_s_init_partition, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, mrb_esp32_nvs, "deinit", mrb_esp32_nvs_s_deinit, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, mrb_esp32_nvs, "deinit_partition", mrb_esp32_nvs_s_deinit_partition, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, mrb_esp32_nvs, "erase", mrb_esp32_nvs_s_erase, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, mrb_esp32_nvs, "erase_partition", mrb_esp32_nvs_s_erase_partition, MRB_ARGS_REQ(1));

  mrb_define_method(mrb, mrb_esp32_nvs, "initialize", mrb_esp32_nvs_init, MRB_ARGS_REQ(2));

#if MRB_INT_BIT >= 8
  mrb_define_method(mrb, mrb_esp32_nvs, "set_i8", mrb_esp32_nvs_set_i8, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mrb_esp32_nvs, "get_i8", mrb_esp32_nvs_get_i8, MRB_ARGS_REQ(1));
#endif
#if MRB_INT_BIT >= 16
  mrb_define_method(mrb, mrb_esp32_nvs, "set_i16", mrb_esp32_nvs_set_i16, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mrb_esp32_nvs, "get_i16", mrb_esp32_nvs_get_i16, MRB_ARGS_REQ(1));
#endif
#if MRB_INT_BIT >= 32
  mrb_define_method(mrb, mrb_esp32_nvs, "set_i32", mrb_esp32_nvs_set_i32, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mrb_esp32_nvs, "get_i32", mrb_esp32_nvs_get_i32, MRB_ARGS_REQ(1));
#endif
#if MRB_INT_BIT >= 64
  mrb_define_method(mrb, mrb_esp32_nvs, "set_i64", mrb_esp32_nvs_set_i64, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mrb_esp32_nvs, "get_i64", mrb_esp32_nvs_get_i64, MRB_ARGS_REQ(1));
#endif
  mrb_define_method(mrb, mrb_esp32_nvs, "set_str", mrb_esp32_nvs_set_str, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mrb_esp32_nvs, "get_str", mrb_esp32_nvs_get_str, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb_esp32_nvs, "set_blob", mrb_esp32_nvs_set_blob, MRB_ARGS_REQ(2));
  mrb_define_method(mrb, mrb_esp32_nvs, "get_blob", mrb_esp32_nvs_get_blob, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb_esp32_nvs, "erase_key", mrb_esp32_nvs_erase_key, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, mrb_esp32_nvs, "erase_all", mrb_esp32_nvs_erase_all, MRB_ARGS_NONE());
  mrb_define_method(mrb, mrb_esp32_nvs, "commit", mrb_esp32_nvs_commit, MRB_ARGS_NONE());
  mrb_define_method(mrb, mrb_esp32_nvs, "close", mrb_esp32_nvs_close, MRB_ARGS_NONE());

  /* nvs_open_mode */
  mrb_define_const(mrb, mrb_esp32_nvs, "READONLY", mrb_fixnum_value(NVS_READONLY));
  mrb_define_const(mrb, mrb_esp32_nvs, "READWRITE", mrb_fixnum_value(NVS_READWRITE));

  /* macros */
  mrb_define_const(mrb, mrb_esp32_nvs, "DEFAULT_PART_NAME", mrb_str_new_cstr(mrb, NVS_DEFAULT_PART_NAME));
}

void mrb_esp32_nvs_gem_final(mrb_state* mrb) {
}
