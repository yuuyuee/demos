// Copyright 2022 The Oak Authors.

#include "oak/addons/module.h"

#include <assert.h>
#include <dlfcn.h>

#include <utility>
#include <algorithm>

#include "oak/common/throw_delegate.h"
#include "oak/common/stringpiece.h"
#include "oak/logging/logging.h"
#include "oak/common/fs.h"

using std::string;
using std::unique_ptr;

namespace oak {

// ModuleHandle
// Maintain the lifecycle of the underlying handle.

class ModuleHandle {
 public:
  int m_id;
  int m_version;
  std::string m_entry;
  std::string m_path;
  ModuleType m_type;
  LangType m_lang_type;

  // For c/c++ dynamic library
  const struct oak_cpp_handle* m_cpp_handle;

  // For python script
  std::string m_py_script;

 public:
  ModuleHandle() noexcept;
  ~ModuleHandle();

  // Move construct.
  ModuleHandle(ModuleHandle&& other);

  // Move assignment.
  ModuleHandle& operator=(ModuleHandle&& other);

  // Load module with entry name from the path.
  int OpenModule(int id, StringPiece entry, StringPiece path);

  // Close module to free handler.
  void CloseModule();

  OAK_DISALLOW_COPY_AND_ASSIGN(ModuleHandle);

 private:
  bool ownership_;
  void* dl_handle_;

  // Cpp implemented module Open and Close operation.

  // Load cpp module with entry name from the path, if path has empty,
  // load from main program.
  int OpenCppModule(int id, StringPiece entry, StringPiece path);

  // Close module to free handler.
  void CloseCppModule();

  // Move copy assignment operation helper.
  void MoveCppModule(ModuleHandle* other);

  // Python implemented module Open and Close operation.

  // Load module with entry name from the path.
  int OpenPyModule(int id, StringPiece entry, StringPiece path);

  // Close module to free handler.
  void ClosePyModule();

  // Move copy assignment operation helper.
  void MovePyModule(ModuleHandle* other);
};

ModuleHandle::ModuleHandle()
    : m_id(-1), m_version(0), m_entry(), m_path(),
      m_type(MODULE_TYPE_UNKNOWN),
      m_lang_type(LANG_TYPE_UNKNOWN),
      m_cpp_handle(nullptr),
      m_py_script(),
      ownership_(false),
      dl_handle_(nullptr) {}

ModuleHandle::~ModuleHandle() {
  CloseModule();
}

// Move construct.
ModuleHandle::ModuleHandle(ModuleHandle&& other) {
    m_id = other.m_id;
    m_version = other.m_version;
    m_entry = other.m_entry;
    m_path = other.m_path;
    m_lang_type = other.m_lang_type;

    switch (m_lang_type) {
    case LANG_TYPE_C_CPP:
      MoveCppModule(&other);
      break;
    case LANG_TYPE_PYTHON:
      MovePyModule(&other);
      break;
    default: break;
    }

    other.ownership_ = false;
}

// Move assignment.
ModuleHandle& ModuleHandle::operator=(ModuleHandle&& other) {
  if (this == &other)
    return *this;

  if (ownership_)
    CloseModule();

  ownership_ = other.ownership_;
  if (ownership_) {
    m_id = other.m_id;
    m_version = other.m_version;
    m_entry = other.m_entry;
    m_path = other.m_path;
    m_lang_type = other.m_lang_type;

    switch (m_lang_type) {
    case LANG_TYPE_C_CPP:
      MoveCppModule(&other);
      break;
    case LANG_TYPE_PYTHON:
      MovePyModule(&other);
      break;
    default: break;
    }

    other.ownership_ = false;
  }
}

// Load cpp module with entry name from the path, if path has empty,
// load from main program.
int ModuleHandle::OpenModule(int id, StringPiece entry, StringPiece path) {
  assert(!entry.empty() && "invalid module entry");
  int ret;
  if (path.empty() || path.ends_with(".so")) {
    ret = OpenCppModule(id, entry, path);
  } else if (StringPiece(path).ends_with(".py")) {
    ret = OpenCppModule(id, entry, path);
  } else {
    OAK_ERROR("Open module %*s: Unidentified module\n",
              path.size(), path.data());
    ret = -1;
  }

  if (ret == 0)
    ownership_ = true;

  OAK_INFO(
      "Module ID: %d\n"
      "Module Version: %d\n"
      "Module Entry: %s\n"
      "Module Path: %s\n"
      "Module Type: %s\n"
      "Module Language Type: %s\n"
      "Module loaded successfully.\n",
      m_id, m_version, m_entry.c_str(),
      m_path.empty() ? "builtin" : m_path.c_str(),
      ModuleTypeName(m_type), LangTypeName(m_lang_type));

  return ret;
}

// Close module to free handler.
void ModuleHandle::CloseModule() {
  if (ownership_) {
    switch (m_lang_type) {
    case LANG_TYPE_C_CPP:
      CloseCppModule();
      break;
    case LANG_TYPE_PYTHON:
      ClosePyModule();
      break;
    default:
      break;
    }
    ownership_ = false;
  }
}

// Cpp implemented module Open and Close operation.

int ModuleHandle::OpenCppModule(int id, StringPiece entry, StringPiece path) {
  m_id = id;
  m_entry = string(entry);
  m_path = string(path);

  const char* rpath = m_path.empty() ? nullptr : m_path.c_str();
  const char* err = dlerror();
  void* handle = dlopen(rpath, RTLD_NOW | RTLD_GLOBAL);
  if (!handle) {
    OAK_ERROR("Open module %s: dlopen failed: %s\n",
              m_entry.c_str(), dlerror());
    return -1;
  }

  dl_handle_ = handle;

  const char* err = dlerror();
  void* cpp_handle = dlsym(handle, m_entry.c_str());
  if (!handle) {
    CloseCppModule();
    OAK_ERROR("Open module %s: Not found entry: %s\n",
              m_entry.c_str(), dlerror());
    return -1;
  }

  m_cpp_handle = static_cast<struct oak_cpp_handle*>(cpp_handle);
  m_version = m_cpp_handle->version;
  if ((m_version >> 16) < OAK_VERSION_MAJOR) {
    CloseCppModule();
    OAK_ERROR("Open module %s: Incompatible version\n", m_entry.c_str());
    return -1;
  }
  m_type = static_cast<ModuleType>(m_cpp_handle->flags);
  m_lang_type = LANG_TYPE_C_CPP;
  return 0;
}

void ModuleHandle::CloseCppModule() {
  const char* err = dlerror();
  int ret = dlclose(dl_handle_);
  if (ret != 0) {
    OAK_ERROR("Open module %s: dlclose() failed: %s\n",
              m_entry.c_str(), dlerror());
  }
}

void ModuleHandle::MoveCppModule(ModuleHandle* other) {
  m_cpp_handle = other->m_cpp_handle;
  dl_handle_ = other->dl_handle_;
}

// Loading module with entry @module_name from the path.
int ModuleHandle::OpenPyModule(int, StringPiece, StringPiece) {
  // TODO(YUYUE): Not implemented
  return -ENOSYS;
}

// Close module to free handler.
void ModuleHandle::ClosePyModule() {
  // TODO(YUYUE): Not implemented
}

void ModuleHandle::MovePyModule(ModuleHandle* other) {
  // TODO(YUYUE): Not implemented
  m_py_script = other->m_py_script;
}


// CppEventModule

// Cpp implemented event module.

class CppEventModule: public EventModule {
 public:
  explicit CppEventModule(ModuleHandle&& base, void* ctx);
  virtual ~CppEventModule();

  // pull out the previous event from event module.
  virtual int Pull(struct incoming_event* event, int size);

  // Receive the event from event module.
  virtual int Recv(struct incoming_event* event);

  // Send the event to event module.
  virtual int Send(const struct outgoing_event* event);

  // Get unique ID of the event module.
  virtual int id() const noexcept;

  // Get unique name of the event module.
  virtual std::string name() const noexcept;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(CppEventModule);

  ModuleHandle base_;

};

CppEventModule::CppEventModule(ModuleHandle&& base)


// PyEventModule

// Python implemented event module.

class PyEventModule;

// ================
// EventModule
// ================

EventModule::EventModule() {}
EventModule::~EventModule() {}

// Create a event module, return 0 on success, -1 if any error
// occursed.
int EventModuleFactory(const std::string& module_name,
                       const Dict& config,
                       std::unique_ptr<EventModule>* module) {

}

// CppSourceModule

// Cpp implemented source module.

class CppSourceModule {
 public:
  CppSourceModule();
  virtual ~CppSourceModule();

  // Read the buffer and decoded as the metadata.
  virtual int Read(struct oak_metadata* metadata);

  // Get unique ID of the source module.
  virtual int id() const noexcept = 0;

  // Get unique name of the source module.
  virtual std::string name() const noexcept = 0;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(CppSourceModule);
};

// PySourceModule

// Python implemented source module.

class PySourceModule {
 public:
  PySourceModule();
  virtual ~PySourceModule();

  // Read the buffer and decoded as the metadata.
  virtual int Read(struct oak_metadata* metadata);

  // Get unique ID of the source module.
  virtual int id() const noexcept = 0;

  // Get unique name of the source module.
  virtual std::string name() const noexcept = 0;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(PySourceModule);
};

// ================
// SourceModule
// ================

SourceModule::SourceModule() {}
SourceModule::~SourceModule() {}

// Create a source module, return 0 on success, -1 if any error
// occursed.
int SourceModuleFactory(const std::string& module_name,
                        const Dict& config,
                        std::unique_ptr<SourceModule>* module) {

}

// CppEventModule

// Cpp implemented parser module.

class CppParserModule {
 public:
  CppParserModule();
  virtual ~CppParserModule();

  // Parse stream to extract the fields.
  virtual int Parse(const struct oak_buffer_ref* up_stream,
                    const struct oak_buffer_ref* down_stream,
                    struct oak_dict* fields);

  // Parse stream to indicate whether or not the stream should
  // be controlled.
  virtual int Mark(const struct oak_buffer_ref* up_stream,
                   const struct oak_buffer_ref* down_stream);

  // Get unique ID of the parser module.
  virtual int id() const noexcept;

  // Get unique name of the parser module.
  virtual std::string name() const noexcept;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(CppParserModule);
};

// PyParserModule

// Python implemented parser module.

class PyParserModule {
 public:
  PyParserModule();
  virtual ~PyParserModule();

  // Parse stream to extract the fields.
  virtual int Parse(const struct oak_buffer_ref* up_stream,
                    const struct oak_buffer_ref* down_stream,
                    struct oak_dict* fields);

  // Parse stream to indicate whether or not the stream should
  // be controlled.
  virtual int Mark(const struct oak_buffer_ref* up_stream,
                   const struct oak_buffer_ref* down_stream);

  // Get unique ID of the parser module.
  virtual int id() const noexcept;

  // Get unique name of the parser module.
  virtual std::string name() const noexcept;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(PyParserModule);
};

// ================
// ParserModule
// ================

ParserModule::ParserModule() {}
ParserModule::~ParserModule() {}

// Create a parser module, return 0 on success, -1 if any error
// occursed.
int ParserModuleFactory(const std::string& module_name,
                        const Dict& config,
                        std::unique_ptr<ParserModule>* module) {

}

// CppSinkModule

// Cpp implemented sink module.

class CppSinkModule: public SinkModule {
 public:
  explicit CppSinkModule(ModuleHandle&& base);
  virtual ~CppSinkModule();

  // Write the metadata to module specified the sink.
  virtual int Write(const struct oak_metadata* metadata) = 0;

  // Get unique ID of the sink module.
  virtual int id() const noexcept = 0;

  // Get unique name of the sink module.
  virtual std::string name() const noexcept = 0;

 private:
  OAK_DISALLOW_COPY_AND_ASSIGN(CppSinkModule);
};

// ================
// SinkModule
// ================

SinkModule::SinkModule() {}
SinkModule::~SinkModule() {}

// Create a sink module, return 0 on success, -1 if any error
// occursed.
int SinkModuleFactory(const std::string& module_name,
                      const Dict& config,
                      std::unique_ptr<SinkModule>* module) {

}
}  // namespace oak
