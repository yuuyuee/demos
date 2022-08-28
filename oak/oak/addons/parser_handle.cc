// Copyright 2022 The Oak Authors.

#include "oak/addons/parser_handle.h"

#include <assert.h>
#include <dlfcn.h>
#include "oak/addons/metadata_internal.h"
#include "oak/logging/logging.h"

using std::string;
using std::unique_ptr;

namespace oak {

// ParserHandle

ParserHandle::ParserHandle(size_t id, const string& name, const string& path)
    : ModuleBase(id, name, path) {}

ParserHandle::~ParserHandle() {}

// CppParserHandle

// Cpp implemented parser module.

class CppParserHandle: public ParserHandle {
 public:
  explicit CppParserHandle(size_t id,
                          const string& name,
                          const string& path,
                          void* dl_handle,
                          struct oak_parser_module* handle,
                          void* context);

  virtual ~CppParserHandle();

  CppParserHandle(CppParserHandle const&) = delete;
  CppParserHandle& operator=(CppParserHandle const&) = delete;

  // Parse stream to extracting to filling the fields.
  virtual int Parse(const struct oak_buffer* up_stream,
                    const struct oak_buffer* down_stream,
                    const struct oak_dict* extracted,
                    struct oak_dict* fields) {
    if (!handle_->parse)
      return -1;
    return handle_->parse(context_, up_stream, down_stream, extracted, fields);
  }

  // Parse stream to indicate whether or not the stream should
  // be controlled.
  virtual int Mark(const struct oak_buffer* up_stream,
                   const struct oak_buffer* down_stream) {
    if (!handle_->mark)
      return -1;
    return handle_->mark(context_, up_stream, down_stream);
  }

 private:
  void* dl_handle_;
  struct oak_parser_module* handle_;
  void* context_;
};

CppParserHandle::CppParserHandle(size_t id,
                                 const string& name,
                                 const string& path,
                                 void* dl_handle,
                                 struct oak_parser_module* handle,
                                 void* context)
    : ParserHandle(id, name, path),
      dl_handle_(dl_handle), handle_(handle), context_(context) {
  assert(!name.empty() && "Invalid module name");
  assert(dl_handle && "Invalid dl_handle");
  assert(handle && "Invalid handle");
  version_ = handle_->version;
  type_ = MODULE_TYPE_PARSER;
  lang_type_ = LANG_TYPE_C_CPP;
}

CppParserHandle::~CppParserHandle() {
  handle_->close(context_);
  dlerror();
  if (dlclose(dl_handle_) != 0) {
    OAK_WARNING("Close module %s: dlclose failed: %s\n",
                name_.c_str(), dlerror());
  }
}

int CppParserHandleFactory(const ModuleConfig& module_config,
                           unique_ptr<ParserHandle>* module_handle) {
  assert(!module_config.name.empty() && "Invalid module name");
  const char* path = module_config.path.empty() ?
      nullptr : module_config.path.c_str();
  dlerror();
  void* dl_handle = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
  if (!dl_handle) {
    OAK_ERROR("Open module %s: dlopen failed: %s\n",
              module_config.name.c_str(), dlerror());
    return -1;
  }

  dlerror();
  void* symbol = dlsym(dl_handle, module_config.name.c_str());
  if (!symbol) {
    OAK_ERROR("Open module %s: dlsym failed: %s\n",
              module_config.name.c_str(), dlerror());
    dlclose(dl_handle);
    return -1;
  }

  struct oak_parser_module* handle =
      reinterpret_cast<struct oak_parser_module*>(symbol);

  if (handle->flags != MODULE_TYPE_PARSER) {
    OAK_ERROR("Open module %s: Unidentify module flags: %d\n",
              module_config.name.c_str(), handle->flags);
    dlclose(dl_handle);
    return -1;
  }

  if ((handle->version >> 16) != OAK_VERSION_MAJOR) {
    OAK_ERROR("Open module %s: Module version is too old: %d\n",
              module_config.name.c_str(), handle->version);
    dlclose(dl_handle);
    return -1;
  }

  struct oak_dict dict;
  oak_dict_init2(&dict);
  for (auto const& it : module_config.config) {
    if (dict.size >= OAK_DICT_DFL_CAP)
      break;
    if (it.first.empty() || it.second.empty())
      continue;
    oak_dict_ref(&dict,
        it.first.c_str(), it.first.size(),
        it.second.c_str(), it.second.size());
  }

  void* context = nullptr;
  int ret = handle->init(&context, &dict);
  if (ret != 0) {
    OAK_ERROR("Open module %s: Module initialization failed\n",
              module_config.name.c_str());
    dlclose(dl_handle);
    return -1;
  }

  CppParserHandle* object =
      new CppParserHandle(module_config.id,
                         module_config.name,
                         module_config.path,
                         dl_handle, handle, context);
  object->Dump();
  module_handle->reset(object);
  return 0;
}

// PyParserHandle

// Python implemented parser module.

class PyParserHandle: public ParserHandle {
 public:
  virtual ~PyParserHandle();

  // Parse stream to extract the fields.
  virtual int Parse(const struct oak_buffer* up_stream,
                    const struct oak_buffer* down_stream,
                    const struct oak_dict* extracted,
                    struct oak_dict* fields) {
    // TODO(YUYUE): implemented
    (void) up_stream;
    (void) down_stream;
    (void) extracted;
    (void) fields;
    return -1;
  }

  // Parse stream to indicate whether or not the stream should
  // be controlled.
  virtual int Mark(const struct oak_buffer* up_stream,
                   const struct oak_buffer* down_stream) {
    // TODO(YUYUE): implemented
    (void) up_stream;
    (void) down_stream;
    return -1;
  }

 protected:
  PyParserHandle();
};

PyParserHandle::PyParserHandle(): ParserHandle(0, "", "") {}

PyParserHandle::~PyParserHandle() {}

int PyParserHandleFactory(const ModuleConfig& module_config,
                          unique_ptr<ParserHandle>* module_handle) {
  // TODO(YUYUE): implemented
  (void) module_config;
  (void) module_handle;
  return -1;
}

// Create a parser handle, return 0 on success, -1 if any error
// occursed.
int ParserHandleFactory(const ModuleConfig& module_config,
                        unique_ptr<ParserHandle>* module_handle) {
  assert(!module_config.name.empty() && "Invalid module name");
  if (module_config.path.empty() ||
      module_config.path.find(".so") != string::npos) {
    return CppParserHandleFactory(module_config, module_handle);
  }

  OAK_ERROR("Open module %s: Unidentified module\n",
            module_config.name.c_str());
  return -1;
}

}  // namespace oak
