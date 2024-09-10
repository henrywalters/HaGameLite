#include <stdexcept>

#include "../../../include/hagame/utils/libLoader.h"

using namespace hg::utils;

LibLoader::LibLoader(std::string path):
	m_path(path),
	m_library(nullptr)
{
	reload();
}

hg::utils::LibLoader::~LibLoader()
{
	if (!m_library) {
		return;
	}
#ifdef __UNIX__
	dlclose(m_library)
#endif

#ifdef _WIN32
	FreeLibrary(static_cast<HMODULE>(m_library));
#endif
}

void hg::utils::LibLoader::reload()
{
#ifdef __UNIX__
	m_library = dlopen(m_path.c_str(), RTLD_NOW);
#endif

#ifdef _WIN32
	m_library = static_cast<HMODULE>(LoadLibrary(m_path.c_str()));
#endif

	if (m_library == nullptr) {
		throw std::runtime_error("Failed to open library or dll");
	}
}