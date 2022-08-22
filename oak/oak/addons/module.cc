// Copyright 2022 The Oak Authors.

#include "oak/addons/module.h"

#include <assert.h>
#include <dlfcn.h>

#include <utility>
#include <algorithm>

#include "oak/common/stringpiece.h"
#include "oak/logging/logging.h"
#include "oak/common/fs.h"

namespace oak {

// Load cpp module with entry name from the path, if path has empty,
// load from main program.
int OpenModule(int id, StringPiece entry, StringPiece path) {
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
void ClosePyModule() {
  // TODO(YUYUE): Not implemented
}

void MovePyModule(ModuleHandle* other) {
  // TODO(YUYUE): Not implemented
  m_py_script = other->m_py_script;
}


// EventInfo

ModuleBase::ModuleBase() = default;
ModuleBase::~ModuleBase() = default;

}  // namespace oak
